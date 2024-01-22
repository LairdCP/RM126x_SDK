/*!
 * \file      smtc_modem_hal.c
 *
 * \brief     Modem Hardware Abstraction Layer API implementation.
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021-23. All rights reserved.
 * Copyright Laird Connectivity 2024. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "modem_pinout.h"
#include "smtc_board.h"
#include "smtc_modem_hal.h"
#include "smtc_hal.h"
#include "ticker.h"
#include "em_gpio.h"
#include "sl_emlib_gpio_init_pin_sx_dio1_config.h"
#include "gpiointerrupt.h"
#include "ticker.h"
// for variadic args
#include <stdio.h>
#include <stdarg.h>

// for LoRa context storage in attribute table
#include "app_log.h"
#include "Attribute.h"
#include "AttributeTable.h"
#include "lr1mac_defs.h"
#include "modem_context.h"
#include "smtc_secure_element.h"
#include "string_conversion.h"

// for memcpy
#include <string.h>

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */
#define JOIN_NONCE_SIZE 6
#define JOIN_NONCE_STR_SIZE (JOIN_NONCE_SIZE * 2)

/*
 * Secure element data is stored for internal use in the structure below.
 *
 * typedef struct soft_se_data_s
 * {
 *   uint8_t       deveui[SMTC_SE_EUI_SIZE];          //!< DevEUI storage
 *   uint8_t       joineui[SMTC_SE_EUI_SIZE];         //!< Join EUI storage
 *   uint8_t       pin[SMTC_SE_PIN_SIZE];             //!< pin storage
 *   soft_se_key_t key_list[SOFT_SE_NUMBER_OF_KEYS];  //!< The key list
 * } soft_se_data_t;
 *
 * The key_list is used to store details of keys used by the stack, with each
 * key having a key id and associated key data. SOFT_SE_NUMBER_OF_KEYS
 * currently contains 23 keys.
 *
 * In external storage, key values are stored in ASCII format in a list of
 * string based attributes prefixed with lora_se_key_x. The key ids are stored
 * in two string based attributes, lora_se_key_map_1 and lora_se_key_map_2, with
 * individual key ids also being stored in ASCII format. This avoids having a
 * further SOFT_SE_NUMBER_OF_KEYS attributes for the individual key ids, and
 * allows the key ids to be read out using array indexing.
 */

/* This is the total number of key ids that can be held in both attributes.
 * This define is also used to describe the width of each ASCII based key map
 * attribute, with each containing half of the content of the binary format
 * key list.
 */
#define LC_SOFT_SE_NUM_KEYS 32

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static hal_gpio_irq_t radio_dio_irq;
#ifdef USE_CRASHLOG
uint8_t __attribute__( ( section( ".noinit" ) ) ) saved_crashlog[CRASH_LOG_SIZE];
volatile bool __attribute__( ( section( ".noinit" ) ) ) crashlog_available;
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */
static void smtc_modem_hal_mdm_ctx_attr_set(modem_context_nvm_t *buffer,
                                            uint32_t size);
static void smtc_modem_hal_mac_ctx_attr_set(mac_context_t *buffer,
                                            uint32_t size);
static void smtc_modem_hal_devnonce_ctx_attr_set(lr1_counter_context_t *buffer,
                                                 uint32_t size);
static void smtc_modem_hal_se_ctx_attr_set(soft_se_context_nvm_t *buffer,
                                                 uint32_t size);
static void smtc_modem_hal_mdm_ctx_attr_get(modem_context_nvm_t *buffer,
                                            uint32_t size);
static void smtc_modem_hal_mac_ctx_attr_get(mac_context_t *buffer,
                                            uint32_t size);
static void smtc_modem_hal_devnonce_ctx_attr_get(lr1_counter_context_t *buffer,
                                                 uint32_t size);
static void smtc_modem_hal_se_ctx_attr_get(soft_se_context_nvm_t *buffer,
                                                 uint32_t size);
static smtc_se_key_identifier_t smtc_modem_hal_se_ctx_key_map(uint8_t index,
                                                              uint8_t *key);
/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/* ------------ Reset management ------------*/
void smtc_modem_hal_reset_mcu( void )
{
#ifdef COVERAGE_ENABLED
    /* Save coverage info */
    coverage_dump( );
#endif
    hal_mcu_reset( );
}

