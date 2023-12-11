/*******************************************************************************
  Filename:       string_conversion.h
  Revised:        08/12/2023
  Revision:       1.0

  Description:    this file contains various string manipulation
                  functions commonly used throughout the application.

  Copyright (c) 2023 Laird Connectivity LLC.
 
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
  modification, are allowed if following conditions are met:
 
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the disclaimer below.
 
  2. Redistributions in binary form, except as embedded into a Laird
     Connectivity LLC module in a product, a Laird Connectivity LLC product, or
     a software update for such products, must reproduce the above copyright
     notice, this list of conditions and the disclaimer in the documentation
     and/or other materials provided with the distribution.
 
  3. Neither the name of Laird Connectivity LLC nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.
 
  4. This software, with or without modification, may only be used with a Laird
     Connectivity LLC module or Laird Connectivity LLC product.
 
  5. Any software provided in binary form under this license may not be reverse
     engineered, decompiled, modified or disassembled.
 
  THIS SOFTWARE IS PROVIDED BY LAIRD CONNECTIVITY LLC "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. TO THE MAXIMUM EXTENT ALLOWED BY LAW, IN NO EVENT SHALL LAIRD
  CONNECTIVITY LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/
#ifndef LC_COMPONENTS_HDR_STRING_CONVERSION_H_
#define LC_COMPONENTS_HDR_STRING_CONVERSION_H_

#define HEX_DECODE_LOWER_CASE_ALPHA_SUBTRACT      0x57
#define HEX_DECODE_UPPER_CASE_ALPHA_SUBTRACT      0x37
#define HEX_DECODE_NUMERIC_SUBTRACT               0x30
#define HEX_ENCODE_LOWER_CASE_ALPHA_ADDITION      0x27
#define HEX_ENCODE_UPPER_CASE_ALPHA_ADDITION      0x7

#define UUID128_LEN 16
#define UUID16_LEN 2
#define UUID128_STR_LEN (UUID128_LEN * 2 + 1)

#define STR_NOT_FOUND -1
#define CHR_ASCII_SPACE                             0x20

uint8_t* lc_str_deescape(uint8_t *ptr, uint32_t *length);
void lc_str_strescape(uint8_t *ptr, size_t *length, uint8_t *ptr1);
uint8_t lc_str_strdehexize(uint8_t *ptr);
void lc_str_hex_decode(uint8_t *out, uint8_t *in, uint32_t len);
sl_status_t lc_str_string_to_uuid128(uint8_t *out, uint8_t *in, uint32_t len);
bool lc_str_is_decimal_digits(char *in_string);
bool lc_str_is_hex_digits(char *in_string);
bool lc_str_is_signed_decimal_digits(char *in_string);
void lc_str_decimal_to_2_digit_str(uint8_t in, char *out);
void lc_str_bin_to_hex_dump_std(uint8_t *input, uint8_t *output, size_t len);
void lc_str_bin_to_hex_dump_restricted(char *message, uint16_t message_size,
    uint8_t *data, uint8_t data_length);
void lc_str_bin_to_hex_single(uint8_t *out, uint8_t data);
int lc_str_pos(char *str, char *str_sub, int start_pos, int length_str);
void lc_str_split_left(const char *str, int length, char *ptr);
void lc_str_to_lower(char *str);
void lc_str_shift_left(char *cmd_str, int drop);
int lc_str_get_chr(char *str, int i);
void lc_drop_white_space(char *cmd_str, int pos);
void lc_output_immediate_hex_bytes(uint8_t *out_buffer, uint16_t read_length);

#endif /* LC_COMPONENTS_HDR_STRING_CONVERSION_H_ */
