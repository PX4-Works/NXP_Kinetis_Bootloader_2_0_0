/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
#if !defined(__BUS_PAL_HARDWARE_H__)
#define __BUS_PAL_HARDWARE_H__

#include "dspi/fsl_dspi.h"
#include "i2c/fsl_i2c.h"
#include "bl_peripheral.h"

/*!
 * @brief user config from host for i2c
 */
typedef struct _i2c_user_config
{
    uint8_t slaveAddress;
    uint16_t baudRate_kbps;
} i2c_user_config_t;

/*!
 * @brief user config from host for spi
 */
typedef struct _dspi_user_config
{
    dspi_clock_polarity_t polarity;   /*!< Clock polarity */
    dspi_clock_phase_t phase;         /*!< Clock phase */
    dspi_shift_direction_t direction; /*!< MSB or LSB */
    uint32_t baudRate_Bps;            /*!< Baud Rate for SPI in Hz */
    uint32_t clock_Hz;
} dspi_user_config_t;

/*!
 * @brief hardware initialization
 */
void init_hardware(void);

//! @brief sending host bytes command process
void write_bytes_to_host(uint8_t *src, uint32_t length);

//! @brief receiving host start command process
void host_start_command_rx(uint8_t *dest, uint32_t length);

//! @brief receiving host stop command process
void host_stop_command_rx(void);

//! @brief receiving host get bytes command process
uint32_t get_bytes_received_from_host(void);

//! @brief spi config speed process
void configure_spi_speed(uint32_t speedkhz);

//! @brief spi config settings process
void configure_spi_settings(dspi_clock_polarity_t polarity, dspi_clock_phase_t phase, dspi_shift_direction_t direction);

//! @brief i2c config address process
void configure_i2c_address(uint8_t address);

//! @brief i2c config speed process
void configure_i2c_speed(uint32_t speedkhz);

//! @brief spi send data process
void send_spi_data(uint8_t *src, uint32_t writeLength);

//! @brief spi receiving data process
void receive_spi_data(uint8_t *dest, uint32_t readLength);

//! @brief flexCAN config speed process
void configure_can_speed(uint32_t speed);

//! @brief flexCAN config txid
void configure_can_txid(uint32_t txid);

//! @brief flexCAN config rxid
void configure_can_rxid(uint32_t rxid);

//! @brief flexCAN sending data process
void send_can_data(uint8_t *src, uint32_t writeLength);

//! @brief flexCAN receiving data process
void receive_can_data(uint8_t data, uint32_t instance);

//! @brief flexCAN read data process
void read_can_data(uint8_t *dest, uint32_t readLength);

//! @brief flexCAN reset buffer process
void reset_can_buffer(void);

//! @brief i2c sending data process
status_t send_i2c_data(uint8_t *src, uint32_t writeLength);

//! @brief i2c receiving data process
status_t receive_i2c_data(uint8_t *dest, uint32_t readLength);

//! @brief GPIO config processing
void configure_gpio(uint8_t port, uint8_t pinNum, uint8_t muxVal);

//! @brief GPIO set up function
void set_gpio(uint8_t port, uint8_t pinNum, uint8_t level);

//! @brief fpga clock set function
void set_fpga_clock(uint32_t clock);

bool scuart_poll_for_activity();
bool usb_hid_poll_for_activity(const peripheral_descriptor_t *self);
status_t usb_hid_packet_init(const peripheral_descriptor_t *self);
status_t usb_hid_packet_read(const peripheral_descriptor_t *self,
                             uint8_t **packet,
                             uint32_t *packetLength,
                             packet_type_t packetType);
status_t usb_hid_packet_write(const peripheral_descriptor_t *self,
                              const uint8_t *packet,
                              uint32_t byteCount,
                              packet_type_t packetType);
#endif // __BUS_PAL_HARDWARE_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