/* ------------ Watchdog management ------------*/

void smtc_modem_hal_reload_wdog( void )
{
    hal_watchdog_reload( );
}

/* ------------ Time management ------------*/

uint32_t smtc_modem_hal_get_time_in_s( void )
{
    return hal_rtc_get_time_s( );
}

uint32_t smtc_modem_hal_get_compensated_time_in_s( void )
{
    return hal_rtc_get_time_s( );
}

int32_t smtc_modem_hal_get_time_compensation_in_s( void )
{
    return 0;
}

uint32_t smtc_modem_hal_get_time_in_ms( void )
{
    return hal_rtc_get_time_ms( );
}

uint32_t smtc_modem_hal_get_time_in_100us( void )
{
    return hal_rtc_get_time_100us( );
}

uint32_t smtc_modem_hal_get_radio_irq_timestamp_in_100us( void )
{
    /* In current LBM implementation the call of this function is done in
     * radio_planner radio irq handler so the current time is the irq time
     */
    return hal_rtc_get_time_100us( );
}

/* ------------ Timer management ------------*/

void smtc_modem_hal_start_timer( const uint32_t milliseconds,
                                 void ( *callback )( void* context ),
                                 void* context )
{
  /* Start a timer with callback service */
  start_ticker_timed_callback(milliseconds, callback, context);
}

void smtc_modem_hal_stop_timer( void )
{
  start_ticker_timed_callback_stop();
}

/* ------------ IRQ management ------------*/

void smtc_modem_hal_disable_modem_irq( void )
{
    hal_gpio_irq_disable( );
    hal_lp_timer_irq_disable( );
}

void smtc_modem_hal_enable_modem_irq( void )
{
    hal_gpio_irq_enable( );
    hal_lp_timer_irq_enable( );
}

/* ------------ Context saving management ------------*/
void smtc_modem_hal_context_restore( const modem_context_type_t ctx_type,
                                     uint8_t* buffer, const uint32_t size )
{
    if ((buffer != NULL) && (size > 0))
    {
        /* ensure a zero'd out buffer before we start */
        memset(buffer, 0, size);

        switch( ctx_type )
        {
          case CONTEXT_MODEM:
            smtc_modem_hal_mdm_ctx_attr_get((modem_context_nvm_t *)buffer,
                                            size);
            break;
          case CONTEXT_LR1MAC:
            smtc_modem_hal_mac_ctx_attr_get((mac_context_t *)buffer, size);
            break;
          case CONTEXT_DEVNONCE:
            smtc_modem_hal_devnonce_ctx_attr_get((lr1_counter_context_t *)buffer,
                                                 size);
            break;
          case CONTEXT_SECURE_ELEMENT:
            smtc_modem_hal_se_ctx_attr_get((soft_se_context_nvm_t *)buffer,
                                           size);
            break;
          default:
            break;
        }
    }
}

void smtc_modem_hal_context_store( const modem_context_type_t ctx_type,
                                   const uint8_t* buffer, const uint32_t size )
{
  if (buffer != NULL)
  {
    switch( ctx_type )
    {
      case CONTEXT_MODEM:
        smtc_modem_hal_mdm_ctx_attr_set((modem_context_nvm_t *)buffer, size);
        break;
      case CONTEXT_LR1MAC:
        smtc_modem_hal_mac_ctx_attr_set((mac_context_t *)buffer, size);
        break;
      case CONTEXT_DEVNONCE:
        smtc_modem_hal_devnonce_ctx_attr_set((lr1_counter_context_t *)buffer,
                                             size);
        break;
      case CONTEXT_SECURE_ELEMENT:
        smtc_modem_hal_se_ctx_attr_set((soft_se_context_nvm_t *)buffer, size);
        break;
      default:
        app_log("Attempting to store an invalid modem context type %d.\n",
                ctx_type);
        break;
    }
  }
}

/* ------------ Crashlog management ------------*/

void smtc_modem_hal_store_crashlog( uint8_t *crashlog )
{
    #ifndef USE_CRASHLOG
    UNUSED( crashlog );
    #else
    memcpy( &saved_crashlog, crashlog, CRASH_LOG_SIZE );
    #endif
}

