/***************************************************************************//**
 * @file rm1262_config.h
 * @brief Configuration file for RM1262 module component code.
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

#ifndef RM1262_CONFIG_H_
#define RM1262_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Region
// <o RM126X_REGION> Region
// <14=> AU_915
// <15=> AU_AS_923
// <20=> NZ_915
// <21=> NZ_AS_923
// <26=> US_915
// <i> Default: 26
#define RM126X_REGION                   26
// </h>

// <h> Sub-band
// <o RM126X_SUB_BAND> Sub-band
// <0=> All 125kHz and 500kHz channels enabled
// <1=> 1: 125kHz channels 0 to 7 and 500kHz channel 0 enabled
// <2=> 2: 125kHz channels 8 to 15 and 500kHz channel 1 enabled
// <3=> 3: 125kHz channels 16 to 23 and 500kHz channel 2 enabled
// <4=> 4: 125kHz channels 24 to 31 and 500kHz channel 3 enabled
// <5=> 5: 125kHz channels 32 to 39 and 500kHz channel 4 enabled
// <6=> 6: 125kHz channels 40 to 47 and 500kHz channel 5 enabled
// <7=> 7: 125kHz channels 48 to 55 and 500kHz channel 6 enabled
// <8=> 8: 125kHz channels 56 to 63 and 500kHz channel 7 enabled
// <i> Default: 2
#define RM126X_SUB_BAND                       2
// </h>

// <<< end of configuration section >>>

#ifdef __cplusplus
}
#endif

#endif /* RM1262_CONFIG_H_ */
