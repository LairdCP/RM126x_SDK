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
#ifndef APP_FORMATTERS_H
#define APP_FORMATTERS_H

/* Width of data returned by app_formatters_get_int_dec8 */
#define APP_FORMATTERS_INT_DEC8_SIZE 2
/* Width of data returned by app_formatters_get_int_dec16 */
#define APP_FORMATTERS_INT_DEC16_SIZE 4
/* Width of data returned by app_formatters_get_uint16 */
#define APP_FORMATTERS_UINT16_SIZE 2
/* Width of data returned by app_formatters_get_int16 */
#define APP_FORMATTERS_INT16_SIZE 2
/* Width of data returned by app_formatters_get_int32 */
#define APP_FORMATTERS_INT32_SIZE 4
/* Width of data returned by app_formatters_get_uint32 */
#define APP_FORMATTERS_UINT32_SIZE 4

/***************************************************************************//**
 * Converts a float into int8 decimal and fractional parts.
 * @param[in] in_data - Float data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_int_dec8(float in_data, uint8_t *out_data);

/***************************************************************************//**
 * Converts a float into int16 decimal and fractional parts.
 * @param[in] in_data - Float data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_int_dec16(float in_data, uint8_t *out_data);

/***************************************************************************//**
 * Converts a uint16 into big endian format.
 * @param[in] in_data - UInt16 data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_uint16(uint16_t in_data, uint8_t *out_data);

/***************************************************************************//**
 * Converts an int16 into big endian format.
 * @param[in] in_data - Int16 data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_int16(int16_t in_data, uint8_t *out_data);

/***************************************************************************//**
 * Converts an int32 into big endian format.
 * @param[in] in_data - Int32 data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_int32(int32_t in_data, uint8_t *out_data);

/***************************************************************************//**
 * Converts an uint32 into big endian format.
 * @param[in] in_data - UInt32 data to convert.
 * @param[out] out_data - Formatted uint8 data.
 ******************************************************************************/
void app_formatters_get_uint32(uint32_t in_data, uint8_t *out_data);

#endif  // APP_FORMATTERS_H
