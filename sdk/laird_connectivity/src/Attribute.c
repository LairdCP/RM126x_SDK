/*******************************************************************************
  Filename:       Attribute.c
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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "app.h"
#include "app_log.h"
#include "AttributeTable.h"
#include "Attribute.h"
#include "nvm3_default.h"
#include "nvm3_default_config.h"
#include <inttypes.h>

/******************************************************************************/
/* External Function Declarations */
/******************************************************************************/
extern void AttributeTable_Initialize(void);
extern void AttributeTable_FactoryReset(void);

/******************************************************************************/
/* Global Constants, Macros and Type Definitions                              */
/******************************************************************************/
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/******************************************************************************/
/* Global Data Definitions                                                    */
/******************************************************************************/
extern AttributeEntry_t attrTable[ATTR_TABLE_SIZE];
extern AttributeEntry_t core_attrTable[CORE_ATTR_TABLE_SIZE];

/******************************************************************************/
/* Local Data Definitions                                                     */
/******************************************************************************/
static const char EMPTY_STRING[] = "";

/******************************************************************************/
/* Local Function Prototypes                                                  */
/******************************************************************************/
static int Validate(attr_idx_t Index, AttrType_t Type, void *pValue,
                    size_t Length, e_table_selection table);

static int Write(attr_idx_t Index, AttrType_t Type, void *pValue,size_t Length,
                 e_table_selection table);

static int PrepareForRead(attr_idx_t Index, e_table_selection table);
static bool isValid(attr_idx_t Index, e_table_selection table);
static bool isReadable(attr_idx_t Index, e_table_selection table);
static bool isWritable(attr_idx_t Index, e_table_selection table);

static AttributeEntry_t * get_attribute_table(e_table_selection table);

/******************************************************************************/
/* Global Function Definitions                                                */
/******************************************************************************/
int Attribute_Init(void)
{
  int r = -SL_STATUS_FAIL;
  nvm3_ObjectKey_t keys[ATTR_TABLE_SIZE];
  size_t len, objects_count;
  uint32_t type;
  Ecode_t err;
  size_t i;
  bool error_flag = false;
  //AttributeEntry_t *attr_table_entry;

  /* Load the default values. */
  AttributeTable_Initialize();

  /* Load the attributes that have been saved to FLASH. */
  err = nvm3_initDefault();

  if (err != ECODE_NVM3_OK)
  {
      r = -SL_STATUS_FAIL;
  }
  else
  {
      /* Retrieve the keys of stored data. */
      objects_count = nvm3_enumObjects(NVM3_DEFAULT_HANDLE,
                                       (uint32_t *)keys,
                                       sizeof(keys) / sizeof(keys[0]),
                                       MIN_DATA_KEY,
                                       MAX_DATA_KEY);

      if (objects_count == 0)
      {
          r = SL_STATUS_OK;
      } else {
            /* Load the values of the attributes that have been saved to
             * FLASH.
             */
            for (i = 0; i < objects_count; i++)
            {
                nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, keys[i], &type, &len);

                if ( (type == NVM3_OBJECTTYPE_DATA) && (len ==
                    attrTable[keys[i]].size) )
                {
                    err = nvm3_readData(NVM3_DEFAULT_HANDLE, keys[i],
                                        attrTable[keys[i]].pData, len);

                    if (err == ECODE_NVM3_OK)
                    {
                        r = SL_STATUS_OK;
                    }
                    else
                    {
                        /* The read from FLASH had a problem, so reset the
                         * value of the attribute to its default value. */
                        memcpy(attrTable[keys[i]].pData,
                               attrTable[keys[i]].pDefault,
                               attrTable[keys[i]].size);

                        error_flag = true;
                    }
                }
                else
                {
                    /* If the type is not of type object data or the size does
                     * not match the value expected for that parameter, then
                     * flag the error and move onto the next entry that was
                     * saved (if there is one.)
                     */
                    error_flag = true;
                }
            }

            /* Check that all the objects were read with out errors. */
            if (error_flag == true)
            {
                r = -SL_STATUS_FAIL;
            }
        }
        /* Commit updated / added attributes to flash */
        if ((error_flag == true) || (objects_count != ATTR_TABLE_SIZE))
        {
          Attribute_SaveAll();
        }
  }

  /* Now initialise the attributes that are in the core (used for temp storage).
   */
  for (i = 0; i < CORE_ATTR_TABLE_SIZE; i++)
  {
      memcpy(core_attrTable[i].pData,
             core_attrTable[i].pDefault,
             core_attrTable[i].size);
  }

  return r;
}

int Attribute_FactoryReset(void)
{
  int r = -SL_STATUS_FAIL;
  Ecode_t err;
  size_t i;
  bool error_flag = false;

  AttributeTable_FactoryReset();

  /* Save the default values to FLASH. */
  i = 0;

  while ((i < ATTR_TABLE_SIZE) && (error_flag == false))
  {
    if (attrTable[i].savable == true)
    {
      err = nvm3_writeData(NVM3_DEFAULT_HANDLE,
                           i,
                           (unsigned char *)attrTable[i].pDefault,
                           attrTable[i].size);

      if (err != ECODE_NVM3_OK)
      {
        error_flag = true;
      }
    }

    i++;
  }

  /* Now check if any errors occurred while saving values to the FLASH. */
  if (error_flag == true)
  {
      r = -SL_STATUS_FAIL;
  }

  return r;
}

