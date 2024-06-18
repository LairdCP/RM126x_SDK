/*!
 * @file      smtc_hal_gpio_pin_names.h
 *
 * @brief     Defines RM126x platform pin names
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 * Copyright Ezurio 2023. All rights reserved.
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

#ifndef SMTC_HAL_GPIO_PIN_NAMES_H
#define SMTC_HAL_GPIO_PIN_NAMES_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

#define PA_COUNT 0x9
#define PB_COUNT 0x5
#define PC_COUNT 0x8
#define PD_COUNT 0x4

/*!
 * @brief HAL Gpio pin name
 */
typedef enum hal_gpio_pin_names_e
{
    // GPIOA
    PA_0  = 0x0,
    PA_1  = 0x1,
    PA_2  = 0x2,
    PA_3  = 0x3,
    PA_4  = 0x4,
    PA_5  = 0x5,
    PA_6  = 0x6,
    PA_7  = 0x7,
    PA_8  = 0x8,

    // GPIOB
    PB_0 = 0x09,
    PB_1 = 0x0A,
    PB_2 = 0x0B,
    PB_3 = 0x0C,
    PB_4 = 0x0D,

    // GPIOC
    PC_0  = 0x0E,
    PC_1  = 0x0F,
    PC_2  = 0x10,
    PC_3  = 0x11,
    PC_4  = 0x12,
    PC_5  = 0x13,
    PC_6  = 0x14,
    PC_7  = 0x15,

    // GPIOD
    PD_0 = 0x16,
    PD_1 = 0x17,
    PD_2 = 0x18,
    PD_3 = 0x19,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // Not connected
    NC = -1
} hal_gpio_pin_names_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // SMTC_HAL_GPIO_PIN_NAMES_H
