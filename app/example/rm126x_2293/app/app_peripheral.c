/***************************************************************************//**
 * @file
 * @brief Peripheral application functions for MikroE 2293 weather station
 *        boards.
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
#include "app_peripheral.h"
#include "mikroe_dps310_spi.h"
#include "sl_spidrv_instances.h"
#include "ez_common.h"
#include "app_formatters.h"

// -----------------------------------------------------------------------------
// Private functions
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length);

// -----------------------------------------------------------------------------
// Public functions
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor)
{
  sl_status_t init_result;

  /* Initialise the MikroE Pressure sensor */
  init_result = mikroe_pressure3_init(sl_spidrv_mikroe_handle);

  /* Blank the descriptor if initialisation failed */
  if (init_result)
  {
    app_log("\n2293 failed to initialise hardware.\n");
    peripheral_descriptor->app_peripheral_get = NULL;
  }
  else
  {
    peripheral_descriptor->app_peripheral_get = app_peripheral_get;
  }
  /* No set behaviour for 2293 peripherals */
  peripheral_descriptor->app_peripheral_set = NULL;
}

void app_peripheral_update(void)
{
  /* No update activity for 2293 based peripherals */
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
  float temperature;
  float pressure;
  uint8_t bytes_added;

  mikroe_pressure3_get_t_p_data(&temperature, &pressure);

  /* Then format the data into the client buffer */
  app_formatters_get_int_dec8(temperature, out_data);
  bytes_added = APP_FORMATTERS_INT_DEC8_SIZE;
  app_formatters_get_int_dec16(pressure, &out_data[bytes_added]);
  bytes_added += APP_FORMATTERS_INT_DEC16_SIZE;
  *out_length += bytes_added;
}
