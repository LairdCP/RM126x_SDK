/*******************************************************************************
  Filename:       string_conversion.c
  Revised:        08/12/2023
  Revision:       1.0

  Description:    this file contains various string manipulation
                  functions commonly used throughout the application. Many of the
                  functions are ported from other Ezurio projects.

  Copyright (c) 2024 Ezurio.
 
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
  modification, are allowed if following conditions are met:
 
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the disclaimer below.
 
  2. Redistributions in binary form, except as embedded into an Ezurio LLC
     module in a product, an Ezurio LLC product, or a software update for such
     products, must reproduce the above copyright notice, this list of
     conditions and the disclaimer in the documentation and/or other materials
     provided with the distribution.

  3. Neither the name of Ezurio LLC nor the names of its contributors may be
     used to endorse or promote products derived from this software without
     specific prior written permission.

  4. This software, with or without modification, may only be used with an
     Ezurio LLC module or Ezurio LLC product.
 
  5. Any software provided in binary form under this license may not be reverse
     engineered, decompiled, modified or disassembled.
 
  THIS SOFTWARE IS PROVIDED BY EZURIO LLC "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. TO THE MAXIMUM EXTENT ALLOWED BY LAW, IN NO EVENT SHALL EZURIO
  LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/
#include <ctype.h>
#include <sl_status.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_log.h"
#include "ez_common.h"
#include "string_conversion.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
/* ASCII definitions */
#define ASCII_NULL            0x00
#define ASCII_HORIZONTAL_TAB  0x09
#define ASCII_LINE_FEED       0x0a
#define ASCII_CARRIAGE_RETURN 0x0d
#define ASCII_SPACE           0x20
#define ASCII_LEFT_SLASH      0x5c
#define ASCII_DOUBLE_QUOTES   0x22
#define ASCII_DEL             0x7f
#define ASCII_ZERO            0x30

/* HEX definitions */
#define DEC_2_DIGIT_MAX       99
#define DEC_DIGIT_1_IDX       0
#define DEC_DIGIT_2_IDX       1
#define DEC_TENS_DIVISOR      10

/* Fast and small hex output defines */
#define TOP_NYBBLE_MASK 0xF0
#define TOP_NYBBLE_SHIFT 4
#define BOTTOM_NYBBLE_MASK 0x0F
#define NEXT_OCTET 2
#define MESSAGE_TERMINATOR_SAFETY 3
#define OCTET_PLUS_NULL_LENGTH 3

/****************************************************************************
 * Constant data
 ****************************************************************************/
const char hex_conversion[] = "0123456789ABCDEF";

/*******************************************************************************
 ***************************  LOCAL FUNCTIONS PROTOTYPES ***********************
 ******************************************************************************/
static int cleanup_string(uint8_t *ptr, uint32_t position, uint32_t length,
                          uint32_t chars_to_remove);
static void insert_string(uint8_t *ptr, uint32_t *position, uint8_t *insert,
                          uint32_t chars_to_add);

/*******************************************************************************
 ***************************  LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

/******************************************************************************
 * This inserts a string into another string at a specified position
 *
 * @param[in/out] ptr - the string to modify
 * @param[in] position - the pointer to the position index for insertion
 * @param[in] insert - the pointer to the characters to insert
 * @param[in] chars_to_add - the number of chars to add
 *
 * @retval none
 *****************************************************************************/
static void insert_string(uint8_t *ptr, uint32_t *position,
                          uint8_t *insert, uint32_t chars_to_add)
{
  uint32_t insert_loop;

  if (ptr != NULL)
  {
    insert_loop = 0;
    while (insert_loop < chars_to_add)
    {
      *(ptr + (insert_loop + *position)) = *(insert + insert_loop);

      insert_loop++;
    }
  }

  *position += chars_to_add;
}

/******************************************************************************
 * Internal function that is used to remove specified characters and to update
 * the length.
 *
 * @param[in] ptr - the string to modify
 * @param[in] position - the position to remove chars
 * @param[in] length - the length of the input string
 * @param[in] chars_to_remove - the number of chars to remove
 *
 * @retval The new length of the string.
 *****************************************************************************/
static int cleanup_string(uint8_t *ptr, uint32_t position, uint32_t length,
                          uint32_t chars_to_remove)
{
  uint32_t cleanup_loop;

  cleanup_loop = position + 1;
  while (cleanup_loop < length)
  {
    *(ptr + cleanup_loop) = *(ptr + (cleanup_loop + chars_to_remove));
    cleanup_loop++;
  }

  return (length - chars_to_remove);
}

