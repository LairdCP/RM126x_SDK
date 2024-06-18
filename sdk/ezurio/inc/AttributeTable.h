/*******************************************************************************
  Filename:       AttributeTable.h
  Revised:        08/12/2023
  Revision:       1.0

  Description:    Access point for the attribute functionality

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
 
  THIS SOFTWARE IS PROVIDED BY EZURIO LLC "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. TO THE MAXIMUM EXTENT ALLOWED BY LAW, IN NO EVENT SHALL EZURIO LLC
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/

#ifndef __ATTRIBUTE_TABLE_H__
#define __ATTRIBUTE_TABLE_H__

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Global Constants, Macros and Type Definitions                              */
/******************************************************************************/
typedef size_t attr_idx_t;

typedef enum {
  ATTR_TYPE_UNKNOWN = 0,
  ATTR_TYPE_BOOL,
  ATTR_TYPE_U8,
  ATTR_TYPE_U16,
  ATTR_TYPE_U32,
  ATTR_TYPE_U64,
  ATTR_TYPE_S8,
  ATTR_TYPE_S16,
  ATTR_TYPE_S32,
  ATTR_TYPE_S64,
  ATTR_TYPE_FLOAT,
  ATTR_TYPE_STRING,
  ATTR_TYPE_ANY
} AttrType_t;

#define ATTR_TYPE_BYTE_ARRAY ATTR_TYPE_STRING

typedef struct minmax {
  union {
    uint32_t ux;
    int32_t sx;
    float fx;
  };
} minmax_t;

typedef struct AttributeEntry AttributeEntry_t;

struct AttributeEntry {
  const char *const name;
  void *pData;
  void const *const pDefault;
  const size_t size;
  const AttrType_t type;
  const bool savable;
  const bool writable;
  const bool readable;
  const bool lockable;
  const bool broadcast;
  const bool deprecated;
  const bool donotdump;
  int (*const pValidator)(AttributeEntry_t *, void *, size_t, bool);
  int (*const pPrepare)(void);
  const minmax_t min;
  const minmax_t max;
  const uint32_t bl654_register;
  bool modified;
};


/* pystart - attribute table size */
#define ATTR_TABLE_SIZE 59
#define CORE_ATTR_TABLE_SIZE 1
/* pyend */

/* The code generator tool should generate this */
#define ATTR_MAX_STR_LENGTH 50

#define ATTR_MAX_STR_SIZE (ATTR_MAX_STR_LENGTH + 1)

#define ATTR_MAX_HEX_SIZE 8

#define ATTR_MAX_VERSION_LENGTH 11

#define MAX_ATTR_VALUE 9999
#define MAX_ATTR_VALUE_DIGITS 4       /* Maximum number of digits that an
                                       * attribute can be, this covers the range
                                       * 0-9999.
                                       */

#define MAX_ATTRIBUTE_LENGTH 33       /* The maximum length of a non-numeric
                                       * attribute (bytes).
                                       */

