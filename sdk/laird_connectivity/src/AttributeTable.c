/*******************************************************************************
  Filename:       AttributeTable.c
  Revised:        08/12/2023
  Revision:       1.0

  Description:    Access point for the attribute functionality

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

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "app.h"
#include "app_version.h"
#include "AttributeTable.h"

/******************************************************************************/
/* Local Constant, Macro and Type Definitions                                 */
/******************************************************************************/
/* pystart - default defines */
#define DRW DEFAULT_RW_ATTRIBUTE_VALUES
#define DRO DEFAULT_RO_ATTRIBUTE_VALUES

#define CORE_DRW CORE_DEFAULT_RW_ATTRIBUTE_VALUES
#define CORE_DRO CORE_DEFAULT_RO_ATTRIBUTE_VALUES
/* pyend */

/* clang-format off */
#define b   ATTR_TYPE_BOOL
#define u8  ATTR_TYPE_U8
#define u16 ATTR_TYPE_U16
#define u32 ATTR_TYPE_U32
#define u64 ATTR_TYPE_U64
#define i8  ATTR_TYPE_S8
#define i16 ATTR_TYPE_S16
#define i32 ATTR_TYPE_S32
#define i64 ATTR_TYPE_S64
#define f   ATTR_TYPE_FLOAT
#define s   ATTR_TYPE_STRING
#define a   ATTR_TYPE_STRING
/* clang-format on */

#define STR_INITIALISER(x) #x

#define SX1261_POWER_TX_MIN           -17
#define SX1261_POWER_TX_MAX           15
#define SX1262_POWER_TX_MIN           -10
#define SX1262_POWER_TX_MAX           22

#ifdef SEMTECH_RADIO_SX1261
#define MIN_TX_POWER SX1261_POWER_TX_MIN
#define MAX_TX_POWER SX1261_POWER_TX_MAX
#define DEFAULT_TX_POWER SX1261_POWER_TX_MAX
#define DEFAULT_MODULE_TYPE 0
#define DEFAULT_DEVICE_ID "RM1261"
#else
#define MIN_TX_POWER SX1262_POWER_TX_MIN
#define MAX_TX_POWER SX1262_POWER_TX_MAX
#define DEFAULT_TX_POWER SX1262_POWER_TX_MAX
#define DEFAULT_MODULE_TYPE 1
#define DEFAULT_DEVICE_ID "RM1262"
#endif

/* Add things to the end!  Do not remove items. Change them to deprecated. */
/* pystart - rw attributes */
typedef struct RwAttributesTag {
    uint16_t lora_modem_dm_port;
    uint16_t lora_modem_upload_sctr;
    uint8_t lora_modem_app_key_crc_status;
    uint32_t lora_modem_app_key_crc;
    uint32_t lora_modem_rfu_1;
    uint32_t lora_modem_rfu_2;
    uint32_t lora_modem_rfu_3;
    uint32_t lora_mac_adr_custom;
    uint8_t lora_mac_region_type;
    uint8_t lora_mac_certification_enabled;
    uint8_t lora_mac_rfu_1;
    uint8_t lora_mac_rfu_2;
    uint8_t lora_mac_rfu_3;
    uint8_t lora_mac_rfu_4;
    uint8_t lora_mac_rfu_5;
    uint8_t lora_mac_rfu_6;
    uint8_t lora_mac_rfu_7;
    uint8_t lora_mac_rfu_8;
    uint8_t lora_mac_rfu_9;
    uint8_t lora_mac_rfu_10;
    uint8_t lora_mac_rfu_11;
    uint32_t lora_mac_crc;
    uint16_t lora_devnonce_devnonce;
    uint32_t lora_devnonce_nb_reset;
    char lora_devnonce_join_nonce[12 + 1];
    uint32_t lora_devnonce_rfu_1;
    uint32_t lora_devnonce_rfu_2;
    uint32_t lora_devnonce_rfu_3;
    uint32_t lora_devnonce_crc;
    uint32_t lora_modem_crc;
    char lora_se_deveui[16 + 1];
    char lora_se_joineui[16 + 1];
    char lora_se_pin[8 + 1];
    char lora_se_key_1[32 + 1];
    char lora_se_key_2[32 + 1];
    char lora_se_key_3[32 + 1];
    char lora_se_key_4[32 + 1];
    char lora_se_key_5[32 + 1];
    char lora_se_key_6[32 + 1];
    char lora_se_key_7[32 + 1];
    char lora_se_key_8[32 + 1];
    char lora_se_key_9[32 + 1];
    char lora_se_key_10[32 + 1];
    char lora_se_key_11[32 + 1];
    char lora_se_key_12[32 + 1];
    char lora_se_key_13[32 + 1];
    char lora_se_key_14[32 + 1];
    char lora_se_key_15[32 + 1];
    char lora_se_key_16[32 + 1];
    char lora_se_key_17[32 + 1];
    char lora_se_key_18[32 + 1];
    char lora_se_key_19[32 + 1];
    char lora_se_key_20[32 + 1];
    char lora_se_key_21[32 + 1];
    char lora_se_key_22[32 + 1];
    char lora_se_key_23[32 + 1];
    char lora_se_key_map_1[32 + 1];
    char lora_se_key_map_2[32 + 1];
    uint32_t lora_se_crc;
} RwAttribute_t;

