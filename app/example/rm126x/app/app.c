/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020-23 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "app_log.h"
#include <string.h>
#include "sl_status.h"
#include "Attribute.h"
#include "app_log.h"
#include "em_cmu.h"
#include "sl_iostream_init_instances.h"
#include "app_internal.h"

void app_init(void)
{
  /* Initialise the NVM. */
  (void)Attribute_Init();

  /* Now perform internal application initialisation */
  app_internal_init();
}

void app_process_action(void)
{
  /* Update LoRaWAN if needed */
  app_internal_process_action();

  /* And perform other application updates here */
}
