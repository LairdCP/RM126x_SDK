/*!
 * @file      smtc_hal_uart.c
 *
 * @brief     Board specific package UART API implementation.
 *
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

#include <stdint.h>   // C99 types
#include <stdbool.h>  // bool type

#include "smtc_hal_gpio_pin_names.h"
#include "smtc_hal_uart.h"
#include "smtc_hal_mcu.h"
#include "smtc_modem_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * @brief UART structure
 */
typedef struct hal_uart_s
{
    struct
    {
        hal_gpio_pin_names_t tx;
        hal_gpio_pin_names_t rx;
    } pins;
} hal_uart_t;

typedef void UART_HandleTypeDef;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

uint8_t uart_rx_done = false;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */
void USART1_IRQHandler( void );
void USART2_IRQHandler( void );
void USART3_IRQHandler( void );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */
void hal_uart_init( const uint32_t id, const hal_gpio_pin_names_t uart_tx, const hal_gpio_pin_names_t uart_rx )
{
    UNUSED( id );
    UNUSED( uart_tx );
    UNUSED( uart_rx );
}

void hal_uart_deinit( const uint32_t id )
{
    UNUSED( id );
}

void hal_uart_tx( const uint32_t id, uint8_t* buff, uint16_t len )
{
    UNUSED( id );
    UNUSED( buff );
    UNUSED( len );
}

void hal_uart_rx( const uint32_t id, uint8_t* rx_buffer, uint16_t len )
{
    UNUSED( id );
    UNUSED( rx_buffer );
    UNUSED( len );
}

void HAL_UART_MspInit( UART_HandleTypeDef* huart )
{
    UNUSED( huart );
}

void HAL_UART_MspDeInit( UART_HandleTypeDef* huart )
{
    UNUSED( huart );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */
/**
 * @brief  This function handles USART1 interrupt request.
 */
void USART1_IRQHandler( void )
{
}

/**
 * @brief  This function handles USART2 interrupt request.
 */
void USART2_IRQHandler( void )
{
}

/**
 * @brief  This function handles USART3 interrupt request.
 */
void USART3_IRQHandler( void )
{
}

/* --- EOF ------------------------------------------------------------------ */
