/***************************************************************************//**
 * @file
 * @brief SPIDRV_USART Config
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef SL_SPIDRV_USART_SPI_SX126X_CONFIG_H
#define SL_SPIDRV_USART_SPI_SX126X_CONFIG_H

#include "spidrv.h"

// <<< Use Configuration Wizard in Context Menu >>>
// <h> SPIDRV settings

// <o SL_SPIDRV_USART_SPI_SX126X_BITRATE> SPI bitrate
// <i> Default: 1000000
#define SL_SPIDRV_USART_SPI_SX126X_BITRATE           1000000

// <o SL_SPIDRV_USART_SPI_SX126X_FRAME_LENGTH> SPI frame length <4-16>
// <i> Default: 8
#define SL_SPIDRV_USART_SPI_SX126X_FRAME_LENGTH      8

// <o SL_SPIDRV_USART_SPI_SX126X_TYPE> SPI mode
// <spidrvMaster=> Master
// <spidrvSlave=> Slave
#define SL_SPIDRV_USART_SPI_SX126X_TYPE              spidrvMaster

// <o SL_SPIDRV_USART_SPI_SX126X_BIT_ORDER> Bit order on the SPI bus
// <spidrvBitOrderLsbFirst=> LSB transmitted first
// <spidrvBitOrderMsbFirst=> MSB transmitted first
#define SL_SPIDRV_USART_SPI_SX126X_BIT_ORDER         spidrvBitOrderMsbFirst

// <o SL_SPIDRV_USART_SPI_SX126X_CLOCK_MODE> SPI clock mode
// <spidrvClockMode0=> SPI mode 0: CLKPOL=0, CLKPHA=0
// <spidrvClockMode1=> SPI mode 1: CLKPOL=0, CLKPHA=1
// <spidrvClockMode2=> SPI mode 2: CLKPOL=1, CLKPHA=0
// <spidrvClockMode3=> SPI mode 3: CLKPOL=1, CLKPHA=1
#define SL_SPIDRV_USART_SPI_SX126X_CLOCK_MODE        spidrvClockMode0

// <o SL_SPIDRV_USART_SPI_SX126X_CS_CONTROL> SPI master chip select (CS) control scheme.
// <spidrvCsControlAuto=> CS controlled by the SPI driver
// <spidrvCsControlApplication=> CS controlled by the application
#define SL_SPIDRV_USART_SPI_SX126X_CS_CONTROL        spidrvCsControlApplication

// <o SL_SPIDRV_USART_SPI_SX126X_SLAVE_START_MODE> SPI slave transfer start scheme
// <spidrvSlaveStartImmediate=> Transfer starts immediately
// <spidrvSlaveStartDelayed=> Transfer starts when the bus is idle (CS deasserted)
// <i> Only applies if instance type is spidrvSlave
#define SL_SPIDRV_USART_SPI_SX126X_SLAVE_START_MODE  spidrvSlaveStartImmediate
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <usart signal=TX,RX,CLK,(CS)> SL_SPIDRV_USART_SPI_SX126X
// $[USART_SL_SPIDRV_USART_SPI_SX126X]
#define SL_SPIDRV_USART_SPI_SX126X_PERIPHERAL    USART1
#define SL_SPIDRV_USART_SPI_SX126X_PERIPHERAL_NO 1

// USART1 TX on PA04
#define SL_SPIDRV_USART_SPI_SX126X_TX_PORT       gpioPortA
#define SL_SPIDRV_USART_SPI_SX126X_TX_PIN        4

// USART1 RX on PA08
#define SL_SPIDRV_USART_SPI_SX126X_RX_PORT       gpioPortA
#define SL_SPIDRV_USART_SPI_SX126X_RX_PIN        8

// USART1 CLK on PA00
#define SL_SPIDRV_USART_SPI_SX126X_CLK_PORT      gpioPortA
#define SL_SPIDRV_USART_SPI_SX126X_CLK_PIN       0

// [USART_SL_SPIDRV_USART_SPI_SX126X]$
// <<< sl:end pin_tool >>>

#endif // SL_SPIDRV_USART_SPI_SX126X_CONFIG_H
