/*!
 * @file      smtc_shield_sx1280rf1zhp.c
 *
 * @brief     Implementation specific to SX1280RF1ZHP shield
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
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

#include <stddef.h>
#include "smtc_shield_sx1280rf1zhp.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */


/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

sx128x_reg_mod_t smtc_shield_sx128x_get_reg_mode( void )
{
    return SX128X_REG_MODE_DCDC;
}

void smtc_shield_sx128x_init( void )
{
    smtc_shield_sx128x_init_ant_sw( );

    smtc_shield_sx128x_set_ant_sw( );

    smtc_shield_sx128x_init_led_tx( );
    smtc_shield_sx128x_init_led_rx( );

    smtc_shield_sx128x_reset_led_rx( );
    smtc_shield_sx128x_reset_led_tx( );
}

void smtc_shield_sx128x_deinit( void )
{
    smtc_shield_sx128x_deinit_ant_sw( );

    smtc_shield_sx128x_deinit_led_tx( );
    smtc_shield_sx128x_deinit_led_rx( );
}

void smtc_shield_sx128x_handle_pre_rx( void )
{
    smtc_shield_sx128x_set_led_rx( );
}

void smtc_shield_sx128x_handle_post_rx( void )
{
    smtc_shield_sx128x_reset_led_rx( );
}

void smtc_shield_sx128x_handle_pre_tx( void )
{
    smtc_shield_sx128x_set_led_tx( );
}

void smtc_shield_sx128x_handle_post_tx( void )
{
    smtc_shield_sx128x_reset_led_tx( );
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
