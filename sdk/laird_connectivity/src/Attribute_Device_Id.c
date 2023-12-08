/*******************************************************************************
  Filename:       Attribute_Device_Id.c
  Revised:        08/12/2023
  Revision:       1.0

  Description:    Attribute prepare handlers for device id related parameters.

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
#include <string.h>
#include <sl_status.h>
#include "Attribute.h"

/*******************************************************************************
 ***************************  GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/****************************************************************************
 * Prepare handler for device_id attribute.
 * @return - Result of the attribute prepare operation.
 ****************************************************************************/
int AttributePrepare_device_id(void)
{
  const struct AttributeEntry *attribute_entry;
  #ifdef SEMTECH_RADIO_SX1261
  #define DEFAULT_DEVICE_ID "RM1261"
  #else
  #define DEFAULT_DEVICE_ID "RM1262"
  #endif

  char device_id[] = DEFAULT_DEVICE_ID;

  attribute_entry = attr_map(CORE_ATTR_INDEX_device_id,
                              SELECT_CORE_ATTRIBUTE_TABLE);

  memset(attribute_entry->pData, 0x0, attribute_entry->size);
  memcpy(attribute_entry->pData, device_id,
          strlen(device_id));

  return(0);
}