typedef struct CoreRwAttributesTag {
} CoreRwAttribute_t;
/* pyend */

/* pystart - rw defaults */
static const RwAttribute_t DEFAULT_RW_ATTRIBUTE_VALUES = {
    .lora_modem_dm_port = 0,
    .lora_modem_upload_sctr = 0,
    .lora_modem_app_key_crc_status = 0,
    .lora_modem_app_key_crc = 0,
    .lora_modem_rfu_1 = 0,
    .lora_modem_rfu_2 = 0,
    .lora_modem_rfu_3 = 0,
    .lora_mac_adr_custom = 0,
    .lora_mac_region_type = 0,
    .lora_mac_certification_enabled = 0,
    .lora_mac_rfu_1 = 0,
    .lora_mac_rfu_2 = 0,
    .lora_mac_rfu_3 = 0,
    .lora_mac_rfu_4 = 0,
    .lora_mac_rfu_5 = 0,
    .lora_mac_rfu_6 = 0,
    .lora_mac_rfu_7 = 0,
    .lora_mac_rfu_8 = 0,
    .lora_mac_rfu_9 = 0,
    .lora_mac_rfu_10 = 0,
    .lora_mac_rfu_11 = 0,
    .lora_mac_crc = 0,
    .lora_devnonce_devnonce = 0,
    .lora_devnonce_nb_reset = 0,
    .lora_devnonce_join_nonce = "000000000000",
    .lora_devnonce_rfu_1 = 0,
    .lora_devnonce_rfu_2 = 0,
    .lora_devnonce_rfu_3 = 0,
    .lora_devnonce_crc = 0,
    .lora_modem_crc = 0,
    .lora_se_deveui = "0000000000000000",
    .lora_se_joineui = "0000000000000000",
    .lora_se_pin = "00000000",
    .lora_se_key_1 = "000000000000000000000000000000",
    .lora_se_key_2 = "000000000000000000000000000000",
    .lora_se_key_3 = "000000000000000000000000000000",
    .lora_se_key_4 = "000000000000000000000000000000",
    .lora_se_key_5 = "000000000000000000000000000000",
    .lora_se_key_6 = "000000000000000000000000000000",
    .lora_se_key_7 = "000000000000000000000000000000",
    .lora_se_key_8 = "000000000000000000000000000000",
    .lora_se_key_9 = "000000000000000000000000000000",
    .lora_se_key_10 = "000000000000000000000000000000",
    .lora_se_key_11 = "000000000000000000000000000000",
    .lora_se_key_12 = "000000000000000000000000000000",
    .lora_se_key_13 = "000000000000000000000000000000",
    .lora_se_key_14 = "000000000000000000000000000000",
    .lora_se_key_15 = "000000000000000000000000000000",
    .lora_se_key_16 = "000000000000000000000000000000",
    .lora_se_key_17 = "000000000000000000000000000000",
    .lora_se_key_18 = "000000000000000000000000000000",
    .lora_se_key_19 = "000000000000000000000000000000",
    .lora_se_key_20 = "000000000000000000000000000000",
    .lora_se_key_21 = "000000000000000000000000000000",
    .lora_se_key_22 = "000000000000000000000000000000",
    .lora_se_key_23 = "000000000000000000000000000000",
    .lora_se_key_map_1 = "000000000000000000000000000000",
    .lora_se_key_map_2 = "000000000000000000000000000000",
    .lora_se_crc = 0
};

static const CoreRwAttribute_t CORE_DEFAULT_RW_ATTRIBUTE_VALUES = {
};
/* pyend */

/* pystart - ro attributes */
typedef struct RoAttributesTag {
} RoAttribute_t;

typedef struct CoreRoAttributesTag {
    char device_id[6 + 1];
} CoreRoAttribute_t;
/* pyend */

/* pystart - ro defaults */
static const RoAttribute_t DEFAULT_RO_ATTRIBUTE_VALUES = {
};

