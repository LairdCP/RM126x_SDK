/*******************************************************************************
  Filename:       Attribute.h
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

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "AttributeTable.h"
#include "nvm3.h"
#include "sl_status.h"

/******************************************************************************/
/* Global Constants, Macros and Type Definitions                              */
/******************************************************************************/
/* Maximum number of data objects saved                                       */
/* No. of attributes (see AttributeTable.h)                                   */
#define MAX_OBJECT_COUNT    ATTR_TABLE_SIZE

/* Max and min keys for data objects */
#define MIN_DATA_KEY  NVM3_KEY_MIN
#define MAX_DATA_KEY  (MIN_DATA_KEY + ATTR_TABLE_SIZE - 1)

/* Key of write counter object */
#define WRITE_COUNTER_KEY   MAX_OBJECT_COUNT

/* Use the default nvm3 handle from nvm3_default.h */
#define NVM3_DEFAULT_HANDLE nvm3_defaultHandle

/* If a register is not found, then the register translation function returns
 * this value.
 */
#define REG_NOT_FOUND -1

/* Possible return codes. */
#define EOK 0    /* The operation worked. */
#define EPERM 1  /* A permissions error has occurred. */
#define EINVAL 2 /* The value is invalid. */

/* Table selection enum. */
typedef enum
{
  SELECT_ATTRIBUTE_TABLE = 0,
  SELECT_CORE_ATTRIBUTE_TABLE
} e_table_selection;

/******************************************************************************/
/* Function Definitions                                                       */
/******************************************************************************/

/**
 * @brief Read attributes from flash
 *
 * @retval negative error code, 0 on success
 */
int Attribute_Init(void);

/**
 * @brief Set values to default (except items configured during production).
 *
 * @retval negative error code, 0 on success
 */
int Attribute_FactoryReset(void);

/**
 * @brief Get the type of the attribute
 *
 * @param Index A valid index into an attribute table.
 * @param Which attribute table is to be referred to.
 *
 * @retval type of variable
 */
AttrType_t Attribute_GetType(attr_idx_t Index, e_table_selection table);

/**
 * @brief Helper function
 *
 * @param Index A valid index into an attribute table.
 * @param Which attribute table is to be referred to.
 *
 * @retval true if index is valid, false otherwise
 */
bool Attribute_ValidIndex(attr_idx_t Index, e_table_selection table);

/**
 * @brief Set value.
 *
 * @param Index A valid index into an attribute table.
 * @param Type the type of attribute
 * @param pValue pointer representation of variable
 * @param ValueLength The length (without null char) of the string
 * being passed in.  If the value isn't a string, then the length is
 * not used.
 * @param modified Will be set to true if value was updated (different than the
 * existing value). Can safely be supplied NULL to ignore
 * @param Which attribute table is to be referred to.
 *
 * @retval negative error code, 0 on success
 */
int Attribute_Set(attr_idx_t Index, AttrType_t Type, void *pValue,
    size_t ValueLength, bool *modified, e_table_selection table);

/**
 * @brief Default value of an attribute. This function will find the default
 * value and return based on the index.
 *
 * @param Index A valid index into an attribute table.
 * @param pValue pointer to the default value
 * @param ValueLength is the size of pValue.
 * @param Which attribute table is to be referred to.
 *
 * @retval negative error code, size of value on return
 */
int Attribute_GetDefault(attr_idx_t Index, void *pValue, size_t ValueLength,
                         e_table_selection table);

/**
 * @brief Copy an attribute.
 *
 * @param Index A valid index into an attribute table.
 * @param pValue pointer to location to copy string
 * @param ValueLength is the size of pValue.
 * @param Which attribute table is to be referred to.
 *
 * @retval negative error code, size of value on return
 */
int Attribute_Get(attr_idx_t Index, void *pValue, size_t ValueLength,
                  e_table_selection table);

/**
 * @brief Get the name of an attribute
 *
 * @param Index a valid index into a table
 * @param Which attribute table is to be referred to.
 *
 * @retval empty string if not found
 */
const char *Attribute_GetName(attr_idx_t Index, e_table_selection table);

/**
 * @brief Get the size of an attribute
 *
 * @param Index a valid index into a table
 * @param Which attribute table is to be referred to.
 *
 * @param size of attribute, size with null if string
 */
size_t Attribute_GetSize(attr_idx_t Index, e_table_selection table);

/**
 * @brief Get the minimum size of a string attribute
 *
 * @param Index a valid index into a table
 * @param Which attribute table is to be referred to.
 *
 * @retval The minimum size of the field in bytes
 */
uint32_t Attribute_GetMinSize_String(attr_idx_t Index, e_table_selection table);

/**
 * @brief Get the maximum size of a string attribute
 *
 * @param Index a valid index into a table
 * @param Which attribute table is to be referred to.
 *
 * @retval The maximum size of the field in bytes
 */
uint32_t Attribute_GetMaxSize_String(attr_idx_t Index, e_table_selection table);

/**
 * @brief Get the minimum size of a numeric attribute
 *
 * @param index - a valid index into a table
 * @param table - attribute table to be referred to
 * @param data - the output data
 *
 * @retval The Silabs based result code
 */
sl_status_t Attribute_GetMinSize_Numeric(attr_idx_t index,
                                          e_table_selection table,
                                          int64_t *data);

/**
 * @brief Get the maximum size of a numeric attribute
 *
 * @param index - a valid index into a table
 * @param table - attribute table to be referred to
 * @param data - the output data
 *
 * @retval The Silabs based result code
 */
sl_status_t Attribute_GetMaxSize_Numeric(attr_idx_t index,
                                          e_table_selection table,
                                          int64_t *data);

/**
 * @brief Commits all NV based Attributes to NV storage
 *
 * @retval The Silabs Ecode_t based result code
 */
Ecode_t Attribute_SaveAll(void);

/**
 * @brief Converts a numeric array into the string equivalent.
 *
 * @param register_index - The register index.
 * @param attribute - Output buffer for string data.
 * @param attribute_length - Optional output buffer length.
 * @param table - The table where the parameter resides.
 */
void Attribute_Convert_Numeric_To_String(int32_t register_index,
                                        char *attribute,
                                        size_t *attribute_length,
                                        e_table_selection table);

/**
 * @brief Returns a pointer to the indexed attribute and table item.
 *
 * @param index - The attribute index.
 * @param table - The table where the attribute resides.
 * @return NULL if not found, the attribute pointer otherwise.
 */
AttributeEntry_t *attr_map(attr_idx_t index, e_table_selection table);
#ifdef __cplusplus
}
#endif

#endif /* __ATTRIBUTE_H__ */
