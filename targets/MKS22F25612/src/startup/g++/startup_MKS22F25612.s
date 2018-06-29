/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MKS22F25612.s                                                       */
/*  @purpose: CMSIS Cortex-M4 Core Device Startup File                                    */
/*            MKS22F25612                                                                 */
/*  @version: 1.0                                                                         */
/*  @date:    2015-6-23                                                                   */
/*  @build:   b150820                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright (c) 1997 - 2015 , Freescale Semiconductor, Inc.                              */
/* All rights reserved.                                                                   */
/*                                                                                        */
/* Redistribution and use in source and binary forms, with or without modification,       */
/* are permitted provided that the following conditions are met:                          */
/*                                                                                        */
/* o Redistributions of source code must retain the above copyright notice, this list     */
/*   of conditions and the following disclaimer.                                          */
/*                                                                                        */
/* o Redistributions in binary form must reproduce the above copyright notice, this       */
/*   list of conditions and the following disclaimer in the documentation and/or          */
/*   other materials provided with the distribution.                                      */
/*                                                                                        */
/* o Neither the name of Freescale Semiconductor, Inc. nor the names of its               */
/*   contributors may be used to endorse or promote products derived from this            */
/*   software without specific prior written permission.                                  */
/*                                                                                        */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND        */
/* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED          */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                 */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES         */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;           */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON         */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS          */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/*****************************************************************************/
/* Version: CodeSourcery Sourcery G++ Lite (with CS3)                        */
/*****************************************************************************/


/* Vector Table */

    .section ".cs3.interrupt_vector"
    .globl  __cs3_interrupt_vector_cortex_m
    .type   __cs3_interrupt_vector_cortex_m, %object

