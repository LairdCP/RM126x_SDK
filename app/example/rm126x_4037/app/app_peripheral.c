/***************************************************************************//**
 * @file
 * @brief Peripheral application functions for MikroE 4037 biometric boards.
 *******************************************************************************
 * LicenseID: LicenseRef-Ezurio-Clause
 *
 * Copyright (c) 2024 Ezurio LLC.
 *
 * All rights reserved.
 *
 * Section 1. Definitions
 *
 * "Authorized Product" means an Ezurio LLC or Laird Connectivity LLC hardware
 * or software product.
 *
 * Section 2. Software License Agreement
 *
 * Permission to use, copy, modify, and/or distribute the Software in source or
 * binary form is granted, provided the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * permission notice, and the disclaimer below.
 *
 * Neither the name of Ezurio LLC nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * The Software, with or without modification, may only be used with an
 * Authorized Product.
 *
 * If and to the extent that the Software is designed to be compliant with any
 * published or de facto standard, regulatory standard, or industry
 * specification, the Software may not be modified such that the Software or
 * Authorized Product would be incompatible with such standard or specification.
 *
 * Any Software provided in binary form under this license may not be reverse
 * engineered, decompiled, modified or disassembled.
 *
 * Section 3. Disclaimer
 *
 * THIS SOFTWARE IS PROVIDED BY EZURIO LLC "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. TO THE MAXIMUM EXTENT ALLOWED BY LAW, IN NO EVENT SHALL EZURIO
 * LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#include <stdint.h>
#include "sl_i2cspm_instances.h"
#include "app_formatters.h"
#include "ez_common.h"
#include <string.h>
#include "app_peripheral.h"
#include "sl_sleeptimer.h"
#include "maxm86161_hrm_spo2.h"
#include "hrm_helper.h"
#include "gpiointerrupt.h"
#include <inttypes.h>

// -----------------------------------------------------------------------------
// Private functions
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length);
static void app_peripheral_irq(uint8_t pin_no);

// -----------------------------------------------------------------------------
// Private variables
/** Sensor handle for the maxm86161 hrm algorithm */
static maxm_hrm_handle_t *hrmHandle;

/** Data Storage memory bock required by the maxm86161 hrm algorithm */
static mamx86161_hrm_data_storage_t hrm_data_storage;
static maxm86161_spo2_data_storage_t spo2_data;
static maxm86161_data_storage_t data_storage;
static mamx86161_hrm_data_t hrm_data;
static int16_t heart_rate;
static int16_t spo2 = 0;

// -----------------------------------------------------------------------------
// Public functions
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor)
{
  sl_status_t init_result;

  /* Initialise the MikroE HR sensor */

  /* Enable the HR sensor and wait briefly before reading registers */
  GPIO_PinModeSet(MAXM86161_EN_GPIO_PORT,
                  MAXM86161_EN_GPIO_PIN,
                  gpioModePushPull,
                  1);

  GPIO_PinModeSet(MAXM86161_INT_GPIO_PORT,
                  MAXM86161_INT_GPIO_PIN,
                  gpioModeInputPull,
                  1);

  GPIOINT_Init();
  GPIOINT_CallbackRegister(MAXM86161_INT_GPIO_PIN, app_peripheral_irq);
  GPIO_ExtIntConfig(MAXM86161_INT_GPIO_PORT,
                    MAXM86161_INT_GPIO_PIN,
                    MAXM86161_INT_GPIO_PIN,
                    true,
                    true,
                    true);

  sl_sleeptimer_delay_millisecond(100);

  data_storage.spo2 = &spo2_data;
  data_storage.hrm = &hrm_data_storage;

  init_result = maxm86161_hrm_initialize(&data_storage, &hrmHandle);
  if (!init_result)
  {
    init_result = maxm86161_hrm_configure(hrmHandle, NULL, true);
    sl_sleeptimer_delay_millisecond(100);
  }

  /* Run the HR service if initialised OK */
  if (!init_result)
  {
    maxm86161_hrm_run(hrmHandle);
    peripheral_descriptor->app_peripheral_get = app_peripheral_get;
  }
  else
  {
    /* Otherwise log and blank the descriptor */
    app_log("\n 4037 failed to initialise.\n");
    peripheral_descriptor->app_peripheral_get = NULL;
  }
  /* No set behaviour for 4037 boards */
  peripheral_descriptor->app_peripheral_set = NULL;
}

void app_peripheral_update(void)
{
  int16_t num_samples_processed;
  int32_t hrm_status;

  /* Check if we missed any events */
  if (!GPIO_PinInGet(MAXM86161_INT_GPIO_PORT, MAXM86161_INT_GPIO_PIN))
  {
    maxm86161_hrm_helper_process_irq();
  }

  (void)maxm86161_hrm_process(hrmHandle,
                              &heart_rate,
                              &spo2,
                              1,
                              &num_samples_processed,
                              &hrm_status, &hrm_data);
}

// -----------------------------------------------------------------------------
// Private functions

/***************************************************************************//**
 * Method called to get data from a peripheral.
 * @param[out] out_data - Data added by the peripheral.
 * @param[out] out_length - Additional count of data bytes added.
 ******************************************************************************/
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length)
{
  app_formatters_get_int16(heart_rate, out_data);
  *out_length += APP_FORMATTERS_INT16_SIZE;
}

/***************************************************************************//**
 * IRQ handler for 4037 IRQ pin.
 * @param[in] pin_no - Pin associated with the IRQ.
 ******************************************************************************/
static void app_peripheral_irq(uint8_t pin_no)
{
  if (pin_no == MAXM86161_INT_GPIO_PIN)
  {
    maxm86161_hrm_helper_process_irq();
  }
}
