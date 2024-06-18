/***************************************************************************//**
 * @file
 * @brief Peripheral application functions
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
#ifndef APP_PERIPHERAL_H
#define APP_PERIPHERAL_H

typedef struct _peripheral_descriptor_t
{
  void (*app_peripheral_get)(uint8_t *out_data, uint8_t *out_length);
  void (*app_peripheral_set)(void *in_data, uint8_t in_length);
}peripheral_descriptor_t;

#ifdef RM126X_ADD_PERIPHERAL_SUPPORT
/***************************************************************************//**
 * Initialises the connected peripheral device.
 * @param[out] The initialised peripheral descriptor.
 ******************************************************************************/
void app_peripheral_init(peripheral_descriptor_t *peripheral_descriptor);

/***************************************************************************//**
 * Regular tick function for the connected peripheral device.
 ******************************************************************************/
void app_peripheral_update(void);
#else
/***************************************************************************//**
 * For builds where RM126X_ADD_PERIPHERAL_SUPPORT is not defined, resolves to
 * nothing.
 ******************************************************************************/
#define app_peripheral_init(x)

/***************************************************************************//**
 * For builds where RM126X_ADD_PERIPHERAL_SUPPORT is not defined, resolves to
 * nothing.
 ******************************************************************************/
#define app_peripheral_update()
#endif

#endif  // APP_PERIPHERAL_H
