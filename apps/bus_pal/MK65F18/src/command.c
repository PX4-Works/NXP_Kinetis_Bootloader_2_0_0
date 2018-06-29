/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "memory/memory.h"
#include "sbloader/sbloader.h"
#include "property/property.h"
#include "utilities/fsl_assert.h"
#include <string.h>
#include <stdint.h>
#include "command.h"
#include "packet/serial_packet.h"
#include "crc/crc16.h"
#include "utilities/fsl_rtos_abstraction.h"

#include "flash/fsl_flash.h"
#include "fsl_device_registers.h"

#if defined(ENABLE_USB)
#include "usb_class_hid.h"
#include "usb_device_stack_interface.h"
#include "usb_hid.h"
#include "bootloader_hid_report_ids.h"
#include "usb_error.h"
#include "usb_descriptor.h"
#include "usb_class_hid.h"
#endif

#include "bus_pal_hardware.h"

//! @addtogroup command
//! @{

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Command processor state data.
command_processor_data_t g_commandData;
buspal_state_t g_buspalState = kBuspal_Idle;
uint8_t g_targetRxBuffer[64];

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

void handle_reset(uint8_t *packet, uint32_t packetLength);
void send_generic_response(uint32_t commandStatus, uint32_t commandTag);

static status_t handle_command_internal(uint8_t *packet, uint32_t packetLength);
static status_t handle_data(bool *hasMoreData);
static uint16_t calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data);
static status_t handle_data_read(bool *hasMoreData);
static void handle_read_memory_command(uint8_t *packet, uint32_t packetLength);
static void finalize_data_phase(status_t status);
static void reset_data_phase();
static status_t peripheral_read(uint8_t *dest, uint32_t readLength);
static status_t peripheral_write(uint8_t *src, uint32_t writeLength);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void handleUsbBusPalCommand()
{
    g_commandData.state = kCommandState_CommandPhase;

    while (1)
    {
        bootloader_command_pump();
    }
}

static void handle_config_i2c(uint8_t *packet, uint32_t packetLength)
{
    configure_i2c_packet_t *command = (configure_i2c_packet_t *)packet;
    configure_i2c_address(command->address);
    configure_i2c_speed(command->speed);

    g_buspalState = kBuspal_I2c;

    send_generic_response(kStatus_Success, command->commandPacket.commandTag);
}

static void handle_config_spi(uint8_t *packet, uint32_t packetLength)
{
    configure_spi_packet_t *command = (configure_spi_packet_t *)packet;
    configure_spi_speed(command->speedKhz);
    configure_spi_settings((dspi_clock_polarity_t)command->polarity, (dspi_clock_phase_t)command->phase,
                           (dspi_shift_direction_t)command->direction);
    g_buspalState = kBuspal_Spi;

    send_generic_response(kStatus_Success, command->commandPacket.commandTag);

    // send ping to target and get response
    uint8_t header[2] = { kFramingPacketStartByte, kFramingPacketType_Ping };
    uint8_t response[10];
    uint8_t count = 32;
    peripheral_write(header, 2);
    do
    {
        peripheral_read(header, 1);
    } while ((header[0] != kFramingPacketStartByte) && (count-- != 0));
    peripheral_read(response, sizeof(ping_response_t) + 1);
}

static void handle_config_can(uint8_t *packet, uint32_t packetLength)
{
    configure_can_packet_t *command = (configure_can_packet_t *)packet;
    configure_can_speed(command->speed);
    configure_can_txid(command->txid);
    configure_can_rxid(command->rxid);

    g_buspalState = kBuspal_FlexCAN;

    send_generic_response(kStatus_Success, command->commandPacket.commandTag);
}

static void handle_read_memory_command(uint8_t *packet, uint32_t packetLength)
{
    read_memory_packet_t *command = (read_memory_packet_t *)packet;

    // Start the data phase.
    reset_data_phase();
    g_commandData.dataPhase.count = command->byteCount;
    g_commandData.dataPhase.address = command->startAddress;
    g_commandData.dataPhase.commandTag = kCommandTag_ReadMemory;
}

static void handle_write_memory_command(uint8_t *packet, uint32_t packetLength)
{
    read_memory_packet_t *command = (read_memory_packet_t *)packet;

    // Start the data phase.
    reset_data_phase();
    g_commandData.dataPhase.count = command->byteCount;
    g_commandData.dataPhase.address = command->startAddress;
    g_commandData.dataPhase.commandTag = kCommandTag_WriteMemory;
}

