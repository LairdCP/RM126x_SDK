/***************************************************************************//**
 * @file
 * @brief Peripheral application functions for MikroE 3331 temperature/humidity
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
#include "app_log.h"
#include "mikroe_shtc3.h"
#include "sl_i2cspm_instances.h"
#include "ez_common.h"
#include "app_formatters.h"
#include "app_peripheral.h"

// -----------------------------------------------------------------------------
// Private functions
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length);

// -----------------------------------------------------------------------------
// Private defines
#define MIKROE_SHTC3_ID_MASK 0x0807

// -----------------------------------------------------------------------------
// Public functions
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor)
{
  sl_status_t init_result;
  uint16_t id;

  /* Initialise the MikroE TH sensor */
  init_result = mikroe_shtc3_init(sl_i2cspm_mikroe);

  /* If the hardware wasn't set up OK, log this. */
  if (init_result)
  {
    app_log("\n3331 failed to initialise hardware.\n");
  }
  else
  {
    /* If the hardware initialised OK, try to read back the device id */
    init_result = mikroe_shtc3_read_register(MIKROE_SHTC3_CMD_READ_ID,
                                             SHTC3_REGISTER_ACCESS_SINGLE_DATA,
                                             &id);

    if (init_result)
    {
      app_log("\n3331 failed to initialise.\n");
    }
  }
  if (!init_result)
  {
    if ((id & MIKROE_SHTC3_ID_MASK) != MIKROE_SHTC3_ID_MASK)
    {
      app_log("\n3331 failed to communicate.\n");
      init_result = SL_STATUS_FAIL;
    }
  }
  /* Blank descriptor on error */
  if (init_result)
  {
    peripheral_descriptor->app_peripheral_get = NULL;
  }
  else
  {
    peripheral_descriptor->app_peripheral_get = app_peripheral_get;
  }
  /* No set behaviour for this board */
  peripheral_descriptor->app_peripheral_set = NULL;
}

void app_peripheral_update(void)
{
  /* No update activity for 3331 based peripherals */
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
  mikroe_shtc3_data_mode_t data_mode = SHTC3_DATA_MODE_NORMAL;
  mikroe_shtc3_measurement_data_t measurement_data;
  uint8_t bytes_added;

  /* Get the latest TH data */
  mikroe_shtc3_get_temperature_and_humidity(data_mode, &measurement_data);

  /* Then format the data into the client buffer */
  app_formatters_get_int_dec8(measurement_data.temperature, out_data);
  bytes_added = APP_FORMATTERS_INT_DEC8_SIZE;
  app_formatters_get_int_dec8(measurement_data.humidity,
                                         &out_data[bytes_added]);
  bytes_added += APP_FORMATTERS_INT_DEC8_SIZE;
  *out_length += bytes_added;
}
