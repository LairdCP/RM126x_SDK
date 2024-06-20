/***************************************************************************//**
 * @file
 * @brief LoRaWAN application functions
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
/*!
 * @file      app_lorawan.c
 *
 * @brief     LoRa Basics Modem Class A device implementation
 *
 * @copyright
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
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
#include <stddef.h>
#include "app_log.h"
#include <string.h>
#include "sl_status.h"
#include "Attribute.h"
#include "app_log.h"
#include "em_cmu.h"
#include "sl_iostream_init_instances.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "apps_modem_common.h"
#include "apps_modem_event.h"
#include "smtc_board_ralf.h"
#include "apps_utilities.h"
#include "smtc_modem_utilities.h"
#include "smtc_modem_api_str.h"
#include "rm126x_config.h"
#include "smtc_modem_hal.h"
#include "ticker.h"
#include "sl_sleeptimer.h"
#include "app_peripheral.h"
#include "app_lorawan.h"
#include "app_formatters.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*!
 * @brief Stringify constants
 */
#define xstr( a ) str( a )
#define str( a ) #a

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */
#define LORAWAN_APP_DATA_MAX_SIZE 242
#define LORAWAN_APP_PORT RM126X_APPLICATION_PORT
#define APP_TX_DUTYCYCLE RM126X_PERIODIC_UPDATE_RATE_S
#define LORAWAN_CONFIRMED_MSG_ON RM126X_PACKET_TYPE
#define LORAWAN_DEFAULT_DATARATE SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED


/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */
/*!
 * @brief Flag used to indicate a stack update has been scheduled.
 *        Default this to true so the first update always occurs.
 */
static bool stack_update_needed = true;

 /*!
  * @brief Handle to the sleep timer used to manage stack updates
  */
static sl_sleeptimer_timer_handle_t stack_update_timer = {};

/*!
 * @brief Stack identifier
 */
static uint8_t stack_id = 0;

/*!
 * @brief User application data
 */
static uint8_t app_data_buffer[LORAWAN_APP_DATA_MAX_SIZE];

/*!
 * @brief ADR custom list when LORAWAN_DEFAULT_DATARATE is set to SMTC_MODEM_ADR_PROFILE_CUSTOM
 */
static uint8_t adr_custom_list[16] = { 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x03, 0x03,
                                       0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00 };

/*!
 * @brief Count of uplinks sent
 */
static uint16_t uplink_count = 0;

/*!
 * @brief Connected peripheral instance. This is unused in sample applications
 *        where RM126X_ADD_PERIPHERAL_SUPPORT is not defined.
 */
#ifdef RM126X_ADD_PERIPHERAL_SUPPORT
static peripheral_descriptor_t *peripheral_descriptor = NULL;
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * @brief   Send an application frame on LoRaWAN port defined by LORAWAN_APP_PORT
 *
 * @param [in] buffer     Buffer containing the LoRaWAN buffer
 * @param [in] length     Payload length
 * @param [in] confirmed  Send a confirmed or unconfirmed uplink [false : unconfirmed / true : confirmed]
 */
static void send_frame( const uint8_t* buffer, const uint8_t length, const bool confirmed );

/*!
 * @brief Reset event callback
 *
 * @param [in] reset_count reset counter from the modem
 */
static void on_modem_reset( uint16_t reset_count );

/*!
 * @brief Network Joined event callback
 */
static void on_modem_network_joined( void );

/*!
 * @brief Alarm event callback
 */
static void on_modem_alarm( void );

/*!
 * @brief Tx done event callback
 *
 * @param [in] status tx done status @ref smtc_modem_event_txdone_status_t
 */
static void on_modem_tx_done( smtc_modem_event_txdone_status_t status );

/*!
 * @brief Downlink data event callback.
 *
 * @param [in] rssi       RSSI in signed value in dBm + 64
 * @param [in] snr        SNR signed value in 0.25 dB steps
 * @param [in] rx_window  RX window
 * @param [in] port       LoRaWAN port
 * @param [in] payload    Received buffer pointer
 * @param [in] size       Received buffer size
 */
static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size );

/*!
 * @brief Sleeptimer callback invoked when the stack update sleeptimer expires.
 *
 * @param [in] handle     Associated sleeptimer handler
 * @param [in] data       Data associated with the callback
 */
static void lora_interface_update_callback( sl_sleeptimer_timer_handle_t *handle,
                                            void *data );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

/**
 * @brief Main application entry point.
 */