void smtc_modem_hal_restore_crashlog( uint8_t crashlog[CRASH_LOG_SIZE] )
{
    #ifndef USE_CRASHLOG
    UNUSED( crashlog );
    #else
    memcpy( crashlog, &saved_crashlog, CRASH_LOG_SIZE );
    #endif
}

void smtc_modem_hal_set_crashlog_status( bool available )
{
    #ifndef USE_CRASHLOG
    UNUSED( available );
    #else
    crashlog_available = available;
    #endif
}

bool smtc_modem_hal_get_crashlog_status( void )
{
    #ifdef USE_CRASHLOG
    return crashlog_available;
    #else
    return false;
    #endif
}

/* ------------ assert management ------------*/
void smtc_modem_hal_assert_fail( uint8_t* func, uint32_t line )
{
    (void)func;
    (void)line;
}

/* ------------ Random management ------------*/

uint32_t smtc_modem_hal_get_random_nb( void )
{
    return hal_rng_get_random( );
}

uint32_t smtc_modem_hal_get_random_nb_in_range( const uint32_t val_1,
                                                const uint32_t val_2 )
{
    return hal_rng_get_random_in_range( val_1, val_2 );
}

int32_t smtc_modem_hal_get_signed_random_nb_in_range( const int32_t val_1,
                                                      const int32_t val_2 )
{
    return hal_rng_get_signed_random_in_range( val_1, val_2 );
}

/* ------------ Radio env management ------------*/
void smtc_modem_hal_irq_config_radio_irq( void ( *callback )( void* context ),
                                          void* context )
{
  /* Save the radio planner context for the IRQ handler */
  radio_dio_irq.pin      = SMTC_RADIO_DIOX;
  radio_dio_irq.callback = callback;
  radio_dio_irq.context  = context;
  hal_gpio_irq_attach( &radio_dio_irq );

  /* Set the DIO1 interrupt callback */
  GPIOINT_CallbackRegister(SL_EMLIB_GPIO_INIT_PIN_SX_DIO1_PIN,
                           HAL_GPIO_EXTI_Callback);

  /* Set the DIO1 interrupt and enable it */
  GPIO_ExtIntConfig(SL_EMLIB_GPIO_INIT_PIN_SX_DIO1_PORT,
                    SL_EMLIB_GPIO_INIT_PIN_SX_DIO1_PIN,
                    SL_EMLIB_GPIO_INIT_PIN_SX_DIO1_PIN, true, false, true);
}

void smtc_modem_hal_radio_irq_clear_pending( void )
{

}

void smtc_modem_hal_start_radio_tcxo( void )
{
    // put here the code that will start the tcxo if needed
}

void smtc_modem_hal_stop_radio_tcxo( void )
{
    // put here the code that will stop the tcxo if needed
}

uint32_t smtc_modem_hal_get_radio_tcxo_startup_delay_ms( void )
{
    return smtc_board_get_tcxo_startup_time_in_ms( );
}

/* ------------ Environment management ------------*/

uint8_t smtc_modem_hal_get_battery_level( void )
{
    return 254;
}

int8_t smtc_modem_hal_get_temperature( void )
{
    return 0;
}

uint8_t smtc_modem_hal_get_voltage( void )
{

    // convert voltage from mv to cloud readable (1/50V = 20mv)
    return 0;
}

int8_t smtc_modem_hal_get_board_delay_ms( void )
{
    return 1;
}

/* ------------ Trace management ------------*/