static const CoreRoAttribute_t CORE_DEFAULT_RO_ATTRIBUTE_VALUES = {
    .device_id = "RM126X"
};
/* pyend */

/******************************************************************************/
/* Local Data Definitions                                                     */
/******************************************************************************/
/* pystart - data definitions */
static RwAttribute_t rw;
static RoAttribute_t ro;

static CoreRwAttribute_t core_rw;
static CoreRoAttribute_t core_ro;
/* pyend */

/******************************************************************************/
/* Global Data Definitions                                                    */
/******************************************************************************/

/* Expander for string and other values
 *
 *...............name...value...default....size...writable..readable
 */
#define STRINGIFY(x) #x

/* pystart - string defines */
#define RW_ATTRS(n) STRINGIFY(n), rw.n, DRW.n, sizeof(rw.n)
#define RW_ATTRX(n) STRINGIFY(n), &rw.n, &DRW.n, sizeof(rw.n)
#define RO_ATTRS(n) STRINGIFY(n), ro.n, DRO.n, sizeof(ro.n)
#define RO_ATTRX(n) STRINGIFY(n), &ro.n, &DRO.n, sizeof(ro.n)

#define CORE_RW_ATTRS(n) STRINGIFY(n), core_rw.n, CORE_DRW.n, sizeof(core_rw.n)
#define CORE_RW_ATTRX(n) STRINGIFY(n), &core_rw.n, &CORE_DRW.n, sizeof(core_rw.n)
#define CORE_RO_ATTRS(n) STRINGIFY(n), core_ro.n, CORE_DRO.n, sizeof(core_ro.n)
#define CORE_RO_ATTRX(n) STRINGIFY(n), &core_ro.n, &CORE_DRO.n, sizeof(core_ro.n)
/* pyend */

#define y true
#define n false

/* If min == max then range isn't checked. */