/*******************************************************************************
 ***************************  GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/******************************************************************************
 * This converts the escape characters in a string to their binary form.
 *
 * @param[in/out] ptr - the string to convert escape characters on
 * @param[in/out] length - the length of the string.
 *
 * @retval char * - the pointer to the original input string
 *****************************************************************************/
uint8_t* ez_str_deescape(uint8_t *ptr, uint32_t *length)
{
  uint32_t loop;
  uint8_t control_string[2];
  bool do_not_move_forward = false;

  if (ptr != NULL)
  {
    loop = 0;
    while (loop < *length)
    {
      do_not_move_forward = false;

      /* Do the conversion */
      /* Handle \r */
      if ((*(ptr + loop) == ASCII_LEFT_SLASH) && (*(ptr + (loop + 1)) == 'r'))
      {
        *(ptr + loop) = ASCII_CARRIAGE_RETURN;
        *length = cleanup_string(ptr, loop, *length, 1);
      }

      /* Handle \n */
      if ((*(ptr + loop) == ASCII_LEFT_SLASH) && (*(ptr + (loop + 1)) == 'n'))
      {
        *(ptr + loop) = ASCII_LINE_FEED;
        *length = cleanup_string(ptr, loop, *length, 1);
      }

      /* Handle \t */
      if ((*(ptr + loop) == ASCII_LEFT_SLASH) && (*(ptr + (loop + 1)) == 't'))
      {
        *(ptr + loop) = ASCII_HORIZONTAL_TAB;
        *length = cleanup_string(ptr, loop, *length, 1);
      }

      /* Handle \\ */
      if ((*(ptr + loop) == ASCII_LEFT_SLASH) &&
          (*(ptr + (loop + 1)) == ASCII_LEFT_SLASH))
      {
        /* Keep the first \ and remove the second. */
        *length = cleanup_string(ptr, loop, *length,1);
      }

      /* Handle "" */
      if ((*(ptr + loop) == ASCII_DOUBLE_QUOTES) &&
          (*(ptr + (loop + 1)) == ASCII_DOUBLE_QUOTES))
      {
        /* Keep the first " and remove the second. */
        *length = cleanup_string(ptr, loop, *length, 1);
      }

      /* Handle \HH ASCII byte HH */
      /* Check that the order is \ hex digit hex digit */
      if ((*(ptr + loop) == ASCII_LEFT_SLASH) &&
          isxdigit((int)(*(ptr + (loop + 1)))) &&
          isxdigit((int)(*(ptr + (loop + 2)))) )
      {
        /* Extract the hex value */
        control_string[0] = *(ptr + (loop + 1));
        control_string[1] = *(ptr + (loop + 2));

        *(ptr + loop) = (uint8_t)strtol((char *)control_string, NULL, BASE_HEX);

        /* Now cleanup the string. */
        *length = cleanup_string(ptr, loop, *length, 2);

        do_not_move_forward = true;
      }

      if (do_not_move_forward == false)
      {
        loop++;
      }
    }

    /* Make sure the resulting string is null terminated. */
    *(ptr + *length) = ASCII_NULL;
  }

  return ptr;
}

/******************************************************************************
 * This produces a printable version of a string.
 *
 * @param[in] ptr     - the string to convert
 * @param[in] *length - the length of the input string
 * @param[out] ptr1   - the resulting string
 *
 * @retval none
 *****************************************************************************/
