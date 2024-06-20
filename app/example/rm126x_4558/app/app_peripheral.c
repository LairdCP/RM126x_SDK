/***************************************************************************//**
 * @file
 * @brief Peripheral application functions for MikroE 4558 environmental
 *        sensing boards.
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
/*******************************************************************************
** Copyright (C) 2020 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include <stdint.h>
#include "mikroe_sht40_sgp40.h"
#include "sl_i2cspm_instances.h"
#include "app_formatters.h"
#include "app_peripheral.h"
#include "app_log.h"

// -----------------------------------------------------------------------------
// Private functions
static void app_peripheral_get(uint8_t *out_data, uint8_t *out_length);
static uint8_t app_peripheral_dev_calc_crc(uint8_t data_0, uint8_t data_1);
static sl_status_t app_peripheral_crc_block(uint8_t *in_data,
                                            uint8_t in_crc_count);
static sl_status_t app_peripheral_check_sht40_id(i2c_master_t *i2c_master);
static sl_status_t app_peripheral_check_sgp40_id(i2c_master_t *i2c_master);

// -----------------------------------------------------------------------------
// Private defines
#define MIKROE_ENVIRONMENT2_SHT40_CMD_GET_ID 0x89
#define MIKROE_ENVIRONMENT2_SHT40_CMD_RESET 0x94
#define MIKROE_ENVIRONMENT2_SHT40_CMD_SIZE 1
#define MIKROE_ENVIRONMENT2_SHT40_CRC_COUNT 2
#define MIKROE_ENVIRONMENT2_SHT40_ID_SIZE 6

#define MIKROE_ENVIRONMENT2_SGP40_CMD_GET_ID 0x3682
#define MIKROE_ENVIRONMENT2_SGP40_CMD_SIZE 2
#define MIKROE_ENVIRONMENT2_SGP40_CRC_COUNT 3
#define MIKROE_ENVIRONMENT2_SGP40_ID_SIZE 9

// -----------------------------------------------------------------------------
// Public functions
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor)
{
  sl_status_t init_result = SL_STATUS_OK;
  i2c_master_t i2c_master;

  /* Validate part ids before starting driver */
  i2c_master.handle = sl_i2cspm_mikroe;

  i2c_master_config_t i2c_master_config;
  i2c_master_config.scl = hal_gpio_pin_name(SL_I2CSPM_MIKROE_SCL_PORT,
                                            SL_I2CSPM_MIKROE_SCL_PIN);
  i2c_master_config.sda = hal_gpio_pin_name(SL_I2CSPM_MIKROE_SDA_PORT,
                                            SL_I2CSPM_MIKROE_SDA_PIN);
  i2c_master_config.speed = I2C_MASTER_SPEED_STANDARD;
  i2c_master_config.timeout_pass_count = 10000;
  i2c_master_open(&i2c_master, &i2c_master_config);

  /* Start with SHT40 */
  init_result = app_peripheral_check_sht40_id(&i2c_master);
  if (!init_result)
  {
    /* Then SGP40 */
    init_result = app_peripheral_check_sgp40_id(&i2c_master);
  }
  /* Destroy local I2C master instance - not needed anymore */
  i2c_master_close(&i2c_master);

  /* Initialise the MikroE Env sensor */
  if (!init_result)
  {
    init_result = mikroe_environment2_init(sl_i2cspm_mikroe);
  }
  if (!init_result)
  {
    init_result = mikroe_environment2_sgp40_heater_off();
  }
  if (!init_result)
  {
    init_result = mikroe_environment2_config_sensors();
  }
  /* If initialisation failed, log it and blank the descriptor */
  if (init_result)
  {
    app_log("\n4558 failed to initialise.\n");
    peripheral_descriptor->app_peripheral_get = NULL;
  }
  else
  {
    peripheral_descriptor->app_peripheral_get = app_peripheral_get;
  }
  /* No set behaviour for 4558 boards */
  peripheral_descriptor->app_peripheral_set = NULL;
}

void app_peripheral_update(void)
{
  /* No update activity for 4558 based peripherals */
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
  float humidity;
  float temperature;
  int32_t voc_index;
  uint8_t bytes_added;

  mikroe_environment2_get_temp_hum(&humidity, &temperature);
  mikroe_environment2_get_voc_index(&voc_index);

  app_formatters_get_int_dec8(temperature, out_data);
  bytes_added = APP_FORMATTERS_INT_DEC8_SIZE;
  app_formatters_get_int_dec8(humidity, &out_data[bytes_added]);
  bytes_added += APP_FORMATTERS_INT_DEC8_SIZE;
  app_formatters_get_int32(voc_index, &out_data[bytes_added]);
  bytes_added += APP_FORMATTERS_INT32_SIZE;
  *out_length += bytes_added;
}

/***************************************************************************//**
 * Taken from environment2 (not exposed as public).
 * @param[in] data_0 - First CRC data byte.
 * @param[in] data_1 - Second CRC data byte.
 * @return - Calculated CRC.
 ******************************************************************************/