AttrType_t Attribute_GetType(attr_idx_t Index, e_table_selection table)
{
  AttrType_t return_value = ATTR_TYPE_UNKNOWN;
  AttributeEntry_t  *attr_ptr = NULL;


  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {
    return_value = attr_ptr[Index].type;
  }

  return return_value;
}

bool Attribute_ValidIndex(attr_idx_t Index, e_table_selection table)
{
  return isValid(Index, table);
}

int Attribute_Set(attr_idx_t Index, AttrType_t Type, void *pValue,
        size_t ValueLength, bool *modified, e_table_selection table)
{
  int r = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(Index, table)) {
    if (isWritable(Index, table)) {
      r = Validate(Index, Type, pValue, ValueLength, table);
      if (r == 0) {
        r = Write(Index, Type, pValue, ValueLength, table);

        if (modified != NULL) {
            attr_ptr = get_attribute_table(table);

            if (attr_ptr != NULL)
            {
              *modified = attr_ptr[Index].modified;
            }

        }
      }
    }
  }

  return r;
}

int Attribute_GetDefault(attr_idx_t Index, void *pValue, size_t ValueLength,
                         e_table_selection table)
{
  memset(pValue, 0, ValueLength);
  size_t size = -SL_STATUS_INVALID_INDEX;
  int r = -SL_STATUS_INVALID_INDEX;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(Index, table)) {

    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      size = MIN(attr_ptr[Index].size, ValueLength);
      memcpy(pValue, attr_ptr[Index].pDefault, size);
    }

    r = size;
  }

  return r;
}

int Attribute_Get(attr_idx_t Index, void *pValue, size_t ValueLength,
                  e_table_selection table)
{
  memset(pValue, 0, ValueLength);
  size_t size = -SL_STATUS_INVALID_INDEX;
  int r = -SL_STATUS_INVALID_INDEX;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(Index, table)) {
    if (isReadable(Index, table)) {
      r = PrepareForRead(Index, table);
      if (r >= 0) {

        attr_ptr = get_attribute_table(table);

        if (attr_ptr != NULL)
        {
          size = MIN(attr_ptr[Index].size, ValueLength);
          memcpy(pValue, attr_ptr[Index].pData, size);
        }

        r = size;
      }
    }
    else
    {
       r = -SL_STATUS_FAIL;
    }
  }

  return r;
}

const char *Attribute_GetName(attr_idx_t Index, e_table_selection table)
{
  const char *p = EMPTY_STRING;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(Index,table))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      p = (const char *)(attr_ptr[Index].name);
    }
  }

  return p;
}

size_t Attribute_GetSize(attr_idx_t Index, e_table_selection table)
{
  size_t size = 0;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(Index,table))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      size = attr_ptr[Index].size;
    }
  }

  return size;
}

uint32_t Attribute_GetMinSize_String(attr_idx_t Index, e_table_selection table)
{
  uint32_t size = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  if ((isValid(Index,table)) && (Attribute_GetType(Index,table) == ATTR_TYPE_STRING))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      size = attr_ptr[Index].min.ux;
    }
  }

  return size;
}

uint32_t Attribute_GetMaxSize_String(attr_idx_t Index, e_table_selection table)
{
  uint32_t size = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  if ((isValid(Index,table)) && (Attribute_GetType(Index,table) == ATTR_TYPE_STRING))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      size = attr_ptr[Index].max.ux;
    }
  }

  return size;
}

sl_status_t Attribute_GetMinSize_Numeric(attr_idx_t index,
                                          e_table_selection table,
                                          int64_t *data)
{
  sl_status_t result = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(index, table))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      *data = attr_ptr[index].min.ux;
      result = SL_STATUS_OK;
    }
  }
  return result;
}

sl_status_t Attribute_GetMaxSize_Numeric(attr_idx_t index,
                                          e_table_selection table,
                                          int64_t *data)
{
  sl_status_t result = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  if (isValid(index, table))
  {
    attr_ptr = get_attribute_table(table);

    if (attr_ptr != NULL)
    {
      *data = attr_ptr[index].max.ux;
      result = SL_STATUS_OK;
    }
  }
  return result;
}

Ecode_t Attribute_SaveAll(void)
{
  uint16_t i;
  Ecode_t err = ECODE_NVM3_OK;

  for (i = 0; (i < ATTR_TABLE_SIZE) && (err == ECODE_NVM3_OK); i++)
  {
    if (attrTable[i].savable == true)
    {
      err = nvm3_writeData(NVM3_DEFAULT_HANDLE,
                           i,
                           (unsigned char *)attrTable[i].pData,
                           attrTable[i].size);
    }
  }
  return(err);
}

