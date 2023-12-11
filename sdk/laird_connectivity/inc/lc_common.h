/*******************************************************************************
  Filename:       lc_common.h
  Revised:        11/12/2023
  Revision:       1.0

  Description:    Common global definitions

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
#ifndef LC_COMPONENTS_HDR_LC_COMMON_H_
#define LC_COMPONENTS_HDR_LC_COMMON_H_

#include <app_log.h>

/* Output prefix for at+linkc (link check) */
#define LC_RESPONSE_PREFIX "\nLC: "

/* Text output upon wake up */
#define LC_WAKE_UP "\nWAKE\r"

/* Wake command text */
#define LC_WAKE_UP_COMMAND "AT+WAKE"

/* Text used to spoof an AT+WAKE request */
#define LC_WAKE_UP_REQUEST "AT+WAKE\r"

/* Maximum Lora message buffer requirement */
#define MAX_LORA_MESSAGE_BUFFER_SIZE 530

/* Used to block unused variable warnings */
#define ARG_UNUSED(x) (void)(x)

/* Word manipulation */
#define MSB_16(a) (((a)&0xFF00) >> 8)
#define LSB_16(a) ((a)&0x00FF)

/* Time conversion */
#define SEC_TO_MS(x) (x * 1000)
#define MS_TO_SEC(x) (x / 1000)

/* The number of digits needed to represent the maximum number of registers.
 * This covers 0 - 999.
 */
#define MAX_SIZE_OF_REGISTERS_AS_DIGITS 3

/* Radix for hex string to value conversion */
#define BASE_HEX 16

/* Maximum length of a LORA message in the message buffer */
#define LORA_MESSAGE_MAX_BUF_SIZE 255

/* This is the value of a register index when it is not set, or something went
 * wrong with its extraction from a command string.
 */
#define REGISTER_INDEX_NOT_SET -1

/* This is the id of the Semtech stack, always set to 0 */
#define STACK_ID 0

/* Debug macro enabling */
#if defined(LC_DEBUG) && LC_DEBUG == 1
  #define AT_DEBUG
  #define ERROR_LOG(...) app_log_error("%s: ", __FUNCTION__); app_log_error(__VA_ARGS__)
  #define INFO_LOG(...) app_log_info("%s: ", __FUNCTION__); app_log_info(__VA_ARGS__)
#else
  #define ERROR_LOG(...)
  #define INFO_LOG(...)
#endif

#endif /* LC_COMPONENTS_HDR_LC_COMMON_H_ */