//! @brief Calculate crc over framing data packet.
static uint16_t calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data)
{
    uint16_t crc16;

    // Initialize the CRC16 information
    crc16_data_t crcInfo;
    crc16_init(&crcInfo);

    // Run CRC on all header bytes besides the CRC field
    crc16_update(&crcInfo, (uint8_t *)&packet->header.startByte, sizeof(framing_data_packet_t) - sizeof(uint16_t));

    // Continue running CRC on any payload bytes
    crc16_update(&crcInfo, data, packet->length);

    // Finalize the CRC calculations
    crc16_finalize(&crcInfo, &crc16);

    return crc16;
}

static status_t peripheral_read(uint8_t *dest, uint32_t readLength)
{
    status_t status = kStatus_Success;

    switch (g_buspalState)
    {
        case kBuspal_I2c:
            status = receive_i2c_data(dest, readLength);
            break;
        case kBuspal_Spi:
            receive_spi_data(dest, readLength);
            break;
        case kBuspal_FlexCAN:
            read_can_data(dest, readLength);
            break;
        default:
            status = kStatus_Fail;
            break;
    }

    return status;
}

static status_t peripheral_write(uint8_t *src, uint32_t writeLength)
{
    status_t status = kStatus_Success;

    switch (g_buspalState)
    {
        case kBuspal_I2c:
            status = send_i2c_data(src, writeLength);
            break;
        case kBuspal_Spi:
            send_spi_data(src, writeLength);
            break;
        case kBuspal_FlexCAN:
            send_can_data(src, writeLength);
            break;
        default:
            status = kStatus_Fail;
            break;
    }

    return status;
}

static void finalize_data_phase(status_t status)
{
    uint8_t *rxBuf = (uint8_t *)g_targetRxBuffer;
    uint8_t rxSize = 0;
    framing_sync_packet_t sync;

    g_commandData.dataPhase.address = 0;
    g_commandData.dataPhase.count = 0;

    if (status == kStatus_Success)
    {
        // receiving framing paket header
        if (peripheral_read(rxBuf, sizeof(framing_data_packet_t)) != kStatus_Success)
        {
            status = kStatus_Fail;
        }
        else
        {
            rxSize = ((framing_data_packet_t *)rxBuf)->length;
            if (peripheral_read(rxBuf + sizeof(framing_data_packet_t), rxSize) != kStatus_Success)
            {
                status = kStatus_Fail;
            }
        }

        sync.header.startByte = kFramingPacketStartByte;

        if (status == kStatus_Fail)
        {
            sync.header.packetType = kFramingPacketType_Nak;
        }
        else
        {
            sync.header.packetType = kFramingPacketType_Ack;
        }

        // send Ack/Nak back to peripheral
        peripheral_write((uint8_t *)&sync, sizeof(framing_sync_packet_t));
    }

    // Send final response packet.
    send_generic_response(status, g_commandData.dataPhase.commandTag);
}