void Attribute_Convert_Numeric_To_String(int32_t register_index,
                                        char *attribute,
                                        size_t *attribute_length,
                                        e_table_selection table)
{
  AttrType_t field_type;
  uint32_t numeric_value_u32;
  uint16_t numeric_value_u16;
  uint8_t numeric_value_u8;
  int8_t numeric_value_i8;
  int16_t numeric_value_i16;

  field_type = Attribute_GetType(register_index, table);

  /* now convert the field to a string, if it is numeric. */
  switch (field_type)
  {
    case ATTR_TYPE_STRING:
      break;
    case ATTR_TYPE_U32:
      numeric_value_u32 = ((uint32_t)(attribute[3]) << 24) |
                          ((uint32_t)(attribute[2]) << 16) |
                          ((uint32_t)(attribute[1]) << 8) |
                          (uint32_t)(attribute[0]);
      sprintf(attribute, "%" PRIu32, numeric_value_u32);
      break;
    case ATTR_TYPE_U16:
      numeric_value_u16 = ((uint16_t)(attribute[1]) << 8) |
                            (uint16_t)(attribute[0]);
      sprintf(attribute, "%" PRIu16, numeric_value_u16);
      break;
    case ATTR_TYPE_U8:
    case ATTR_TYPE_BOOL:
      numeric_value_u8 = ((uint8_t)(*attribute));
      sprintf(attribute, "%u", numeric_value_u8);
      break;
    case ATTR_TYPE_S8:
      numeric_value_i8 = ((int8_t)(*attribute));
      sprintf(attribute, "%d", numeric_value_i8);
      break;
    case ATTR_TYPE_S16:
      numeric_value_i16 = ((int16_t)(attribute[1]) << 8) |
                            (int16_t)(attribute[0]);
      sprintf(attribute, "%d", numeric_value_i16);
      break;
    default:
      attribute[0] = '\0';
      break;
  }
  if (attribute_length != NULL)
  {
    *attribute_length = strlen(attribute);
  }
}

/******************************************************************************/
/* Local Function Definitions                                                 */
/******************************************************************************/

static int Validate(attr_idx_t Index, AttrType_t Type, void *pValue,
       size_t Length, e_table_selection table)
{
  int r = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {
    if (Type == attr_ptr[Index].type || Type == ATTR_TYPE_ANY) {
      r = attr_ptr[Index].pValidator(&attr_ptr[Index], pValue, Length, false);
    }
  }

  return r;
}

static int Write(attr_idx_t Index, AttrType_t Type, void *pValue, size_t Length,
                 e_table_selection table)
{
  int r = -SL_STATUS_FAIL;
  AttributeEntry_t  *attr_ptr = NULL;

  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {

    if (Type == attr_ptr[Index].type || Type == ATTR_TYPE_ANY) {
        r = attr_ptr[Index].pValidator(&attr_ptr[Index], pValue, Length, true);
    }

  }

  return r;
}

/**
 * @brief Cause actions that will update an attribute.
 * For the majority of attributes, this function doesn't do anything.
 */
static int PrepareForRead(attr_idx_t Index, e_table_selection table)
{
  int r = 0;
  AttributeEntry_t  *attr_ptr = NULL;

  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {
    if (attr_ptr[Index].pPrepare != NULL) {
      r = attr_ptr[Index].pPrepare();
    }
  }

  return r;
}

static bool isValid(attr_idx_t Index, e_table_selection table)
{
  bool return_value = false;

  if (table == SELECT_ATTRIBUTE_TABLE)
  {
    if (Index < ATTR_TABLE_SIZE)
    {
      return_value = true;
    }
  }
  else if (table == SELECT_CORE_ATTRIBUTE_TABLE)
  {
    if (Index < CORE_ATTR_TABLE_SIZE)
    {
      return_value = true;
    }
  }

  return return_value;
}

static bool isReadable(attr_idx_t Index, e_table_selection table)
{
  bool ret_value = false;
  AttributeEntry_t  *attr_ptr = NULL;

  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {
      ret_value = attr_ptr[Index].readable;
  }

  return ret_value;
}

static bool isWritable(attr_idx_t Index, e_table_selection table)
{
  AttributeEntry_t  *attr_ptr = NULL;
  bool r = false;

  attr_ptr = get_attribute_table(table);

  if (attr_ptr != NULL)
  {
    if (attr_ptr[Index].writable) {
      r = true;
    }
  }

  return r;
}

/* Obtain the pointer to the required attribute table given the table id.
 * If the table does not exist, then return NULL.
 *
 */
static AttributeEntry_t * get_attribute_table(e_table_selection table)
{

  AttributeEntry_t *p = NULL;

  if (table == SELECT_ATTRIBUTE_TABLE)
  {
    p = &attrTable[0];
  }
  else if (table == SELECT_CORE_ATTRIBUTE_TABLE)
  {
    p = &core_attrTable[0];
  }

  return p;
}

AttributeEntry_t *attr_map(attr_idx_t index, e_table_selection table)
{
  AttributeEntry_t *attribute_table_entry = get_attribute_table(table);

  if ((attribute_table_entry != NULL) && isValid(index, table)) {
    attribute_table_entry = &attribute_table_entry[index];
  }
  return(attribute_table_entry);
}