void ez_str_strescape(uint8_t *ptr, size_t *length, uint8_t *ptr1)
{
  uint32_t loop;
  uint32_t output_pos = 0;
  uint8_t control_string[4];

  if ((ptr != NULL) && (ptr1 != NULL))
  {
    loop = 0;
    while (loop < *length)
    {
      /* Do the conversion */
      /* Handle \r */
      if (*(ptr + loop) == ASCII_CARRIAGE_RETURN)
      {
        control_string[0] = ASCII_LEFT_SLASH;
        control_string[1] = 'r';

        insert_string(ptr1, &output_pos, control_string, 2);
      }

      /* Handle \n */
      else if (*(ptr + loop) == ASCII_LINE_FEED)
      {
        control_string[0] = ASCII_LEFT_SLASH;
        control_string[1] = 'n';

        insert_string(ptr1, &output_pos, control_string, 2);
      }

      /* Handle \t */
      else if (*(ptr + loop) == ASCII_HORIZONTAL_TAB)
      {
        control_string[0] = ASCII_LEFT_SLASH;
        control_string[1] = 't';

        insert_string(ptr1, &output_pos, control_string, 2);
      }

      /* Handle \\ */
      else if (*(ptr + loop) == ASCII_LEFT_SLASH)
      {
        control_string[0] = ASCII_LEFT_SLASH;
        control_string[1] = ASCII_LEFT_SLASH;

        insert_string(ptr1, &output_pos, control_string, 2);
      }

      /* Handle " */
      else if (*(ptr + loop) == ASCII_DOUBLE_QUOTES)
      {
        control_string[0] = ASCII_LEFT_SLASH;
        control_string[1] = ASCII_DOUBLE_QUOTES;

        insert_string(ptr1, &output_pos, control_string, 2);
      }

      /* Handle characters that have ASCII values that are less than 0x20
       * and above 0x7F i.e. unprintable characters.
       */
      else if ((*(ptr + loop) < ASCII_SPACE) ||
          (*(ptr + loop) >= ASCII_DEL))
      {
        /* Convert the value into a two character string. */
        /* Make sure the first character is \ then the character string .*/
        /* Insert it. */
        sprintf((char *)control_string, "\\%.2x", *(ptr + loop));
        insert_string(ptr1, &output_pos, control_string, 3);
      }
      else
      {
        /* The character is not special, so just copy it. */
        *(ptr1 + output_pos) = *(ptr + loop);
        output_pos++;
      }

      loop++;
    }

    /* Update the length of the string. */
    *length = output_pos;

    /* Make sure the new string is null terminated. */
    *(ptr1 + output_pos) = ASCII_NULL;
  }
}

/******************************************************************************
 * Is used to convert a string consisting of hex digits to a binary form. The
 * conversion stops at the first non-hex digit character encountered.
 * strdehexize port from SmartBasic to C.
 *
 * @param[in] ptr - the string to convert
 *
 * @retval The number of ASCII byte pairs converted.
 *****************************************************************************/
uint8_t ez_str_strdehexize(uint8_t *ptr)
{
  uint32_t length;
  uint32_t loop;
  char control_string[2];
  uint8_t dehexized_length = 0;

  if (ptr != NULL)
  {
    length = strlen((char *)ptr);

    loop = 0;
    while (loop < length)
    {
      if (isxdigit((int)(*(ptr + loop))) &&
          isxdigit((int)(*(ptr + (loop + 1)))))
      {
        /* Extract the hex value */
        control_string[0] = *(ptr + loop);
        control_string[1] = *(ptr + (loop + 1));

        *(ptr + loop) = (uint8_t)strtol(control_string, NULL, BASE_HEX);

        /* Now cleanup the string. */
        length = cleanup_string(ptr, loop, length, 1);

        /* Another pair of ASCII bytes converted */
        dehexized_length++;
      }
      else
      {
        /* If the digits are not hex-decimal, then stop the loop. */
        *(ptr + loop) = ASCII_NULL;
        break;
      }

      loop++;
    }
  }
  return dehexized_length;
}

/******************************************************************************
 * This decodes a hex string
 *
 * @param[out] out - the resulting binary data
 * @param[in] in - the string to convert
 * @param[in] len - the length of the input string
 *
 * @retval none
 *****************************************************************************/
void ez_str_hex_decode(uint8_t *out, uint8_t *in, uint32_t len)
{
    uint32_t pos = 0;
    while (pos < len)
    {
        out[pos/2] = (in[pos] - (in[pos] >= 'a' && in[pos] <= 'f' ? \
            HEX_DECODE_LOWER_CASE_ALPHA_SUBTRACT : (in[pos] >= 'A' && \
                in[pos] <= 'F' ? \
                    HEX_DECODE_UPPER_CASE_ALPHA_SUBTRACT : \
                    HEX_DECODE_NUMERIC_SUBTRACT))) * 16;
        out[pos/2] += (in[pos+1] - (in[pos+1] >= 'a' && in[pos+1] <= 'f' ? \
            HEX_DECODE_LOWER_CASE_ALPHA_SUBTRACT : (in[pos+1] >= 'A' && \
                in[pos+1] <= 'F' ? \
                    HEX_DECODE_UPPER_CASE_ALPHA_SUBTRACT : \
                    HEX_DECODE_NUMERIC_SUBTRACT)));
        pos+=2;
    }
}