static status_t handle_data_read(bool *hasMoreData)
{
    if (g_commandData.dataPhase.count == 0)
    {
        // No data phase.
        *hasMoreData = false;
        finalize_data_phase(kStatus_Success);
        return kStatus_Success;
    }

    *hasMoreData = true;
    uint32_t remaining = g_commandData.dataPhase.count;
    uint32_t dataAddress = g_commandData.dataPhase.address;
    status_t status = kStatus_Success;
    framing_sync_packet_t sync;

    // Initialize the data packet to send.
    uint32_t packetSize;
    uint8_t packet[kMinPacketBufferSize];

    // Copy the data into the data packet.
    packetSize = MIN(kMinPacketBufferSize, remaining);

    // receiving framing paket header
    if (peripheral_read(packet, sizeof(framing_data_packet_t)) != kStatus_Success)
    {
        status = kStatus_Fail;
    }
    else
    {
        if (peripheral_read(packet, packetSize) != kStatus_Success)
        {
            status = kStatus_Fail;
        }
    }

    sync.header.startByte = kFramingPacketStartByte;

    if (status == kStatus_Fail)
    {
        sync.header.packetType = kFramingPacketType_Nak;
    }
    else
    {
        sync.header.packetType = kFramingPacketType_Ack;
    }

    // send Ack/Nak back to peripheral
    peripheral_write((uint8_t *)&sync, sizeof(framing_sync_packet_t));

    dataAddress += packetSize;

    if (status != kStatus_Success)
    {
        debug_printf(
            "Error: %s returned status 0x%x, abort data phase\r\n",
            (g_commandData.dataPhase.commandTag == kCommandTag_ReadMemory) ? "read memory" : "flash read resource",
            status);
        // Send zero length packet to tell host we are aborting data phase
        usb_hid_packet_write(&g_peripherals[0], (const uint8_t *)packet, 0, kPacketType_Data);
        finalize_data_phase(status);
        *hasMoreData = false;
        return kStatus_Success;
    }

    remaining -= packetSize;

    status = usb_hid_packet_write(&g_peripherals[0], (const uint8_t *)packet, packetSize, kPacketType_Data);

    if (remaining == 0)
    {
        finalize_data_phase(status);
        *hasMoreData = false;
    }
    else if (status != kStatus_Success)
    {
        debug_printf("writePacket aborted due to status 0x%x\r\n", status);
        finalize_data_phase(status);
        *hasMoreData = false;
    }
    else
    {
        g_commandData.dataPhase.count = remaining;
        g_commandData.dataPhase.address = dataAddress;
    }

    return kStatus_Success;
}

static status_t handle_data_write(bool *hasMoreData)
{
    serial_framing_packet_t framingPacket;

    if (g_commandData.dataPhase.count == 0)
    {
        // No data phase.
        *hasMoreData = false;
        finalize_data_phase(kStatus_Success);
        return kStatus_Success;
    }

    *hasMoreData = true;
    uint32_t remaining = g_commandData.dataPhase.count;
    uint32_t dataAddress = g_commandData.dataPhase.address;
    uint8_t *packet;
    uint32_t packetLength = 0;
    status_t status;

    framing_sync_packet_t sync;

    // read data packet
    status = usb_hid_packet_read(&g_peripherals[0], &packet, &packetLength, kPacketType_Data);
    if (status != kStatus_Success)
    {
        // Abort data phase due to error.
        debug_printf("consumer abort data phase due to status 0x%x\r\n", status);
        //        g_packetInterface->abortDataPhase(0);
        finalize_data_phase(status);
        *hasMoreData = false;
        return kStatus_Success;
    }

    if (packetLength == 0)
    {
        // Sender requested data phase abort.
        debug_printf("Data phase aborted by sender\r\n");
        //        finalize_data_phase(kStatus_AbortDataPhase);
        finalize_data_phase(kStatus_Success);
        *hasMoreData = false;
        return kStatus_Success;
    }

    packetLength = MIN(packetLength, remaining);

    // Convert to framing packet
    framingPacket.dataPacket.header.startByte = kFramingPacketStartByte;
    framingPacket.dataPacket.header.packetType = kFramingPacketType_Data;

    framingPacket.dataPacket.length = packetLength;

    // Copy the caller's data buffer into the framing packet.
    if (packetLength)
    {
        memcpy(framingPacket.data, packet, packetLength);
    }

    framingPacket.dataPacket.crc16 = calculate_framing_crc16(&framingPacket.dataPacket, (uint8_t *)framingPacket.data);

    // send framing packet to target peripheral
    if (peripheral_write((uint8_t *)&framingPacket, sizeof(framing_data_packet_t) + framingPacket.dataPacket.length) !=
        kStatus_Success)
    {
        status = kStatus_Fail;
    }
    // recering ACK from target peripheral
    else
    {
        uint8_t count = 32;
        do
        {
            if (peripheral_read((uint8_t *)&sync.header, 2) != kStatus_Success)
            {
                status = kStatus_Fail;
            }
            else
            {
                if ((sync.header.startByte != kFramingPacketStartByte) &&
                    (sync.header.packetType != kFramingPacketType_Ack))
                {
                    status = kStatus_Fail;
                }
                break;
            }
        } while ((sync.header.startByte != kFramingPacketStartByte) && (count-- != 0)); //
    }

    dataAddress += packetLength;
    remaining -= packetLength;

    if (remaining == 0)
    {
        finalize_data_phase(status);
        *hasMoreData = false;
    }
    else if (status != kStatus_Success)
    {
        debug_printf("writePacket aborted due to status 0x%x\r\n", status);
        finalize_data_phase(status);
        *hasMoreData = false;
    }
    else
    {
        g_commandData.dataPhase.count = remaining;
        g_commandData.dataPhase.address = dataAddress;
    }

    return kStatus_Success;
}

