/***************************************************************************//**
 * @file rm1261_config.h
 * @brief Configuration file for RM1261 module component code.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 * <b>Copyright 2023 Laird Connectivity. www.lairdconnect.com</b>
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

#ifndef RM1261_CONFIG_H_
#define RM1261_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Region
// <o RM126X_REGION> Region
// <16=> EU_868
// <17=> IN_865
// <18=> JP_AS_923
// <24=> TW_AS_923
// <25=> UK_868
// <i> Default: 16
#define RM126X_REGION                   16
// </h>

// <<< end of configuration section >>>

// <h> Sub-band
// <o RM126X_SUB_BAND> Sub-band, unused for RM1261
// <i> Default: 0
#define RM126X_SUB_BAND                       0
// </h>

#ifdef __cplusplus
}
#endif

#endif /* RM1261_CONFIG_H_ */
