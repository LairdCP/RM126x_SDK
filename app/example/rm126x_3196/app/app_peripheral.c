/***************************************************************************//**
 * @file
 * @brief Peripheral application functions for MikroE 3196 carbon monoxide
 *        sensor boards.
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
#include <string.h>
#include "app_peripheral.h"
#include "sl_i2cspm_instances.h"
#include "ez_common.h"
#include <math.h>
#include "em_gpio.h"
#include "sl_i2cspm.h"
#include "co2.h"
#include "app_formatters.h"
#include "app_log.h"

// -----------------------------------------------------------------------------
// Private functions
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length);

// -----------------------------------------------------------------------------
// Private variables
static co2_t co2;

// -----------------------------------------------------------------------------
// Private defines
#define CO2_DEVICE_STATUS_REGISTER 0x0
#define CO2_DEVICE_STATUS_SIZE 1
#define CO2_DEVICE_STATUS_READY 1

// -----------------------------------------------------------------------------
// Public functions
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor)
{
  co2_cfg_t co2_cfg;
  uint8_t temp_w;
  sl_status_t init_result;
  uint8_t rx_buf[CO2_DEVICE_STATUS_SIZE];

  /* Initialise the MikroE CO sensor configuration block */
  co2_cfg_setup(&co2_cfg);

  /* I2C port */
  co2.i2c.handle = sl_i2cspm_mikroe;

  /* If the host AD is being used to sense CO levels, configure it here.
   * Otherwise CO level readings are via the on-board AD via I2C.
   */
#ifndef CO2_ADC_ON_I2C
  co2.an.handle = IADC0;
  co2_cfg.an.input_pin = hal_gpio_pin_name(MIKROE_AN_PORT, MIKROE_AN_PIN);
  co2_cfg.an.resolution = ANALOG_IN_RESOLUTION_12_BIT;
  co2_cfg.an.vref_input = ANALOG_IN_VREF_INTERNAL;
  co2_cfg.an.vref_value = 3.3f;
#endif

  /* Reset pin */
  co2_cfg.rst = hal_gpio_pin_name(MIKROE_RESET_PORT, MIKROE_RESET_PIN);

  /* Initialise the board */
  init_result = co2_init(&co2, &co2_cfg);

  /* Check board status if initialised OK */
  if (!init_result)
  {
    co2_generic_read(&co2, CO2_DEVICE_STATUS_REGISTER, rx_buf,
                     CO2_DEVICE_STATUS_SIZE);

    if (!(rx_buf[0] & CO2_DEVICE_STATUS_READY))
    {
      init_result = SL_STATUS_FAIL;
    }
  }

  /* Start the CO sensor if OK to do so */
  if (!init_result)
  {
    /* Configure board */
    temp_w = CO2_WRITE_MODE;
    co2_generic_write(&co2, CO2_LOCK_REG, &temp_w, 1);
    temp_w = CO2_STANDBY_MODE;
    co2_generic_write(&co2, CO2_MODECN_REG, &temp_w, 1);
    temp_w = CO2_3500_OHM_TIA_RES | CO2_100_OHM_LOAD_RES;
    co2_generic_write(&co2, CO2_TIACN_REG, &temp_w, 1);
    temp_w = CO2_VREF_EXT | CO2_50_PERCENTS_INT_ZERO | CO2_BIAS_POL_NEGATIVE |
             CO2_0_PERCENTS_BIAS;
    co2_generic_write(&co2, CO2_REFCN_REG, &temp_w, 1);
  }

  /* Log if initialisation failed */
  if (init_result)
  {
    app_log("\n 3196 initialisation failed.\n");
    peripheral_descriptor->app_peripheral_get = NULL;
  }
  else
  {
    /* Otherwise set up the data descriptor */
    peripheral_descriptor->app_peripheral_get = app_peripheral_get;
  }
  /* No set behaviour for 3196 boards */
  peripheral_descriptor->app_peripheral_set = NULL;
}

void app_peripheral_update(void)
{
  /* No update behaviour for CO sensor */
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
  float ppm_data;

  co2_get_co2_ppm(&co2, &ppm_data);
  app_formatters_get_int_dec16(ppm_data, out_data);
  *out_length += APP_FORMATTERS_INT_DEC16_SIZE;
}