// See command.h for documentation on this function.
status_t bootloader_command_init()
{
    command_processor_data_t *data = g_commandInterface.stateData;

    data->state = kCommandState_CommandPhase;
    return kStatus_Success;
}

// See command.h for documentation on this function.
status_t bootloader_command_pump()
{
    status_t status = kStatus_Success;
    bool hasMoreData = false;
    command_packet_t *cmdPacket;
    uint8_t cmdTag;

    switch (g_commandData.state)
    {
        default:
        case kCommandState_CommandPhase:

            status = usb_hid_packet_read(&g_peripherals[0], &g_commandData.packet, &g_commandData.packetLength,
                                         kPacketType_Command);

            if (status != kStatus_Success)
            {
                debug_printf("Error: readPacket returned status 0x%x\r\n", status);
                break;
            }

            if (g_commandData.packetLength == 0)
            {
                // No command packet is available. Return success.
                break;
            }

            cmdPacket = (command_packet_t *)g_commandData.packet;
            cmdTag = cmdPacket->commandTag;

            // check command tag to see if there will be a dataphase
            if (cmdTag == kCommandTag_ConfigureI2c)
            {
                handle_config_i2c(g_commandData.packet, g_commandData.packetLength);
                break;
            }
            // check command tag to see if there will be a dataphase
            else if (cmdTag == kCommandTag_ConfigureSpi)
            {
                handle_config_spi(g_commandData.packet, g_commandData.packetLength);
                break;
            }
            // check command tag to see if there will be a dataphase
            else if (cmdTag == kCommandTag_ConfigureCan)
            {
                handle_config_can(g_commandData.packet, g_commandData.packetLength);
                break;
            }

            else if (cmdTag == kCommandTag_ReadMemory)
            {
                handle_read_memory_command(g_commandData.packet, g_commandData.packetLength);

                g_commandData.state = kCommandState_DataPhaseRead;
            }
            else if (cmdTag == kCommandTag_FlashReadResource)
            {
                g_commandData.state = kCommandState_DataPhaseRead;
            }
            else if (cmdTag == kCommandTag_ReceiveSbFile || cmdTag == kCommandTag_WriteMemory)
            {
                handle_write_memory_command(g_commandData.packet, g_commandData.packetLength);

                g_commandData.state = kCommandState_DataPhaseWrite;
            }

            status = handle_command_internal(g_commandData.packet, g_commandData.packetLength);
            if (status != kStatus_Success)
            {
                debug_printf("Error: handle_command returned status 0x%x\r\n", status);
                break;
            }

            break;

        case kCommandState_DataPhaseRead:
            status = handle_data_read(&hasMoreData);
            if (status != kStatus_Success)
            {
                g_commandData.state = kCommandState_CommandPhase;
                break;
            }
            g_commandData.state = hasMoreData ? kCommandState_DataPhaseRead : kCommandState_CommandPhase;
            break;

        case kCommandState_DataPhaseWrite:
            status = handle_data_write(&hasMoreData);
            if (status != kStatus_Success)
            {
                g_commandData.state = kCommandState_CommandPhase;
                break;
            }
            g_commandData.state = hasMoreData ? kCommandState_DataPhaseWrite : kCommandState_CommandPhase;
            break;

        case kCommandState_DataPhase:
            status = handle_data(&hasMoreData);
            if (status != kStatus_Success)
            {
                g_commandData.state = kCommandState_CommandPhase;
                break;
            }
            g_commandData.state = hasMoreData ? kCommandState_DataPhase : kCommandState_CommandPhase;
            break;
    }

    return status;
}