void app_lorawan_init(peripheral_descriptor_t *in_peripheral_descriptor)
{
    static apps_modem_event_callback_t smtc_event_callback = {
        .adr_mobile_to_static  = NULL,
        .alarm                 = on_modem_alarm,
        .almanac_update        = NULL,
        .down_data             = on_modem_down_data,
        .join_fail             = NULL,
        .joined                = on_modem_network_joined,
        .link_status           = NULL,
        .mute                  = NULL,
        .new_link_adr          = NULL,
        .reset                 = on_modem_reset,
        .set_conf              = NULL,
        .stream_done           = NULL,
        .time_updated_alc_sync = NULL,
        .tx_done               = on_modem_tx_done,
        .upload_done           = NULL,
    };

    /* Store peripheral data for use later for sample applications where
     * RM126X_ADD_PERIPHERAL_SUPPORT is defined. Otherwise unused. */
#ifdef RM126X_ADD_PERIPHERAL_SUPPORT
    peripheral_descriptor = in_peripheral_descriptor;
#else
    UNUSED(in_peripheral_descriptor);
#endif

    /* Initialise the ralf_t object corresponding to the board */
    ralf_t* modem_radio = smtc_board_initialise_and_get_ralf( );

    /* Disable IRQ to avoid unwanted behaviour during init */
    hal_mcu_disable_irq( );

    /* Init board and peripherals */
    ticker_init( );
    hal_mcu_init( );
    smtc_board_init_periph( );

    /* Init the Lora Basics Modem event callbacks */
    apps_modem_event_init( &smtc_event_callback );

    /* Init the modem and use apps_modem_event_process as event callback, please note that the callback will be called
     * immediately after the first call to modem_run_engine because of the reset detection */
    smtc_modem_init( modem_radio, &apps_modem_event_process );

    /* Re-enable IRQ */
    hal_mcu_enable_irq( );

    HAL_DBG_TRACE_MSG( "\n" );
    HAL_DBG_TRACE_INFO( "###### ===== LoRa Basics Modem LoRaWAN Class A demo application ==== ######\n\n" );
    apps_modem_common_display_version_information( );
}

void app_lorawan_update(void)
{
    /* Time calculated for next stack update */
    uint32_t sleep_time_ms;

    /* Check if a stack update has been scheduled . . . */
    if ( stack_update_needed )
    {
        /* Update will be scheduled . . . */
        stack_update_needed = false;

        /* Update the stack */
        sleep_time_ms = smtc_modem_run_engine( );

        /* Service other parts of the application
         * If the sleep timer has been built we restart it
         */
        if (stack_update_timer.callback != NULL)
        {
            sl_sleeptimer_restart_timer_ms( &stack_update_timer,
                                            sleep_time_ms,
                                            lora_interface_update_callback,
                                            (void *)NULL,
                                            0,
                                            0 );
        }
        else
        {
            /* Otherwise this is the first stack update and we build it here */
            sl_sleeptimer_start_timer_ms( &stack_update_timer,
                                          sleep_time_ms,
                                          lora_interface_update_callback,
                                          (void *)NULL,
                                          0,
                                          0 );
        }
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void on_modem_reset( uint16_t reset_count )
{
    UNUSED( reset_count );

    HAL_DBG_TRACE_INFO( "Application parameters:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN uplink Fport = %d\n", LORAWAN_APP_PORT );
    HAL_DBG_TRACE_INFO( "  - DM report interval   = %d\n", APP_TX_DUTYCYCLE );
    HAL_DBG_TRACE_INFO( "  - Confirmed uplink     = %s\n", ( LORAWAN_CONFIRMED_MSG_ON == true ) ? "Yes" : "No" );

    apps_modem_common_configure_lorawan_params( stack_id );

    ASSERT_SMTC_MODEM_RC( smtc_modem_join_network( stack_id ) );
}

static void on_modem_network_joined( void )
{
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TX_DUTYCYCLE ) );

    ASSERT_SMTC_MODEM_RC( smtc_modem_adr_set_profile( stack_id, LORAWAN_DEFAULT_DATARATE, adr_custom_list ) );
}