void smtc_modem_hal_print_trace( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    vprintf( fmt, args );
    va_end( args );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */
static void smtc_modem_hal_mdm_ctx_attr_set(modem_context_nvm_t *buffer,
                                            uint32_t size)
{
  int attr_ret = 0;
  bool mod = 0;

  if ((buffer != NULL) && (size == sizeof(modem_context_nvm_t)))
  {
      attr_ret = Attribute_Set(ATTR_INDEX_lora_modem_dm_port, ATTR_TYPE_U16,
                               (void *)&(buffer->dm_port),
                               sizeof(uint16_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_dm_port, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_modem_upload_sctr,
                               ATTR_TYPE_U16,
                               (void *)&(buffer->dm_upload_sctr),
                               sizeof(uint16_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_upload_sctr, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_modem_app_key_crc_status,
                               ATTR_TYPE_U8,
                               (void *)&(buffer->appkey_crc_status),
                               sizeof(uint8_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_app_key_crc_status, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_modem_app_key_crc,
                               ATTR_TYPE_U32,
                               (void *)&(buffer->appkey_crc),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_app_key_crc, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_modem_crc,
                               ATTR_TYPE_U32, (void *)&(buffer->crc),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_crc, attr_ret);
      }
  }

  Attribute_SaveAll();
  return;
}

static void smtc_modem_hal_mac_ctx_attr_set(mac_context_t *buffer,
                                            uint32_t size)
{
  int attr_ret = 0;
  bool mod = 0;

  if ((buffer != NULL) && (size == sizeof(mac_context_t)))
  {
      attr_ret = Attribute_Set(ATTR_INDEX_lora_mac_adr_custom,
                               ATTR_TYPE_U32, (void *)&(buffer->adr_custom),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_adr_custom, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_mac_region_type,
                             ATTR_TYPE_U8,
                             (void *)&(buffer->region_type),
                             sizeof(uint8_t), &mod,
                             SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_region_type, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_mac_certification_enabled,
                             ATTR_TYPE_U8,
                             (void *)&(buffer->certification_enabled),
                             sizeof(uint8_t), &mod,
                             SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_certification_enabled, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_mac_crc,
                               ATTR_TYPE_U32, (void *)&(buffer->crc),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_crc, attr_ret);
      }
  }
  Attribute_SaveAll();
  return;
}

static void smtc_modem_hal_devnonce_ctx_attr_set(lr1_counter_context_t *buffer,
                                            uint32_t size)
{
  int attr_ret = 0;
  bool mod = 0;
  uint8_t join_nonce_str[JOIN_NONCE_STR_SIZE];

  if ((buffer != NULL) && (size == sizeof(lr1_counter_context_t)))
  {
      attr_ret = Attribute_Set(ATTR_INDEX_lora_devnonce_devnonce, ATTR_TYPE_U16,
                               (void *)&(buffer->devnonce),
                               sizeof(uint16_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_devnonce, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_devnonce_nb_reset,
                               ATTR_TYPE_U32,
                               (void *)&(buffer->nb_reset),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_nb_reset, attr_ret);
      }

      lc_str_bin_to_hex_dump_std(&(buffer->join_nonce[0]), join_nonce_str,
                                 JOIN_NONCE_SIZE);

      attr_ret = Attribute_Set(ATTR_INDEX_lora_devnonce_join_nonce,
                               ATTR_TYPE_STRING,
                               (void *)join_nonce_str,
                               JOIN_NONCE_STR_SIZE, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_join_nonce, attr_ret);
      }

      attr_ret = Attribute_Set(ATTR_INDEX_lora_devnonce_crc,
                               ATTR_TYPE_U32, (void *)&(buffer->crc),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_crc, attr_ret);
      }
  }
  Attribute_SaveAll();
  return;
}

