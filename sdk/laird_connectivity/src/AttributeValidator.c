/*******************************************************************************
  Filename:       AttributeValidator.c
  Revised:        08/12/2023
  Revision:       1.0

  Description:    Validator functions for the attribute functionality

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
#include <string.h>
#include <ctype.h>
#include <sl_status.h>

#include "app.h"
#include "AttributeTable.h"
#include "Attribute.h"

#include "lc_common.h"

/******************************************************************************/
/* Global Data Definitions                                                    */
/******************************************************************************/

/******************************************************************************/
/* Local Function Prototypes                                                  */
/******************************************************************************/

/******************************************************************************/
/* Global Function Definitions                                                */
/******************************************************************************/
int attribute_validator_string(AttributeEntry_t *pEntry, void *pValue,
               size_t Length, bool DoWrite)
{
  int r = -SL_STATUS_FAIL;

  /* -1 to account for NULL */
  if (pEntry->size > Length) {
    /* Don't use strncmp because pValue may not be NUL terminated */
    size_t currentLength = strlen(pEntry->pData);
    if (DoWrite && ((memcmp(pEntry->pData, pValue, Length) != 0) ||
        Length == 0 || currentLength != Length)) {
      pEntry->modified = true;
      memset(pEntry->pData, 0, pEntry->size);
      memcpy(pEntry->pData, pValue, Length);
    }
    r = 0;
  }

  return r;
}

int attribute_validator_uint32(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  uint32_t value;
  memcpy(&value, pValue, sizeof(value));

  if (((value >= pEntry->min.ux) && (value <= pEntry->max.ux)) ||
      (pEntry->min.ux == pEntry->max.ux)) {
    if (DoWrite && value != *((uint32_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((uint32_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_uint16(AttributeEntry_t *pEntry, void *pValue,
             size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  uint32_t value = (uint32_t)(*(uint16_t *)pValue);

  if (((value >= pEntry->min.ux) && (value <= pEntry->max.ux)) ||
      (pEntry->min.ux == pEntry->max.ux)) {
    if (DoWrite && value != *((uint16_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((uint16_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_bool(AttributeEntry_t *pEntry, void *pValue,
           size_t Length, bool DoWrite)
{
  /* Same as UINT8 */
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  uint32_t value = (uint32_t)(*(uint8_t *)pValue);

  if (((value >= pEntry->min.ux) && (value <= pEntry->max.ux)) ||
      (pEntry->min.ux == pEntry->max.ux)) {
    if (DoWrite && value != *((uint8_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((uint8_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_uint8(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  uint32_t value = (uint32_t)(*(uint8_t *)pValue);

  if (((value >= pEntry->min.ux) && (value <= pEntry->max.ux)) ||
      (pEntry->min.ux == pEntry->max.ux)) {
    if (DoWrite && value != *((uint8_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((uint8_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_int64(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int64_t value = *(int64_t *)pValue;

  if (DoWrite && value != *((int64_t *)pEntry->pData)) {
    pEntry->modified = true;
    *((int64_t *)pEntry->pData) = value;
  }

  return 0;
}

int attribute_validator_int32(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  int32_t value = *(int32_t *)pValue;

  if (((value >= pEntry->min.sx) && (value <= pEntry->max.sx)) ||
      (pEntry->min.sx == pEntry->max.sx)) {
    if (DoWrite && value != *((int32_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((int32_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_int16(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  int32_t value = (int32_t)(*(int16_t *)pValue);

  if (((value >= pEntry->min.sx) && (value <= pEntry->max.sx)) ||
      (pEntry->min.sx == pEntry->max.sx)) {
    if (DoWrite && value != *((int16_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((int16_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_int8(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  int32_t value = (int32_t)(*(int8_t *)pValue);

  if (((value >= pEntry->min.sx) && (value <= pEntry->max.sx)) ||
      (pEntry->min.sx == pEntry->max.sx)) {
    if (DoWrite && value != *((int8_t *)pEntry->pData)) {
      pEntry->modified = true;
      *((int8_t *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}

int attribute_validator_float(AttributeEntry_t *pEntry, void *pValue,
            size_t Length, bool DoWrite)
{
  ARG_UNUSED(Length);
  int r = -SL_STATUS_FAIL;
  float value = (float)(*(float *)pValue);

  if (((value >= pEntry->min.fx) && (value <= pEntry->max.fx)) ||
      (pEntry->min.fx == pEntry->max.fx)) {
    if (DoWrite && value != *((float *)pEntry->pData)) {
      pEntry->modified = true;
      *((float *)pEntry->pData) = value;
    }
    r = 0;
  }

  return r;
}