__cs3_interrupt_vector_cortex_m:
    .long   __cs3_stack                 /* Top of Stack                 */
    .long   __cs3_reset                 /* Reset Handler                */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_IRQHandler                                 /* DMA Channel 0 Transfer Complete*/
    .long   DMA1_IRQHandler                                 /* DMA Channel 1 Transfer Complete*/
    .long   DMA2_IRQHandler                                 /* DMA Channel 2 Transfer Complete*/
    .long   DMA3_IRQHandler                                 /* DMA Channel 3 Transfer Complete*/
    .long   DMA4_IRQHandler                                 /* DMA Channel 4 Transfer Complete*/
    .long   DMA5_IRQHandler                                 /* DMA Channel 5 Transfer Complete*/
    .long   DMA6_IRQHandler                                 /* DMA Channel 6 Transfer Complete*/
    .long   DMA7_IRQHandler                                 /* DMA Channel 7 Transfer Complete*/
    .long   DMA8_IRQHandler                                 /* DMA Channel 8 Transfer Complete*/
    .long   DMA9_IRQHandler                                 /* DMA Channel 9 Transfer Complete*/
    .long   DMA10_IRQHandler                                /* DMA Channel 10 Transfer Complete*/
    .long   DMA11_IRQHandler                                /* DMA Channel 11 Transfer Complete*/
    .long   DMA12_IRQHandler                                /* DMA Channel 12 Transfer Complete*/
    .long   DMA13_IRQHandler                                /* DMA Channel 13 Transfer Complete*/
    .long   DMA14_IRQHandler                                /* DMA Channel 14 Transfer Complete*/
    .long   DMA15_IRQHandler                                /* DMA Channel 15 Transfer Complete*/
    .long   DMA_Error_IRQHandler                            /* DMA Error Interrupt*/
    .long   MCM_IRQHandler                                  /* Normal Interrupt*/
    .long   FTF_IRQHandler                                  /* FTFA Command complete interrupt*/
    .long   Read_Collision_IRQHandler                       /* Read Collision Interrupt*/
    .long   LVD_LVW_IRQHandler                              /* Low Voltage Detect, Low Voltage Warning*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup Unit*/
    .long   WDOG_EWM_IRQHandler                             /* WDOG Interrupt*/
    .long   RNG_IRQHandler                                  /* RNG Interrupt*/
    .long   LPI2C0_IRQHandler                               /* LPI2C0 interrupt*/
    .long   LPI2C1_IRQHandler                               /* LPI2C1 interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 Interrupt*/
    .long   SPI1_IRQHandler                                 /* SPI1 Interrupt*/
    .long   I2S0_Tx_IRQHandler                              /* I2S0 transmit interrupt*/
    .long   I2S0_Rx_IRQHandler                              /* I2S0 receive interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status/error interrupt*/
    .long   UART0_RX_TX_IRQHandler                          /* UART0 Status Sources interrupt*/
    .long   UART0_ERR_IRQHandler                            /* UART0 Error Sources interrupt*/
    .long   UART1_RX_TX_IRQHandler                          /* UART1 Status Sources interrupt*/
    .long   UART1_ERR_IRQHandler                            /* UART1 Error Sources interrupt*/
    .long   UART2_RX_TX_IRQHandler                          /* UART2 Status Sources interrupt*/
    .long   UART2_ERR_IRQHandler                            /* UART2 Error Sources interrupt*/
    .long   Reserved53_IRQHandler                           /* Reserved interrupt 53*/
    .long   Reserved54_IRQHandler                           /* Reserved interrupt 54*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   Reserved57_IRQHandler                           /* Reserved interrupt 57*/
    .long   TPM0_IRQHandler                                 /* TPM0 Timer Overflow and Channels interrupt*/
    .long   TPM1_IRQHandler                                 /* TPM1 Timer Overflow and Channels interrupt*/
    .long   TPM2_IRQHandler                                 /* TPM2 Timer Overflow and Channels interrupt*/
    .long   Reserved61_IRQHandler                           /* Reserved interrupt 61*/
    .long   RTC_IRQHandler                                  /* RTC alarm interrupt*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds interrupt*/
    .long   PIT0_IRQHandler                                 /* PIT timer channel 0 interrupt*/
    .long   PIT1_IRQHandler                                 /* PIT timer channel 1 interrupt*/
    .long   PIT2_IRQHandler                                 /* PIT timer channel 2 interrupt*/
    .long   PIT3_IRQHandler                                 /* PIT timer channel 3 interrupt*/
    .long   PDB0_IRQHandler                                 /* PDB0 Interrupt*/
    .long   USB0_IRQHandler                                 /* USB0 interrupt*/
    .long   Reserved70_IRQHandler                           /* Reserved interrupt 70*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt 71*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt*/
    .long   MCG_IRQHandler                                  /* MCG Interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTimer interrupt*/
    .long   PORTA_IRQHandler                                /* Port A interrupt*/
    .long   PORTB_IRQHandler                                /* Port B interrupt*/
    .long   PORTC_IRQHandler                                /* Port C interrupt*/
    .long   PORTD_IRQHandler                                /* Port D interrupt*/
    .long   PORTE_IRQHandler                                /* Port E interrupt*/
    .long   SWI_IRQHandler                                  /* Software interrupt*/
    .long   Reserved81_IRQHandler                           /* Reserved interrupt 81*/
    .long   Reserved82_IRQHandler                           /* Reserved interrupt 82*/
    .long   Reserved83_IRQHandler                           /* Reserved interrupt 83*/
    .long   Reserved84_IRQHandler                           /* Reserved interrupt 84*/
    .long   Reserved85_IRQHandler                           /* Reserved interrupt 85*/
    .long   FLEXIO_IRQHandler                               /* Flexible IO interrupt*/
    .long   Reserved87_IRQHandler                           /* Reserved interrupt 87*/
    .long   Reserved88_IRQHandler                           /* Reserved interrupt 88*/
    .long   Reserved89_IRQHandler                           /* Reserved interrupt 89*/
    .long   Reserved90_IRQHandler                           /* Reserved Interrupt 90*/
    .long   CAN0_ORed_Message_buffer_IRQHandler             /* CAN0 OR'd message buffers interrupt*/
    .long   CAN0_Bus_Off_IRQHandler                         /* CAN0 bus off interrupt*/
    .long   CAN0_Error_IRQHandler                           /* CAN0 error interrupt*/
    .long   CAN0_Tx_Warning_IRQHandler                      /* CAN0 Tx warning interrupt*/
    .long   CAN0_Rx_Warning_IRQHandler                      /* CAN0 Rx warning interrupt*/
    .long   CAN0_Wake_Up_IRQHandler                         /* CAN0 wake up interrupt*/
    .long   Reserved97_IRQHandler                           /* Reserved Interrupt 97*/
    .long   Reserved98_IRQHandler                           /* Reserved Interrupt 98*/
    .long   Reserved99_IRQHandler                           /* Reserved Interrupt 99*/
    .long   Reserved100_IRQHandler                          /* Reserved Interrupt 100*/
    .long   Reserved101_IRQHandler                          /* Reserved Interrupt 101*/
    .long   Reserved102_IRQHandler                          /* Reserved Interrupt 102*/
    .long   Reserved103_IRQHandler                          /* Reserved Interrupt 103*/
    .long   I2S1_Tx_IRQHandler                              /* I2S1 transmit interrupt*/
    .long   I2S1_Rx_IRQHandler                              /* I2S1 receive interrupt*/
    .long   Reserved106_IRQHandler                          /* Reserved Interrupt 106*/
    .long   Reserved107_IRQHandler                          /* Reserved Interrupt 107*/
    .long   Reserved108_IRQHandler                          /* Reserved Interrupt 108*/
    .long   Reserved109_IRQHandler                          /* Reserved Interrupt 109*/
    .long   CAN1_ORed_Message_buffer_IRQHandler             /* CAN0 OR'd message buffers interrupt*/
    .long   CAN1_Bus_Off_IRQHandler                         /* CAN1 bus off interrupt*/
    .long   CAN1_Error_IRQHandler                           /* CAN1 error interrupt*/
    .long   CAN1_Tx_Warning_IRQHandler                      /* CAN1 Tx warning interrupt*/
    .long   CAN1_Rx_Warning_IRQHandler                      /* CAN1 Rx warning interrupt*/
    .long   CAN1_Wake_Up_IRQHandler                         /* CAN1 wake up interrupt*/
    .long   DefaultISR                                      /* 116*/
    .long   DefaultISR                                      /* 117*/
    .long   DefaultISR                                      /* 118*/
    .long   DefaultISR                                      /* 119*/
    .long   DefaultISR                                      /* 120*/
    .long   DefaultISR                                      /* 121*/
    .long   DefaultISR                                      /* 122*/
    .long   DefaultISR                                      /* 123*/
    .long   DefaultISR                                      /* 124*/
    .long   DefaultISR                                      /* 125*/
    .long   DefaultISR                                      /* 126*/
    .long   DefaultISR                                      /* 127*/
    .long   DefaultISR                                      /* 128*/
    .long   DefaultISR                                      /* 129*/
    .long   DefaultISR                                      /* 130*/
    .long   DefaultISR                                      /* 131*/
    .long   DefaultISR                                      /* 132*/
    .long   DefaultISR                                      /* 133*/
    .long   DefaultISR                                      /* 134*/
    .long   DefaultISR                                      /* 135*/
    .long   DefaultISR                                      /* 136*/
    .long   DefaultISR                                      /* 137*/
    .long   DefaultISR                                      /* 138*/
    .long   DefaultISR                                      /* 139*/
    .long   DefaultISR                                      /* 140*/
    .long   DefaultISR                                      /* 141*/
    .long   DefaultISR                                      /* 142*/
    .long   DefaultISR                                      /* 143*/
    .long   DefaultISR                                      /* 144*/
    .long   DefaultISR                                      /* 145*/
    .long   DefaultISR                                      /* 146*/
    .long   DefaultISR                                      /* 147*/
    .long   DefaultISR                                      /* 148*/
    .long   DefaultISR                                      /* 149*/
    .long   DefaultISR                                      /* 150*/
    .long   DefaultISR                                      /* 151*/
    .long   DefaultISR                                      /* 152*/
    .long   DefaultISR                                      /* 153*/
    .long   DefaultISR                                      /* 154*/
    .long   DefaultISR                                      /* 155*/
    .long   DefaultISR                                      /* 156*/
    .long   DefaultISR                                      /* 157*/
    .long   DefaultISR                                      /* 158*/
    .long   DefaultISR                                      /* 159*/
    .long   DefaultISR                                      /* 160*/
    .long   DefaultISR                                      /* 161*/
    .long   DefaultISR                                      /* 162*/
    .long   DefaultISR                                      /* 163*/
    .long   DefaultISR                                      /* 164*/
    .long   DefaultISR                                      /* 165*/
    .long   DefaultISR                                      /* 166*/
    .long   DefaultISR                                      /* 167*/
    .long   DefaultISR                                      /* 168*/
    .long   DefaultISR                                      /* 169*/
    .long   DefaultISR                                      /* 170*/
    .long   DefaultISR                                      /* 171*/
    .long   DefaultISR                                      /* 172*/
    .long   DefaultISR                                      /* 173*/
    .long   DefaultISR                                      /* 174*/
    .long   DefaultISR                                      /* 175*/
    .long   DefaultISR                                      /* 176*/
    .long   DefaultISR                                      /* 177*/
    .long   DefaultISR                                      /* 178*/
    .long   DefaultISR                                      /* 179*/
    .long   DefaultISR                                      /* 180*/
    .long   DefaultISR                                      /* 181*/
    .long   DefaultISR                                      /* 182*/
    .long   DefaultISR                                      /* 183*/
    .long   DefaultISR                                      /* 184*/
    .long   DefaultISR                                      /* 185*/
    .long   DefaultISR                                      /* 186*/
    .long   DefaultISR                                      /* 187*/
    .long   DefaultISR                                      /* 188*/
    .long   DefaultISR                                      /* 189*/
    .long   DefaultISR                                      /* 190*/
    .long   DefaultISR                                      /* 191*/
    .long   DefaultISR                                      /* 192*/
    .long   DefaultISR                                      /* 193*/
    .long   DefaultISR                                      /* 194*/
    .long   DefaultISR                                      /* 195*/
    .long   DefaultISR                                      /* 196*/
    .long   DefaultISR                                      /* 197*/
    .long   DefaultISR                                      /* 198*/
    .long   DefaultISR                                      /* 199*/
    .long   DefaultISR                                      /* 200*/
    .long   DefaultISR                                      /* 201*/
    .long   DefaultISR                                      /* 202*/
    .long   DefaultISR                                      /* 203*/
    .long   DefaultISR                                      /* 204*/
    .long   DefaultISR                                      /* 205*/
    .long   DefaultISR                                      /* 206*/
    .long   DefaultISR                                      /* 207*/
    .long   DefaultISR                                      /* 208*/
    .long   DefaultISR                                      /* 209*/
    .long   DefaultISR                                      /* 210*/
    .long   DefaultISR                                      /* 211*/
    .long   DefaultISR                                      /* 212*/
    .long   DefaultISR                                      /* 213*/
    .long   DefaultISR                                      /* 214*/
    .long   DefaultISR                                      /* 215*/
    .long   DefaultISR                                      /* 216*/
    .long   DefaultISR                                      /* 217*/
    .long   DefaultISR                                      /* 218*/
    .long   DefaultISR                                      /* 219*/
    .long   DefaultISR                                      /* 220*/
    .long   DefaultISR                                      /* 221*/
    .long   DefaultISR                                      /* 222*/
    .long   DefaultISR                                      /* 223*/
    .long   DefaultISR                                      /* 224*/
    .long   DefaultISR                                      /* 225*/
    .long   DefaultISR                                      /* 226*/
    .long   DefaultISR                                      /* 227*/
    .long   DefaultISR                                      /* 228*/
    .long   DefaultISR                                      /* 229*/
    .long   DefaultISR                                      /* 230*/
    .long   DefaultISR                                      /* 231*/
    .long   DefaultISR                                      /* 232*/
    .long   DefaultISR                                      /* 233*/
    .long   DefaultISR                                      /* 234*/
    .long   DefaultISR                                      /* 235*/
    .long   DefaultISR                                      /* 236*/
    .long   DefaultISR                                      /* 237*/
    .long   DefaultISR                                      /* 238*/
    .long   DefaultISR                                      /* 239*/
    .long   DefaultISR                                      /* 240*/
    .long   DefaultISR                                      /* 241*/
    .long   DefaultISR                                      /* 242*/
    .long   DefaultISR                                      /* 243*/
    .long   DefaultISR                                      /* 244*/
    .long   DefaultISR                                      /* 245*/
    .long   DefaultISR                                      /* 246*/
    .long   DefaultISR                                      /* 247*/
    .long   DefaultISR                                      /* 248*/
    .long   DefaultISR                                      /* 249*/
    .long   DefaultISR                                      /* 250*/
    .long   DefaultISR                                      /* 251*/
    .long   DefaultISR                                      /* 252*/
    .long   DefaultISR                                      /* 253*/
    .long   DefaultISR                                      /* 254*/
    .long   0xFFFFFFFF                                      /*  Reserved for user TRIM value*/

    .size   __cs3_interrupt_vector_cortex_m, . - __cs3_interrupt_vector_cortex_m