static void smtc_modem_hal_mdm_ctx_attr_get(modem_context_nvm_t *buffer,
                                            uint32_t size)
{
  int attr_ret = 0;

  if ((buffer != NULL) && (size == sizeof(modem_context_nvm_t)))
  {
      Attribute_Get(ATTR_INDEX_lora_modem_dm_port, &(buffer->dm_port),
                    sizeof(uint16_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_dm_port, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_modem_upload_sctr,
                    &(buffer->dm_upload_sctr),
                    sizeof(uint16_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_upload_sctr, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_modem_app_key_crc_status,
                    &(buffer->appkey_crc_status),
                    sizeof(uint8_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_app_key_crc_status, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_modem_app_key_crc,
                    &(buffer->appkey_crc),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_app_key_crc, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_modem_crc,
                    &(buffer->crc),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_modem_crc, attr_ret);
      }
  }
  return;
}

static void smtc_modem_hal_mac_ctx_attr_get(mac_context_t *buffer,
                                            uint32_t size)
{
  int attr_ret = 0;

  if ((buffer != NULL) && (size == sizeof(mac_context_t)))
  {
      Attribute_Get(ATTR_INDEX_lora_mac_adr_custom,
                    &(buffer->adr_custom),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_adr_custom, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_mac_region_type,
                    &(buffer->region_type),
                    sizeof(uint8_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_region_type, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_mac_certification_enabled,
                    &(buffer->certification_enabled),
                    sizeof(uint8_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_certification_enabled, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_mac_crc,
                    &(buffer->crc),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_mac_crc, attr_ret);
      }
  }
  return;
}

static void smtc_modem_hal_devnonce_ctx_attr_get(lr1_counter_context_t *buffer,
                                                 uint32_t size)
{
  int attr_ret = 0;
  uint8_t join_nonce_str[JOIN_NONCE_STR_SIZE];

  if ((buffer != NULL) && (size == sizeof(lr1_counter_context_t)))
  {
      Attribute_Get(ATTR_INDEX_lora_devnonce_devnonce, &(buffer->devnonce),
                    sizeof(uint16_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_devnonce, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_devnonce_nb_reset,
                    &(buffer->nb_reset),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_nb_reset, attr_ret);
      }

      Attribute_Get(ATTR_INDEX_lora_devnonce_join_nonce,
                    join_nonce_str, JOIN_NONCE_STR_SIZE,
                    SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_join_nonce, attr_ret);
      }
      else
      {
          lc_str_hex_decode(&(buffer->join_nonce[0]), join_nonce_str,
                            JOIN_NONCE_STR_SIZE);
      }

      Attribute_Get(ATTR_INDEX_lora_devnonce_crc,
                    &(buffer->crc),
                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_devnonce_crc, attr_ret);
      }
  }
  return;
}

static smtc_se_key_identifier_t smtc_modem_hal_se_ctx_key_map(uint8_t index,
                                                              uint8_t *key)
{
    int attr_ret = 0;
    /* Key id read from the key map via the passed absolute index. */
    smtc_se_key_identifier_t ret = SMTC_SE_NO_KEY;
    /* These are the binary format key ids read back from attributes. */
    uint8_t key_map[LC_SOFT_SE_NUM_KEYS] = { 0 };
    /* This is the content of both key map attributes in ASCII format. */
    uint8_t key_map_str[LC_SOFT_SE_NUM_KEYS * 2] = { 0 };
    /* Attribute readback buffer for keys stored in ASCII format in NVM */
    uint8_t key_attr[SMTC_SE_KEY_SIZE * 2];
    /* Internal representation of keys in binary format */
    uint8_t key_attr_bin[SMTC_SE_KEY_SIZE];

    memset(key, 0, SMTC_SE_KEY_SIZE);

    /* Read back the first half of the key map */
    attr_ret = Attribute_Get(ATTR_INDEX_lora_se_key_map_1,
                             key_map_str, LC_SOFT_SE_NUM_KEYS,
                             SELECT_ATTRIBUTE_TABLE);

    if (attr_ret != LC_SOFT_SE_NUM_KEYS)
    {
        app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                      ATTR_INDEX_lora_se_key_map_1, attr_ret);
    }
    else
    {
        /* Read back the second half of the key map */
        attr_ret = Attribute_Get(ATTR_INDEX_lora_se_key_map_2,
                                 key_map_str + LC_SOFT_SE_NUM_KEYS,
                                 LC_SOFT_SE_NUM_KEYS, SELECT_ATTRIBUTE_TABLE);
        if (attr_ret != LC_SOFT_SE_NUM_KEYS)
        {
            app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                          ATTR_INDEX_lora_se_key_map_2, attr_ret);
        }

        /* Convert both ASCII halves of the key map into binary */
        lc_str_hex_decode(key_map, key_map_str, LC_SOFT_SE_NUM_KEYS * 2);
        /* Then extract the key id as our return value */
        ret = key_map[index];

        /* Read back the key value via its absolute index */
        attr_ret = Attribute_Get((ATTR_INDEX_lora_se_key_1 + index),
                                 key_attr, SMTC_SE_KEY_SIZE * 2,
                                 SELECT_ATTRIBUTE_TABLE);

        if (attr_ret != (SMTC_SE_KEY_SIZE * 2))
        {
            app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                          (ATTR_INDEX_lora_se_key_1 + index), attr_ret);
        }
        else
        {
            /* Then convert to binary if read back was OK */
            lc_str_hex_decode(key_attr_bin, key_attr, SMTC_SE_KEY_SIZE * 2);
            memcpy(key, key_attr_bin, SMTC_SE_KEY_SIZE);
        }
    }

    return (ret);
}

