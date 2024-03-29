/*!
 * @file      main_lorawan_asynchronous.h
 *
 * @brief     LoRa Basics Modem device handling asynchronous events, application configuration
 *
 * @copyright
 * @parblock
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MAIN_LORAWAN_ASYNCHRONOUS_H
#define MAIN_LORAWAN_ASYNCHRONOUS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief LoRaWAN application port
 */
#define LORAWAN_APP_PORT_DEFAULT 2

/*!
 * @brief User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE_DEFAULT 242

/*!
 * @brief Time during which a LED is turned on when a TX or RX event occurs, in [ms]
 */
#define LED_PERIOD_MS_DEFAULT 250

/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON_DEFAULT false

/*!
 * @brief Default datarate
 *
 * @remark See @ref smtc_modem_adr_profile_t
 */
#define LORAWAN_DEFAULT_DATARATE_DEFAULT SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED

#ifndef LED_PERIOD_MS
#define LED_PERIOD_MS LED_PERIOD_MS_DEFAULT
#endif  // LED_PERIOD_MS

#ifndef LORAWAN_APP_PORT
#define LORAWAN_APP_PORT LORAWAN_APP_PORT_DEFAULT
#endif  // LORAWAN_APP_PORT

#ifndef LORAWAN_APP_DATA_MAX_SIZE
#define LORAWAN_APP_DATA_MAX_SIZE LORAWAN_APP_DATA_MAX_SIZE_DEFAULT
#endif  // LORAWAN_APP_DATA_MAX_SIZE

#ifndef LORAWAN_CONFIRMED_MSG_ON
#define LORAWAN_CONFIRMED_MSG_ON LORAWAN_CONFIRMED_MSG_ON_DEFAULT
#endif  // LORAWAN_CONFIRMED_MSG_ON

#ifndef LORAWAN_DEFAULT_DATARATE
#define LORAWAN_DEFAULT_DATARATE LORAWAN_DEFAULT_DATARATE_DEFAULT
#endif  // LORAWAN_DEFAULT_DATARATE

/*!
 * @brief ADR custom list when LORAWAN_DEFAULT_DATARATE is set to SMTC_MODEM_ADR_PROFILE_CUSTOM
 */
uint8_t adr_custom_list[16] = { 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x03, 0x03,
                                0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00 };

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MAIN_LORAWAN_ASYNCHRONOUS_H

/* --- EOF ------------------------------------------------------------------ */