static uint8_t app_peripheral_dev_calc_crc(uint8_t data_0, uint8_t data_1)
{
  uint8_t i_cnt;
  uint8_t j_cnt;
  uint8_t crc_data[2];
  uint8_t crc = 0xFF;

  crc_data[0] = data_0;
  crc_data[1] = data_1;

  for ( i_cnt = 0; i_cnt < 2; i_cnt++ )
  {
    crc ^= crc_data[i_cnt];

    for ( j_cnt = 8; j_cnt > 0; --j_cnt )
    {
      if (crc & 0x80)
      {
        crc = (crc << 1) ^ 0x31u;
      }
      else
      {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

/***************************************************************************//**
 * Validates SHT/SGP40 style I2C read buffers where data is returned in two byte
 *           pairs followed by an 8-bit CRC.
 * @param[in] in_data - The I2C read buffer.
 * @param[in] in_crc_count - The number of CRCs to check.
 * @return - SL_STATUS_OK if all CRCs match.
 ******************************************************************************/
static sl_status_t app_peripheral_crc_block(uint8_t *in_data, uint8_t in_crc_count)
{
  sl_status_t err = SL_STATUS_OK;
  uint8_t crc_count;
  uint8_t crc;
  uint8_t crc_byte_index = 0;

  for (crc_count = 0; (crc_count < in_crc_count) && (err == SL_STATUS_OK);
       crc_count++)
  {
    crc = app_peripheral_dev_calc_crc(in_data[crc_byte_index],
                                      in_data[crc_byte_index + 1]);
    if (crc != in_data[crc_byte_index + 2])
    {
      err = SL_STATUS_FAIL;
    }
    crc_byte_index += 3;
  }
  return(err);
}

/***************************************************************************//**
 * Validates 4558 SHT40 device id.
 * @param[in] i2c_master - Preinitialised I2C master instance.
 * @return - SL_STATUS_OK if id OK.
 ******************************************************************************/
static sl_status_t app_peripheral_check_sht40_id(i2c_master_t *i2c_master)
{
  uint8_t write_buf[MIKROE_ENVIRONMENT2_SHT40_CMD_SIZE];
  uint8_t read_buf[MIKROE_ENVIRONMENT2_SHT40_ID_SIZE];
  sl_status_t err;

  (void)i2c_master_set_slave_address(i2c_master,
                                     ENVIRONMENT2_SHT40_SET_DEV_ADDR);

  /* Reset the part */
  write_buf[0] = MIKROE_ENVIRONMENT2_SHT40_CMD_RESET;

  err = i2c_master_write(i2c_master, write_buf,
                         MIKROE_ENVIRONMENT2_SHT40_CMD_SIZE);

  sl_sleeptimer_delay_millisecond(1);
  if (!err)
  {
    write_buf[0] = MIKROE_ENVIRONMENT2_SHT40_CMD_GET_ID;
    err = i2c_master_write(i2c_master, write_buf,
                           MIKROE_ENVIRONMENT2_SHT40_CMD_SIZE);
    sl_sleeptimer_delay_millisecond(1);
    err = i2c_master_read(i2c_master, read_buf,
                          MIKROE_ENVIRONMENT2_SHT40_ID_SIZE);
  }
  if (!err)
  {
    err = app_peripheral_crc_block(read_buf,
                                   MIKROE_ENVIRONMENT2_SHT40_CRC_COUNT);
  }
  return(err);
}

/***************************************************************************//**
 * Validates 4558 SGP40 device id.
 * @param[in] i2c_master - Preinitialised I2C master instance.
 * @return - SL_STATUS_OK if id OK.
 ******************************************************************************/
static sl_status_t app_peripheral_check_sgp40_id(i2c_master_t *i2c_master)
{
  uint8_t write_buf[MIKROE_ENVIRONMENT2_SGP40_CMD_SIZE];
  uint8_t read_buf[MIKROE_ENVIRONMENT2_SGP40_ID_SIZE];
  sl_status_t err;

  (void)i2c_master_set_slave_address(i2c_master,
                                     ENVIRONMENT2_SGP40_SET_DEV_ADDR);

  write_buf[0] = (uint8_t)(MIKROE_ENVIRONMENT2_SGP40_CMD_GET_ID >> 8);
  write_buf[1] = (uint8_t)(MIKROE_ENVIRONMENT2_SGP40_CMD_GET_ID & 0xFF);

  err = i2c_master_write(i2c_master, write_buf,
                         MIKROE_ENVIRONMENT2_SGP40_CMD_SIZE);
  sl_sleeptimer_delay_millisecond(1);
  err = i2c_master_read(i2c_master, read_buf,
                        MIKROE_ENVIRONMENT2_SGP40_ID_SIZE);
  if (!err)
  {
    err = app_peripheral_crc_block(read_buf,
                                   MIKROE_ENVIRONMENT2_SGP40_CRC_COUNT);
  }
  return(err);
}
