/***************************************************************************//**
 * @file rm126x_config.h
 * @brief Configuration file for RM126x module common component code.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 * <b>Copyright 2024 Ezurio. www.ezurio.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef RM126X_CONFIG_H_
#define RM126X_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Configuration
// <o RM126X_APPLICATION_PORT> Application Port <1..223:1>
#define RM126X_APPLICATION_PORT               1

// <o RM126X_CLASS_C_UPDATE_RATE_MS> Class C Update Rate in ms <100..30000:1>
#define RM126X_CLASS_C_UPDATE_RATE_MS         1000

// <o RM126X_NETWORK_TYPE> Network Type
// <0=> Private
// <1=> Public
// <i> Default: 1
#define RM126X_NETWORK_TYPE                   1

// <o RM126X_PACKET_TYPE> Packet Type
// <0=> Unconfirmed
// <1=> Confirmed
// <i> Default: 1
#define RM126X_PACKET_TYPE                    1

// <o RM126X_PERIODIC_UPDATE_RATE_S> Periodic Update Rate in s <5..180:1>
#define RM126X_PERIODIC_UPDATE_RATE_S         30

// <o RM126X_PING_SLOT_PERIODICITY> Ping Slot Periodicity
// <0=> Ping slot every 1s
// <1=> Ping slot every 2s
// <2=> Ping slot every 4s
// <3=> Ping slot every 8s
// <4=> Ping slot every 16s
// <5=> Ping slot every 32s
// <6=> Ping slot every 64s
// <7=> Ping slot every 128s
// <i> Default: 3
#define RM126X_PING_SLOT_PERIODICITY          3

// </h>

// <h> AppKey
// <o RM126X_APP_KEY_BYTES_15_TO_12> Bytes 15 to 12 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_APP_KEY_BYTES_15_TO_12         0x0

// <o RM126X_APP_KEY_BYTES_11_TO_8> Bytes 11 to 8 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_APP_KEY_BYTES_11_TO_8          0x0

// <o RM126X_APP_KEY_BYTES_7_TO_4> Bytes 7 to 4 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_APP_KEY_BYTES_7_TO_4           0x0

// <o RM126X_APP_KEY_BYTES_3_TO_0> Bytes 3 to 0 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_APP_KEY_BYTES_3_TO_0           0x0
// </h>

// <h> DevEUI
// <o RM126X_DEV_EUI_BYTES_7_TO_4> Bytes 7 to 4 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_DEV_EUI_BYTES_7_TO_4           0x0

// <o RM126X_DEV_EUI_BYTES_3_TO_0> Bytes 3 to 0 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_DEV_EUI_BYTES_3_TO_0           0x0
// </h>

// <h> JoinEUI
// <o RM126X_JOIN_EUI_BYTES_7_TO_4> Bytes 7 to 4 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_JOIN_EUI_BYTES_7_TO_4          0x0

// <o RM126X_JOIN_EUI_BYTES_3_TO_0> Bytes 3 to 0 <0x0..0xFFFFFFFF:0x1><f.h>
#define RM126X_JOIN_EUI_BYTES_3_TO_0          0x0
// </h>

// <<< end of configuration section >>>

#ifdef __cplusplus
}
#endif

#endif /* RM126X_CONFIG_H_ */