static void on_modem_alarm( void )
{
    smtc_modem_status_mask_t modem_status;
    uint8_t                  app_data_size = 0;

    /* Schedule next packet transmission */
    ASSERT_SMTC_MODEM_RC( smtc_modem_alarm_start_timer( APP_TX_DUTYCYCLE ) );
    HAL_DBG_TRACE_PRINTF( "smtc_modem_alarm_start_timer: %d s\n\n", APP_TX_DUTYCYCLE );

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_status( stack_id, &modem_status ) );
    modem_status_to_string( modem_status );

    /* Always add the uplink count */
    app_formatters_get_uint16(uplink_count, app_data_buffer);
    app_data_size = APP_FORMATTERS_UINT16_SIZE;

    /* Skip this step for any sample applications where
     * RM126X_ADD_PERIPHERAL_SUPPORT is not defined. If so, check if we have a
     * peripheral available.
     */
#ifdef RM126X_ADD_PERIPHERAL_SUPPORT
    if ( peripheral_descriptor )
    {
        /* If so, check if it has a get function */
        if ( peripheral_descriptor->app_peripheral_get )
        {
            /* Yes, so call it */
            peripheral_descriptor->app_peripheral_get( &app_data_buffer[
                                                       app_data_size],
                                                       &app_data_size );
        }
        /* Also check if there's a set function */
        if ( peripheral_descriptor->app_peripheral_set )
        {
            /* Yes, so call it */
            peripheral_descriptor->app_peripheral_set( app_data_buffer,
                                                       app_data_size );
        }
    }
#endif

    /* Send the uplink */
    send_frame( app_data_buffer, app_data_size, LORAWAN_CONFIRMED_MSG_ON );
}

static void on_modem_tx_done( smtc_modem_event_txdone_status_t status )
{
    if( status == SMTC_MODEM_EVENT_TXDONE_NOT_SENT )
    {
        HAL_DBG_TRACE_WARNING( "Uplink was not sent\n" );
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Uplink count: %d\n", uplink_count );
        ++uplink_count;
    }
}

static void on_modem_down_data( int8_t rssi, int8_t snr, smtc_modem_event_downdata_window_t rx_window, uint8_t port,
                                const uint8_t* payload, uint8_t size )
{
    HAL_DBG_TRACE_INFO( "Downlink received:\n" );
    HAL_DBG_TRACE_INFO( "  - LoRaWAN Fport = %d\n", port );
    HAL_DBG_TRACE_INFO( "  - Payload size  = %d\n", size );
    HAL_DBG_TRACE_INFO( "  - RSSI          = %d dBm\n", rssi - 64 );
    HAL_DBG_TRACE_INFO( "  - SNR           = %d dB\n", snr >> 2 );

    switch( rx_window )
    {
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX1:
    case SMTC_MODEM_EVENT_DOWNDATA_WINDOW_RX2:
    {
        HAL_DBG_TRACE_INFO( "  - Rx window     = %s\n", smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    default:
    {
        HAL_DBG_TRACE_ERROR( "Unexpected event down data window %s\n",
                             smtc_modem_event_downdata_window_to_str( rx_window ) );
        break;
    }
    }

    if( size != 0 )
    {
        HAL_DBG_TRACE_ARRAY( "Payload", payload, size );
    }
}

static void send_frame( const uint8_t* buffer, const uint8_t length, bool tx_confirmed )
{
    uint8_t tx_max_payload;
    int32_t duty_cycle;

    /* Check if duty cycle is available */
    ASSERT_SMTC_MODEM_RC( smtc_modem_get_duty_cycle_status( &duty_cycle ) );
    if( duty_cycle < 0 )
    {
        HAL_DBG_TRACE_WARNING( "Duty-cycle limitation - next possible uplink in %d ms \n\n", duty_cycle );
        return;
    }

    ASSERT_SMTC_MODEM_RC( smtc_modem_get_next_tx_max_payload( stack_id, &tx_max_payload ) );
    if( length > tx_max_payload )
    {
        HAL_DBG_TRACE_WARNING( "Not enough space in buffer - send empty uplink to flush MAC commands \n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_empty_uplink( stack_id, true, LORAWAN_APP_PORT, tx_confirmed ) );
    }
    else
    {
        HAL_DBG_TRACE_INFO( "Request uplink\n" );
        ASSERT_SMTC_MODEM_RC( smtc_modem_request_uplink( stack_id, LORAWAN_APP_PORT, tx_confirmed, buffer, length ) );
    }
}

static void lora_interface_update_callback(sl_sleeptimer_timer_handle_t *handle,
                                            void *data)
{
    UNUSED( handle );
    UNUSED( data );

    stack_update_needed = true;
}

/* --- EOF ------------------------------------------------------------------ */
