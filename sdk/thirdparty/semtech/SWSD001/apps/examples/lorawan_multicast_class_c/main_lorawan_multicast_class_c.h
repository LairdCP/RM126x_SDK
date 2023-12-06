/*!
 * @file      main_lorawan_multicast_class_c.h
 *
 * @brief     LoRa Basics Modem Class C device with multicast
 *
 * @copyright
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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

#ifndef MAIN_LORAWAN_MULTICAST_CLASS_C_H
#define MAIN_LORAWAN_MULTICAST_CLASS_C_H

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
 * @brief Defines the application data transmission duty cycle. 60s, value in [s].
 */
#define APP_TX_DUTYCYCLE_DEFAULT 60

/*!
 * @brief LoRaWAN application port
 */
#define LORAWAN_APP_PORT_DEFAULT 2

/*!
 * @brief User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE_DEFAULT 242

/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief Multicast group number
 */
#define MULTICAST_GROUP_DEFAULT ( 0 )

/*!
 * @brief Multicast group address
 */
#define MULTICAST_GROUP_ADDRESS_DEFAULT ( 0x00000000 )

/*!
 * @brief Multicast session frequency
 */
#define MULTICAST_SESSION_FREQUENCY_DEFAULT ( 869525000 )

/*!
 * @brief Multicast session datarate
 */
#define MULTICAST_SESSION_DATARATE_DEFAULT ( 4 )

/*!
 * @brief Multicast network session key
 */
#define MULTICAST_NWK_SESSION_KEY_DEFAULT \
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/*!
 * @brief Multicast application session key
 */
#define MULTICAST_APP_SESSION_KEY_DEFAULT \
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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

#ifndef APP_TX_DUTYCYCLE
#define APP_TX_DUTYCYCLE APP_TX_DUTYCYCLE_DEFAULT
#endif  // APP_TX_DUTYCYCLE

#ifndef LORAWAN_APP_PORT
#define LORAWAN_APP_PORT LORAWAN_APP_PORT_DEFAULT
#endif  // LORAWAN_APP_PORT

#ifndef LORAWAN_APP_DATA_MAX_SIZE
#define LORAWAN_APP_DATA_MAX_SIZE LORAWAN_APP_DATA_MAX_SIZE_DEFAULT
#endif  // LORAWAN_APP_DATA_MAX_SIZE

#ifndef MULTICAST_GROUP
#define MULTICAST_GROUP MULTICAST_GROUP_DEFAULT
#endif  // MULTICAST_GROUP

#ifndef MULTICAST_GROUP_ADDRESS
#define MULTICAST_GROUP_ADDRESS MULTICAST_GROUP_ADDRESS_DEFAULT
#endif  // MULTICAST_GROUP_ADDRESS

#ifndef MULTICAST_SESSION_FREQUENCY
#define MULTICAST_SESSION_FREQUENCY MULTICAST_SESSION_FREQUENCY_DEFAULT
#endif  // MULTICAST_SESSION_FREQUENCY

#ifndef MULTICAST_SESSION_DATARATE
#define MULTICAST_SESSION_DATARATE MULTICAST_SESSION_DATARATE_DEFAULT
#endif  // MULTICAST_SESSION_DATARATE

#ifndef MULTICAST_NWK_SESSION_KEY
#define MULTICAST_NWK_SESSION_KEY MULTICAST_NWK_SESSION_KEY_DEFAULT
#endif  // MULTICAST_NWK_SESSION_KEY

#ifndef MULTICAST_APP_SESSION_KEY
#define MULTICAST_APP_SESSION_KEY MULTICAST_APP_SESSION_KEY_DEFAULT
#endif  // MULTICAST_APP_SESSION_KEY

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

#endif  // MAIN_LORAWAN_MULTICAST_CLASS_C_H

/* --- EOF ------------------------------------------------------------------ */