/* Flash Configuration */
    .section .FlashConfig

    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

    .thumb

/* Reset Handler */

    .section .cs3.reset,"x",%progbits
    .thumb_func
    .globl  __cs3_reset_cortex_m
    .type   __cs3_reset_cortex_m, %function
__cs3_reset_cortex_m:
    .fnstart
    LDR     R0, =SystemInit
    BLX     R0
    LDR     R0, =init_data_bss
    BLX     R0
    LDR     R0,=_start
    BX      R0
    .pool
    .cantunwind
    .fnend
    .size   __cs3_reset_cortex_m,.-__cs3_reset_cortex_m

    .section ".text"
    .weak  DefaultISR
    .type  DefaultISR, %function
DefaultISR:
    B	DefaultISR
    .size DefaultISR, . - DefaultISR

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler	handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    DMA0_IRQHandler
    def_irq_handler    DMA1_IRQHandler
    def_irq_handler    DMA2_IRQHandler
    def_irq_handler    DMA3_IRQHandler
    def_irq_handler    DMA4_IRQHandler
    def_irq_handler    DMA5_IRQHandler
    def_irq_handler    DMA6_IRQHandler
    def_irq_handler    DMA7_IRQHandler
    def_irq_handler    DMA8_IRQHandler
    def_irq_handler    DMA9_IRQHandler
    def_irq_handler    DMA10_IRQHandler
    def_irq_handler    DMA11_IRQHandler
    def_irq_handler    DMA12_IRQHandler
    def_irq_handler    DMA13_IRQHandler
    def_irq_handler    DMA14_IRQHandler
    def_irq_handler    DMA15_IRQHandler
    def_irq_handler    DMA_Error_IRQHandler
    def_irq_handler    MCM_IRQHandler
    def_irq_handler    FTF_IRQHandler
    def_irq_handler    Read_Collision_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    RNG_IRQHandler
    def_irq_handler    LPI2C0_IRQHandler
    def_irq_handler    LPI2C1_IRQHandler
    def_irq_handler    SPI0_IRQHandler
    def_irq_handler    SPI1_IRQHandler
    def_irq_handler    I2S0_Tx_IRQHandler
    def_irq_handler    I2S0_Rx_IRQHandler
    def_irq_handler    LPUART0_IRQHandler
    def_irq_handler    UART0_RX_TX_IRQHandler
    def_irq_handler    UART0_ERR_IRQHandler
    def_irq_handler    UART1_RX_TX_IRQHandler
    def_irq_handler    UART1_ERR_IRQHandler
    def_irq_handler    UART2_RX_TX_IRQHandler
    def_irq_handler    UART2_ERR_IRQHandler
    def_irq_handler    Reserved53_IRQHandler
    def_irq_handler    Reserved54_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    Reserved57_IRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    Reserved61_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    PIT1_IRQHandler
    def_irq_handler    PIT2_IRQHandler
    def_irq_handler    PIT3_IRQHandler
    def_irq_handler    PDB0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    Reserved70_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    SWI_IRQHandler
    def_irq_handler    Reserved81_IRQHandler
    def_irq_handler    Reserved82_IRQHandler
    def_irq_handler    Reserved83_IRQHandler
    def_irq_handler    Reserved84_IRQHandler
    def_irq_handler    Reserved85_IRQHandler
    def_irq_handler    FLEXIO_IRQHandler
    def_irq_handler    Reserved87_IRQHandler
    def_irq_handler    Reserved88_IRQHandler
    def_irq_handler    Reserved89_IRQHandler
    def_irq_handler    Reserved90_IRQHandler
    def_irq_handler    CAN0_ORed_Message_buffer_IRQHandler
    def_irq_handler    CAN0_Bus_Off_IRQHandler
    def_irq_handler    CAN0_Error_IRQHandler
    def_irq_handler    CAN0_Tx_Warning_IRQHandler
    def_irq_handler    CAN0_Rx_Warning_IRQHandler
    def_irq_handler    CAN0_Wake_Up_IRQHandler
    def_irq_handler    Reserved97_IRQHandler
    def_irq_handler    Reserved98_IRQHandler
    def_irq_handler    Reserved99_IRQHandler
    def_irq_handler    Reserved100_IRQHandler
    def_irq_handler    Reserved101_IRQHandler
    def_irq_handler    Reserved102_IRQHandler
    def_irq_handler    Reserved103_IRQHandler
    def_irq_handler    I2S1_Tx_IRQHandler
    def_irq_handler    I2S1_Rx_IRQHandler
    def_irq_handler    Reserved106_IRQHandler
    def_irq_handler    Reserved107_IRQHandler
    def_irq_handler    Reserved108_IRQHandler
    def_irq_handler    Reserved109_IRQHandler
    def_irq_handler    CAN1_ORed_Message_buffer_IRQHandler
    def_irq_handler    CAN1_Bus_Off_IRQHandler
    def_irq_handler    CAN1_Error_IRQHandler
    def_irq_handler    CAN1_Tx_Warning_IRQHandler
    def_irq_handler    CAN1_Rx_Warning_IRQHandler
    def_irq_handler    CAN1_Wake_Up_IRQHandler