/* index.....name............................type.savable.writable.readable.lockable.broadcast.deprecated.donotdumpvalidator..min.max.sreg */
/* clang-format off */
/* pystart - attribute tables */
const AttributeEntry_t attrTable[ATTR_TABLE_SIZE] = {
  [0  ] = { RW_ATTRX(lora_modem_dm_port)            , u16, y, y, y, n, n, n, n, attribute_validator_uint16  , NULL                                      , .min.ux = 0         , .max.ux = 65535     , -1 },
  [1  ] = { RW_ATTRX(lora_modem_upload_sctr)        , u16, y, y, y, n, n, n, n, attribute_validator_uint16  , NULL                                      , .min.ux = 0         , .max.ux = 65535     , -1 },
  [2  ] = { RW_ATTRX(lora_modem_app_key_crc_status) , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [3  ] = { RW_ATTRX(lora_modem_app_key_crc)        , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [4  ] = { RW_ATTRX(lora_modem_rfu_1)              , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [5  ] = { RW_ATTRX(lora_modem_rfu_2)              , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [6  ] = { RW_ATTRX(lora_modem_rfu_3)              , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [7  ] = { RW_ATTRX(lora_mac_adr_custom)           , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [8  ] = { RW_ATTRX(lora_mac_region_type)          , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [9  ] = { RW_ATTRX(lora_mac_certification_enabled), u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [10 ] = { RW_ATTRX(lora_mac_rfu_1)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [11 ] = { RW_ATTRX(lora_mac_rfu_2)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [12 ] = { RW_ATTRX(lora_mac_rfu_3)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [13 ] = { RW_ATTRX(lora_mac_rfu_4)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [14 ] = { RW_ATTRX(lora_mac_rfu_5)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [15 ] = { RW_ATTRX(lora_mac_rfu_6)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [16 ] = { RW_ATTRX(lora_mac_rfu_7)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [17 ] = { RW_ATTRX(lora_mac_rfu_8)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [18 ] = { RW_ATTRX(lora_mac_rfu_9)                , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [19 ] = { RW_ATTRX(lora_mac_rfu_10)               , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [20 ] = { RW_ATTRX(lora_mac_rfu_11)               , u8 , y, y, y, n, n, n, n, attribute_validator_uint8   , NULL                                      , .min.ux = 0         , .max.ux = 255       , -1 },
  [21 ] = { RW_ATTRX(lora_mac_crc)                  , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [22 ] = { RW_ATTRX(lora_devnonce_devnonce)        , u16, y, y, y, n, n, n, n, attribute_validator_uint16  , NULL                                      , .min.ux = 0         , .max.ux = 65535     , -1 },
  [23 ] = { RW_ATTRX(lora_devnonce_nb_reset)        , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [24 ] = { RW_ATTRS(lora_devnonce_join_nonce)      , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 12        , .max.ux = 12        , -1 },
  [25 ] = { RW_ATTRX(lora_devnonce_rfu_1)           , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [26 ] = { RW_ATTRX(lora_devnonce_rfu_2)           , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [27 ] = { RW_ATTRX(lora_devnonce_rfu_3)           , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [28 ] = { RW_ATTRX(lora_devnonce_crc)             , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [29 ] = { RW_ATTRX(lora_modem_crc)                , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 },
  [30 ] = { RW_ATTRS(lora_se_deveui)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 16        , .max.ux = 16        , -1 },
  [31 ] = { RW_ATTRS(lora_se_joineui)               , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 16        , .max.ux = 16        , -1 },
  [32 ] = { RW_ATTRS(lora_se_pin)                   , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 8         , .max.ux = 8         , -1 },
  [33 ] = { RW_ATTRS(lora_se_key_1)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [34 ] = { RW_ATTRS(lora_se_key_2)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [35 ] = { RW_ATTRS(lora_se_key_3)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [36 ] = { RW_ATTRS(lora_se_key_4)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [37 ] = { RW_ATTRS(lora_se_key_5)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [38 ] = { RW_ATTRS(lora_se_key_6)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [39 ] = { RW_ATTRS(lora_se_key_7)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [40 ] = { RW_ATTRS(lora_se_key_8)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [41 ] = { RW_ATTRS(lora_se_key_9)                 , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [42 ] = { RW_ATTRS(lora_se_key_10)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [43 ] = { RW_ATTRS(lora_se_key_11)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [44 ] = { RW_ATTRS(lora_se_key_12)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [45 ] = { RW_ATTRS(lora_se_key_13)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [46 ] = { RW_ATTRS(lora_se_key_14)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [47 ] = { RW_ATTRS(lora_se_key_15)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [48 ] = { RW_ATTRS(lora_se_key_16)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [49 ] = { RW_ATTRS(lora_se_key_17)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [50 ] = { RW_ATTRS(lora_se_key_18)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [51 ] = { RW_ATTRS(lora_se_key_19)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [52 ] = { RW_ATTRS(lora_se_key_20)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [53 ] = { RW_ATTRS(lora_se_key_21)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [54 ] = { RW_ATTRS(lora_se_key_22)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [55 ] = { RW_ATTRS(lora_se_key_23)                , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [56 ] = { RW_ATTRS(lora_se_key_map_1)             , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [57 ] = { RW_ATTRS(lora_se_key_map_2)             , s  , y, y, y, n, n, n, n, attribute_validator_string  , NULL                                      , .min.ux = 32        , .max.ux = 32        , -1 },
  [58 ] = { RW_ATTRX(lora_se_crc)                   , u32, y, y, y, n, n, n, n, attribute_validator_uint32  , NULL                                      , .min.ux = 0         , .max.ux = 4294967295, -1 }
};

const AttributeEntry_t core_attrTable[CORE_ATTR_TABLE_SIZE] = {
  [0  ] = { CORE_RO_ATTRS(device_id)                , s  , n, n, y, n, n, n, n, attribute_validator_string  , AttributePrepare_device_id                , .min.ux = 6         , .max.ux = 6         , -1 }
};
/* pyend */
/* clang-format on */

/******************************************************************************/
/* Global Function Definitions                                                */
/******************************************************************************/
/**
 * @brief Copy defaults (before loading from flash)
 */
void AttributeTable_Initialize(void)
{
/* pystart - initialisers */
  memcpy(&rw, &DRW, sizeof(RwAttribute_t));
  memcpy(&ro, &DRO, sizeof(RoAttribute_t));

  memcpy(&core_rw, &CORE_DRW, sizeof(CoreRwAttribute_t));
  memcpy(&core_ro, &CORE_DRO, sizeof(CoreRoAttribute_t));
/* pyend */
}

/**
 * @brief set non-backup values to default
 */
void AttributeTable_FactoryReset(void)
{
  size_t i = 0;
/* pystart - defaulters */
  for (i = 0; i < ATTR_TABLE_SIZE; i++) {
    memcpy(attrTable[i].pData, attrTable[i].pDefault,
           attrTable[i].size);
  }

  for (i = 0; i < CORE_ATTR_TABLE_SIZE; i++) {
    memcpy(core_attrTable[i].pData, core_attrTable[i].pDefault,
           core_attrTable[i].size);
  }
/* pyend */
}