static void smtc_modem_hal_se_ctx_attr_set(soft_se_context_nvm_t *buffer,
                                                 uint32_t size)
{
  int attr_ret = 0;
  bool mod = false;
  soft_se_data_t *se_data = &(buffer->data);

  /* Internally the key map uses single bytes to indicate the key id */
  uint8_t key_map_bin[LC_SOFT_SE_NUM_KEYS] = { 0 };

  /* The key map is stored in NVM in two separate attributes, lora_se_key_map_1
   * and lora_se_key_map_2, in ASCII format. When reading back, the single
   * array below is used for reading from and writing to attributes, with
   * indexing being used to determine what half of the buffer is written to the
   * appropriate attribute.
   */
  uint8_t key_map_str[LC_SOFT_SE_NUM_KEYS * 2] = { 0 };

  /* Individual keys are stored in attributes lora_se_key_x where x is the index
   * of the key. The keys are stored in ASCII format so occupy twice the width
   * of the internal binary representation. Note these have to be consecutive
   * so an absolute index can be used for reading and writing via the loop below
   */
  uint8_t key_str[SMTC_SE_KEY_SIZE * 2] = { 0 };

  /* EUIs are also stored in ASCII format */
  uint8_t eui_str[SMTC_SE_EUI_SIZE * 2] = { 0 };

  /* This is the absolute index of the key */
  uint8_t idx = 0;

  if ((buffer != NULL) && (se_data != NULL) &&
      (size == sizeof(soft_se_context_nvm_t)))
  {
      /* Convert the Dev EUI to ASCII */
      lc_str_bin_to_hex_dump_std(se_data->deveui, eui_str,
                                 SMTC_SE_EUI_SIZE);

      /* Store in the appropriate attribute */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_deveui,
                               ATTR_TYPE_STRING,
                               (void *)eui_str,
                               SMTC_SE_EUI_SIZE * 2, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_deveui, attr_ret);
      }

      /* Convert the Join EUI to ASCII */
      lc_str_bin_to_hex_dump_std(se_data->joineui, eui_str,
                                 SMTC_SE_EUI_SIZE);

      /* Store in the appropriate attribute */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_joineui,
                               ATTR_TYPE_STRING,
                               (void *)eui_str,
                               SMTC_SE_EUI_SIZE * 2, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_joineui, attr_ret);
      }

      /* Convert the PIN to ASCII */
      lc_str_bin_to_hex_dump_std(se_data->pin, eui_str,
                                 SMTC_SE_PIN_SIZE);

      /* Store in the appropriate attribute */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_pin,
                               ATTR_TYPE_STRING,
                               (void *)eui_str,
                               SMTC_SE_PIN_SIZE * 2, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_pin, attr_ret);
      }

      /* Set the key_ids in the key map attribute, and set the
       * keys in their respective attributes. Key setting assumes the
       * keys are stored in consecutive attribute IDs.
       */
      do
      {
          /* Read out the next key id. We don't do anything with the key map
           * until all keys are written.
           */
          key_map_bin[idx] = se_data->key_list[idx].key_id;

          /* Convert the next key to ASCII */
          lc_str_bin_to_hex_dump_std(se_data->key_list[idx].key_value,
                                     key_str, SMTC_SE_KEY_SIZE);

          /* Then store in the appropriate attribute */
          attr_ret = Attribute_Set((ATTR_INDEX_lora_se_key_1 + idx),
                                   ATTR_TYPE_STRING,
                                   (void *)key_str,
                                   SMTC_SE_KEY_SIZE * 2, &mod,
                                   SELECT_ATTRIBUTE_TABLE);

          if (attr_ret != SL_STATUS_OK)
          {
              app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                              (ATTR_INDEX_lora_se_key_1 + idx), attr_ret);
          }
      } while (++idx < SOFT_SE_NUMBER_OF_KEYS);

      /* Convert the whole key map to ASCII */
      lc_str_bin_to_hex_dump_std(key_map_bin, key_map_str,
                                 LC_SOFT_SE_NUM_KEYS);

      /* Store the first half */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_key_map_1,
                               ATTR_TYPE_STRING,
                               (void *)key_map_str,
                               LC_SOFT_SE_NUM_KEYS, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_key_map_1, attr_ret);
      }

      /* Then the second half */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_key_map_2,
                               ATTR_TYPE_STRING,
                               (void *)(key_map_str + LC_SOFT_SE_NUM_KEYS),
                               LC_SOFT_SE_NUM_KEYS, &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_key_map_2, attr_ret);
      }

      /* The pre-calculated CRC is stored last */
      attr_ret = Attribute_Set(ATTR_INDEX_lora_se_crc,
                               ATTR_TYPE_U32, (void *)&(buffer->crc),
                               sizeof(uint32_t), &mod,
                               SELECT_ATTRIBUTE_TABLE);

      if (attr_ret != SL_STATUS_OK)
      {
          app_log_error("Failed to set attribute for modem context storage: %d, %d\n",
                        ATTR_INDEX_lora_se_crc, attr_ret);
      }
  }
  Attribute_SaveAll();
  return;
}

