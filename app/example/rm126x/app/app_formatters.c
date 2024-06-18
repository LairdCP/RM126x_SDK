/***************************************************************************//**
 * @file
 * @brief Data formatter functions
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
#include <math.h>
#include "app_formatters.h"

void app_formatters_get_int_dec8(float in_data, uint8_t *out_data)
{
  out_data[0] = (int8_t)truncf(in_data);
  in_data -= ((float)(out_data[0]));
  out_data[1] = ((int8_t)(in_data * 100.0f));
}

void app_formatters_get_int_dec16(float in_data, uint8_t *out_data)
{
  int16_t int_data;
  int16_t dec_data;

  int_data = (int16_t)truncf(in_data);
  in_data -= ((float)(int_data));
  dec_data = ((int16_t)(in_data * 100.0f));

  out_data[0] = ((uint8_t)(int_data >> 8));
  out_data[1] = ((uint8_t)(int_data & 0x00FF));
  out_data[2] = ((uint8_t)(dec_data >> 8));
  out_data[3] = ((uint8_t)(dec_data & 0x00FF));
}

void app_formatters_get_uint16(uint16_t in_data, uint8_t *out_data)
{
  out_data[0] = (uint8_t)(in_data >> 8);
  out_data[1] = (uint8_t)(in_data & 0xFF);
}

void app_formatters_get_int16(int16_t in_data, uint8_t *out_data)
{
  out_data[0] = (uint8_t)(in_data >> 8);
  out_data[1] = (uint8_t)(in_data & 0xFF);
}

void app_formatters_get_int32(int32_t in_data, uint8_t *out_data)
{
  out_data[0] = (uint8_t)(in_data >> 24);
  out_data[1] = (uint8_t)(in_data >> 16);
  out_data[2] = (uint8_t)(in_data >> 8);
  out_data[3] = (uint8_t)(in_data & 0xFF);
}

void app_formatters_get_uint32(uint32_t in_data, uint8_t *out_data)
{
  out_data[0] = (uint8_t)(in_data >> 24);
  out_data[1] = (uint8_t)(in_data >> 16);
  out_data[2] = (uint8_t)(in_data >> 8);
  out_data[3] = (uint8_t)(in_data & 0xFF);
}
