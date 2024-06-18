/*****************************************************************************
  Filename:       modem_pinout.h
  Revised:        12/12/2023
  Revision:       1.0

  Description: RM126x Radio chip pin definitions

  Copyright 2023 Ezurio. All rights reserved.

  The information contained herein is property of Ezurio.
  Terms and conditions of usage are described in detail in EZURIO STANDARD
  SOFTWARE LICENSE AGREEMENT.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  EZURIO OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE,
  STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO
  ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST
  PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
  SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY
  DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Ezurio at www.ezurio.com.
*******************************************************************************/

#ifndef MODEM_PINOUT_H_
#define MODEM_PINOUT_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "board_options.h"

#define SMTC_RADIO_SPI_MOSI PA_4
#define SMTC_RADIO_SPI_MISO PA_8
#define SMTC_RADIO_SPI_SCLK PA_0
#define SMTC_RADIO_NSS      PB_0
#define SMTC_RADIO_NRST     PA_6
#define SMTC_RADIO_DIOX     PA_5
#define SMTC_RADIO_BUSY     PA_7
#define SMTC_ANT_SW         PB_1
#define SMTC_LED_TX         NC
#define SMTC_LED_RX         NC

#endif /* MODEM_PINOUT_H_ */