static void smtc_modem_hal_se_ctx_attr_get(soft_se_context_nvm_t *buffer,
                                                 uint32_t size)
{
  int attr_ret = 0;
  soft_se_data_t *se_data = NULL;
  uint8_t idx = 0;
  uint8_t eui[SMTC_SE_EUI_SIZE * 2];
  uint8_t pin[SMTC_SE_PIN_SIZE * 2];

  if ((buffer != NULL) && (size == sizeof(soft_se_context_nvm_t)))
  {
      se_data = &(buffer->data);
      memset(buffer, 0, size);

      if (se_data != NULL)
      {
          /* handle the non-key data */
          attr_ret = Attribute_Get(ATTR_INDEX_lora_se_deveui,
                                   eui, SMTC_SE_EUI_SIZE * 2,
                                   SELECT_ATTRIBUTE_TABLE);

          if (attr_ret != (SMTC_SE_EUI_SIZE * 2))
          {
              app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                            ATTR_INDEX_lora_se_deveui, attr_ret);
          }
          else
          {
              lc_str_hex_decode(buffer->data.deveui, eui, (SMTC_SE_EUI_SIZE * 2));
          }

          attr_ret = Attribute_Get(ATTR_INDEX_lora_se_joineui,
                                   eui, SMTC_SE_EUI_SIZE * 2,
                                   SELECT_ATTRIBUTE_TABLE);

          if (attr_ret != (SMTC_SE_EUI_SIZE * 2))
          {
              app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                            ATTR_INDEX_lora_se_joineui, attr_ret);
          }
          else
          {
              lc_str_hex_decode(buffer->data.joineui, eui, (SMTC_SE_EUI_SIZE * 2));
          }

          attr_ret = Attribute_Get(ATTR_INDEX_lora_se_pin,
                                   pin, SMTC_SE_PIN_SIZE * 2,
                                   SELECT_ATTRIBUTE_TABLE);

          if (attr_ret != (SMTC_SE_PIN_SIZE * 2))
          {
              app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                            ATTR_INDEX_lora_se_pin, attr_ret);
          }
          else
          {
              lc_str_hex_decode(buffer->data.pin, eui, (SMTC_SE_PIN_SIZE * 2));
          }

          /* Retrieve the key_ids from the key map attribute, and retrieve the
           * keys from their respective attributes. Key retrieval assumes the
           * keys are stored in consecutive attribute IDs.
           */
          do
          {
              se_data->key_list[idx].key_id =
                    smtc_modem_hal_se_ctx_key_map(idx,
                                          se_data->key_list[idx].key_value);
          } while (++idx < SOFT_SE_NUMBER_OF_KEYS);

          attr_ret = Attribute_Get(ATTR_INDEX_lora_se_crc,
                                    &(buffer->crc),
                                    sizeof(uint32_t), SELECT_ATTRIBUTE_TABLE);

          if (attr_ret != sizeof(uint32_t))
          {
              app_log_error("Failed to get attribute from modem context storage: %d, %d\n",
                            ATTR_INDEX_lora_se_crc, attr_ret);
          }
      }
  }

  return;
}

/* --- EOF ------------------------------------------------------------------ */