/******************************************************************************
 * This converts a uint8_t to a 2 digit decimal string
 *
 * @param[in] uint8_t in - the value to convert to a string
 * @param[out] uint8_t *out - the resulting string (not null terminated)
 *
 * @retval none
 *****************************************************************************/
void ez_str_decimal_to_2_digit_str(uint8_t in, char *out)
{
  uint8_t tens = 0;
  uint8_t ones = 0;

  if ((in <= DEC_2_DIGIT_MAX) && (out != NULL))
  {
    tens = in / DEC_TENS_DIVISOR;
    ones = in % DEC_TENS_DIVISOR;
    out[DEC_DIGIT_1_IDX] = tens + ASCII_ZERO;
    out[DEC_DIGIT_2_IDX] = ones + ASCII_ZERO;
  }

  return;
}

/******************************************************************************
 * This converts a 128-bit UUID string to a binary 128-bit UUID
 *
 * @param[out] out - the resulting binary UUID
 * @param[in] in - the UUID string to convert
 * @param[in] len - the length of the input string
 *
 * @retval sc - the status of the operation
 *****************************************************************************/
sl_status_t ez_str_string_to_uuid128(uint8_t *out, uint8_t *in, uint32_t len)
{
  sl_status_t sc = SL_STATUS_FAIL;

  if (len == UUID128_STR_LEN)
  {
      ez_str_hex_decode(out, in, len);
      sc = SL_STATUS_OK;
  }
  else
  {
      app_log_error("failed to decode a 128-bit UUID\r\n");
  }

  return sc;
}

/****************************************************************************
 * Determine if the given string is all decimal numbers
 * @param in_string - the string to check.
 ****************************************************************************/
bool ez_str_is_decimal_digits(char *in_string)
{
  while (*in_string) {
    /* isdigit expects an int containing a char */
    int test_char = (int)(*in_string);
    if (isdigit (test_char) == 0) {
      return false;
    }
    in_string++;
  }
  return true;
}

/****************************************************************************
 * Determine if the given string is all hex numbers
 * @param in_string - the string to check.
 ****************************************************************************/
bool ez_str_is_hex_digits(char *in_string)
{
  while (*in_string) {
    /* isxdigit expects an int containing a char */
    int test_char = (int)(*in_string);
    if (isxdigit (test_char) == 0) {
      return false;
    }
    in_string++;
  }
  return true;
}

/****************************************************************************
 * Determine if the given string is all decimal numbers
 * @param in_string - the string to check.
 ****************************************************************************/
bool ez_str_is_signed_decimal_digits(char *in_string)
{
  while (*in_string) {
    /* isdigit expects an int containing a char */
    int test_char = (int)(*in_string);
    if (isdigit (test_char) == 0 && test_char != '-') {
      return false;
    }
    in_string++;
  }
  return true;
}

/****************************************************************************
 * Convert a single byte to 2 printable characters
 * @param out - pointer to output will change * and * + 1
 * @param data - byte to convert
 ****************************************************************************/
void ez_str_bin_to_hex_single(uint8_t *out, uint8_t data)
{
  /* Convert upper 4 bits */
  *out = hex_conversion[(data & TOP_NYBBLE_MASK) >> TOP_NYBBLE_SHIFT];
  /* convert lower 4 bits */
  *(out + 1) = hex_conversion[(data & BOTTOM_NYBBLE_MASK)];
}

/****************************************************************************
 * Dump hex to printable string no \ etc.
 * @param[in] input - Pointer to standard uint8_t buffer
 * @param[out] output - Output buffer - assumed to be big enough
 * @param[in] len - length of input buffer
 *
 ****************************************************************************/
void ez_str_bin_to_hex_dump_std(uint8_t *input, uint8_t *output, size_t len)
{
  uint8_t out_index = 0;
  /* Loop through the input data */
  if (input != NULL)
  {
    for (uint8_t in_index = 0; in_index < len; in_index++) {
      /* Get the byte */
      uint8_t data = input[in_index];
      /* Convert upper 4 bits */
      output[out_index] = hex_conversion[(data & TOP_NYBBLE_MASK) >>
                                         TOP_NYBBLE_SHIFT];
      out_index++;
      /* convert lower 4 bits */
      output[out_index] = hex_conversion[(data & BOTTOM_NYBBLE_MASK)];
      out_index++;
    }
  }
  else
  {
    memset(output, 0, len);
    app_log_error("bin to hex string conversion failed due to a null input pointer\r\n");
  }
}