/******************************************************************************/
/* Indices                                                                    */
/******************************************************************************/
/* clang-format off */
/* pystart - attribute indices */
#define ATTR_INDEX_lora_modem_dm_port                    0
#define ATTR_INDEX_lora_modem_upload_sctr                1
#define ATTR_INDEX_lora_modem_app_key_crc_status         2
#define ATTR_INDEX_lora_modem_app_key_crc                3
#define ATTR_INDEX_lora_modem_rfu_1                      4
#define ATTR_INDEX_lora_modem_rfu_2                      5
#define ATTR_INDEX_lora_modem_rfu_3                      6
#define ATTR_INDEX_lora_mac_adr_custom                   7
#define ATTR_INDEX_lora_mac_region_type                  8
#define ATTR_INDEX_lora_mac_certification_enabled        9
#define ATTR_INDEX_lora_mac_rfu_1                        10
#define ATTR_INDEX_lora_mac_rfu_2                        11
#define ATTR_INDEX_lora_mac_rfu_3                        12
#define ATTR_INDEX_lora_mac_rfu_4                        13
#define ATTR_INDEX_lora_mac_rfu_5                        14
#define ATTR_INDEX_lora_mac_rfu_6                        15
#define ATTR_INDEX_lora_mac_rfu_7                        16
#define ATTR_INDEX_lora_mac_rfu_8                        17
#define ATTR_INDEX_lora_mac_rfu_9                        18
#define ATTR_INDEX_lora_mac_rfu_10                       19
#define ATTR_INDEX_lora_mac_rfu_11                       20
#define ATTR_INDEX_lora_mac_crc                          21
#define ATTR_INDEX_lora_devnonce_devnonce                22
#define ATTR_INDEX_lora_devnonce_nb_reset                23
#define ATTR_INDEX_lora_devnonce_join_nonce              24
#define ATTR_INDEX_lora_devnonce_rfu_1                   25
#define ATTR_INDEX_lora_devnonce_rfu_2                   26
#define ATTR_INDEX_lora_devnonce_rfu_3                   27
#define ATTR_INDEX_lora_devnonce_crc                     28
#define ATTR_INDEX_lora_modem_crc                        29
#define ATTR_INDEX_lora_se_deveui                        30
#define ATTR_INDEX_lora_se_joineui                       31
#define ATTR_INDEX_lora_se_pin                           32
#define ATTR_INDEX_lora_se_key_1                         33
#define ATTR_INDEX_lora_se_key_2                         34
#define ATTR_INDEX_lora_se_key_3                         35
#define ATTR_INDEX_lora_se_key_4                         36
#define ATTR_INDEX_lora_se_key_5                         37
#define ATTR_INDEX_lora_se_key_6                         38
#define ATTR_INDEX_lora_se_key_7                         39
#define ATTR_INDEX_lora_se_key_8                         40
#define ATTR_INDEX_lora_se_key_9                         41
#define ATTR_INDEX_lora_se_key_10                        42
#define ATTR_INDEX_lora_se_key_11                        43
#define ATTR_INDEX_lora_se_key_12                        44
#define ATTR_INDEX_lora_se_key_13                        45
#define ATTR_INDEX_lora_se_key_14                        46
#define ATTR_INDEX_lora_se_key_15                        47
#define ATTR_INDEX_lora_se_key_16                        48
#define ATTR_INDEX_lora_se_key_17                        49
#define ATTR_INDEX_lora_se_key_18                        50
#define ATTR_INDEX_lora_se_key_19                        51
#define ATTR_INDEX_lora_se_key_20                        52
#define ATTR_INDEX_lora_se_key_21                        53
#define ATTR_INDEX_lora_se_key_22                        54
#define ATTR_INDEX_lora_se_key_23                        55
#define ATTR_INDEX_lora_se_key_map_1                     56
#define ATTR_INDEX_lora_se_key_map_2                     57
#define ATTR_INDEX_lora_se_crc                           58

#define CORE_ATTR_INDEX_device_id                             0
/* pyend */
/* clang-format on */

/******************************************************************************/
/* Global Function Prototypes                                                 */
/******************************************************************************/
/**
 * Set non-backup values to default.
 *
 * @param none
 *
 * @retval none
 */
void AttributeTable_FactoryReset(void);

/**
 * @param DoWrite true if attribute should be changed, false if pValue should
 * be validated but not written.
 *
 * @retval Validators return negative error code, 0 on success
 */
int attribute_validator_string(AttributeEntry_t *pEntry, void *pValue,
			       size_t Length, bool DoWrite);
int attribute_validator_uint32(AttributeEntry_t *pEntry, void *pValue,
			       size_t Length, bool DoWrite);
int attribute_validator_uint16(AttributeEntry_t *pEntry, void *pValue,
			       size_t Length, bool DoWrite);
int attribute_validator_bool(AttributeEntry_t *pEntry, void *pValue,
			     size_t Length, bool DoWrite);
int attribute_validator_uint8(AttributeEntry_t *pEntry, void *pValue,
			      size_t Length, bool DoWrite);
int attribute_validator_int32(AttributeEntry_t *pEntry, void *pValue,
			      size_t Length, bool DoWrite);
int attribute_validator_int16(AttributeEntry_t *pEntry, void *pValue,
			      size_t Length, bool DoWrite);
int attribute_validator_int8(AttributeEntry_t *pEntry, void *pValue,
			     size_t Length, bool DoWrite);
int attribute_validator_float(AttributeEntry_t *pEntry, void *pValue,
           size_t Length, bool DoWrite);

/* The application must provide implementations for these. */
/* pystart - prepare for read */
int AttributePrepare_device_id(void);
/* pyend */

/* The application must provide implementations for these. */
/* pystart - validators */
/* pyend */

#ifdef __cplusplus
}
#endif

#endif /* __ATTRIBUTE_TABLE_H__ */