//! @brief Handle a command transaction.
static status_t handle_command_internal(uint8_t *packet, uint32_t packetLength)
{
    serial_framing_packet_t framingPacket;
    status_t status = kStatus_Success;

    uint8_t *rxBuf = (uint8_t *)g_targetRxBuffer;
    uint8_t rxSize = 0;

    framing_sync_packet_t sync;

    // Convert to framing packet
    framingPacket.dataPacket.header.startByte = kFramingPacketStartByte;
    framingPacket.dataPacket.header.packetType = kFramingPacketType_Command;
    // Only read, write memory, receive SB file and flash read resource will have data command type
    // Will add later
    framingPacket.dataPacket.length = packetLength;

    // Copy the caller's data buffer into the framing packet.
    if (packetLength)
    {
        memcpy(framingPacket.data, packet, packetLength);
    }

    framingPacket.dataPacket.crc16 = calculate_framing_crc16(&framingPacket.dataPacket, (uint8_t *)framingPacket.data);

    // send framing packet to target peripheral
    if (peripheral_write((uint8_t *)&framingPacket, sizeof(framing_data_packet_t) + framingPacket.dataPacket.length) !=
        kStatus_Success)
    {
        status = kStatus_Fail;
    }
    else // recering ACK from target peripheral
    {
        uint8_t count = 32;
        do
        {
            if (peripheral_read((uint8_t *)&sync.header.startByte, 1) != kStatus_Success)
            {
                status = kStatus_Fail;
                break;
            }
            else
            {
                if (sync.header.startByte == kFramingPacketStartByte)
                {
                    if (peripheral_read((uint8_t *)&sync.header.packetType, 1) != kStatus_Success)
                    {
                        status = kStatus_Fail;
                    }
                    else if (sync.header.packetType != kFramingPacketType_Ack)
                    {
                        status = kStatus_Fail;
                    }
                    else
                    {
                        // receiving framing paket header
                        if (peripheral_read(rxBuf, sizeof(framing_data_packet_t)) != kStatus_Success)
                        {
                            status = kStatus_Fail;
                        }
                        else
                        {
                            rxSize = ((framing_data_packet_t *)rxBuf)->length;
                            if (peripheral_read(rxBuf + sizeof(framing_data_packet_t), rxSize) != kStatus_Success)
                            {
                                status = kStatus_Fail;
                                sync.header.packetType = kFramingPacketType_Nak;
                            }
                            else
                            {
                                sync.header.packetType = kFramingPacketType_Ack;
                            }

                            // send Ack/Nak back to peripheral
                            peripheral_write((uint8_t *)&sync, sizeof(framing_sync_packet_t));
                        }
                    }
                    break;
                }
            }
        } while (count-- != 0); // really good??
    }

    if (status == kStatus_Success)
    {
        status =
            usb_hid_packet_write(&g_peripherals[0], rxBuf + sizeof(framing_data_packet_t), rxSize, kPacketType_Command);
    }

    return status;
}

//! @brief Handle a data transaction.
static status_t handle_data(bool *hasMoreData)
{
    if (g_commandData.handlerEntry)
    {
        // Run data phase if present, otherwise just return success.
        *hasMoreData = 0;
        return g_commandData.handlerEntry->handleData ? g_commandData.handlerEntry->handleData(hasMoreData) :
                                                        kStatus_Success;
    }
    return kStatus_Success;
}

//! @brief Reset command handler.
void handle_reset(uint8_t *packet, uint32_t packetLength)
{
    command_packet_t *commandPacket = (command_packet_t *)packet;
    send_generic_response(kStatus_Success, commandPacket->commandTag);

    // reset Bus Pal (TBD)
}

//! @brief Reset data phase variables.
static void reset_data_phase()
{
    memset(&g_commandData.dataPhase, 0, sizeof(g_commandData.dataPhase));
}

//! @brief Send a generic response packet.
void send_generic_response(uint32_t commandStatus, uint32_t commandTag)
{
    generic_response_packet_t responsePacket;
    responsePacket.commandPacket.commandTag = kCommandTag_GenericResponse;
    responsePacket.commandPacket.flags = 0;
    responsePacket.commandPacket.reserved = 0;
    responsePacket.commandPacket.parameterCount = 2;
    responsePacket.status = commandStatus;
    responsePacket.commandTag = commandTag;

    status_t status = usb_hid_packet_write(&g_peripherals[0], (const uint8_t *)&responsePacket, sizeof(responsePacket),
                                           kPacketType_Command);
    if (status != kStatus_Success)
    {
        debug_printf("Error: writePacket returned status 0x%x\r\n", status);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