/****************************************************************************
 * Find the position of the first instance of str2 in str1 starting at a
 * specified starting point in str1.
 *
 * @param str1        - the string to be searched.
 * @param str2        - the sub string to be searched for (MUST be NULL
 *                      terminated).
 * @param startpos    - the starting position in str1.
 * @param length_str1 - the length of str1 (it could contain nulls).
 *
 * @return The start position of the sub-string appearing in the string to
 *         be searched.
 ****************************************************************************/
int ez_str_pos(char *str, char *str_sub, int start_pos, int length_str)
{
  int i;
  int j;
  int temp_i;
  int length_substr = strlen(str_sub);
  int return_value = STR_NOT_FOUND;

  /* This function does not work with very small strings. */
  if (strcmp(str, str_sub) == 0)
  {
    return_value = 0;
  }
  else
  {
    for (i = start_pos; i < length_str - length_substr; i++)
    {
      temp_i = i;
      for (j = 0; j < length_substr; j++)
      {
        if (str[temp_i] == str_sub[j])
        {
          temp_i++;
          if (j == length_substr - 1)
          {
            /* Check here if we're searching for a single character string */
            if ((temp_i - length_substr) == 0)
            {
              return_value = temp_i;
            }
            else
            {
              /* If not we offset back again to the start of the string */
              return_value = temp_i - length_substr;
            }
            break;
          }
        }
        else
        {
          break;
        }
      }
    }
  }

  return return_value;
}

/****************************************************************************
 * The string pointed to by ptr the consists of the leftmost length
 * characters of the string pointer to by str.
 * This is a port of the SmartBasic STRSPLITLEFT$ command.
 * @param str     - Pointer to initial string.
 * @param length  - The number of left most characters.
 * @param ptr     - The pointer to the initial string with the left most
 *                  characters removed.
 ****************************************************************************/
void ez_str_split_left(const char *str, int length, char *ptr)
{
  int len;

  if (str != NULL)
  {
    /* Make sure that the string returned is of the correct size. */
    if ((int)strlen(str) >= length)
    {
      len = length;
    }
    else
    {
      len = 0;
    }

    strncpy(ptr, str, len);
    *(ptr + len) = '\0';
  }
  else
  {
    ptr = NULL;
  }
}

/****************************************************************************
 * Convert a string to all lower case characters.
 *
 * @param str - The string that is to be converted to all lower case characters.
 *              This also holds the result.
 ****************************************************************************/
void ez_str_to_lower(char *str)
{
  for (size_t i = 0; i < strlen(str); ++i)
  {
    str[i] = (char)tolower((unsigned char)str[i]);
  }
}

/****************************************************************************
 * Shift the specified string to the left by the specified number of characters.
 *
 * @param str  - The string that is to be processed.
 *               This also holds the result.
 *        drop - The number of characters that the string is to be shifted to
 *               the left.
 ****************************************************************************/
void ez_str_shift_left(char *cmd_str, int drop)
{
  int i = 0;
  int end_pos;

  end_pos = strlen(cmd_str);

  while ((i + drop) <= end_pos)
  {
    *(cmd_str + i) = *(cmd_str + i + drop);
    i++;
  }
}

/****************************************************************************
 * Return a single character at position i within string str.
 * Port of the SmartBASIC STRGETCHR command.
 *
 * @param str  - The string from which the character is to be extracted.
 * @param i    - The position of the character.
 *
 * @return The ASCII value of the character at position i within string str.
 ****************************************************************************/
int ez_str_get_chr(char *str, int i)
{
  int length;
  int ret_value;

  length = strlen(str);

  if (i <= length)
  {
    ret_value = *(str + i);
  }
  else
  {
    ret_value = STR_NOT_FOUND;
  }

  return ret_value;
}

/****************************************************************************
 * Drop (remove) all the white space characters from a string.
 * N.B. A white space character is one with a ASCII value of 0x20 or less.
 *
 * @param cmdstr - The string that is to be processed.
 *                 This also holds the result.
 ****************************************************************************/
void ez_drop_white_space(char *cmd_str, int pos)
{
  int tmpDta;

  tmpDta = ez_str_get_chr(cmd_str, pos);

  while ((tmpDta >= 0) && (tmpDta <= CHR_ASCII_SPACE))
  {
    pos++;
    tmpDta = (int)*(cmd_str + pos);
  }

  ez_str_shift_left(cmd_str, pos);
}
