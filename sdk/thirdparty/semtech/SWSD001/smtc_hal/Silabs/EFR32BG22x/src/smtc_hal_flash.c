/*!
 * \file      smtc_hal_flash.c
 *
 * \brief     FLASH Hardware Abstraction Layer implementation
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

#include <stdint.h>   
#include <stdbool.h>  
#include <stdio.h>    // TODO: check if needed

#include "smtc_hal.h"
//#include "efr32bg22xx_hal.h"

#include <string.h>
#if 0
/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

#define FLASH_OPERATION_MAX_RETRY 4
#define FLASH_PAGE_DIVIDER 10
#define NB_FLASH_BYTES_TO_TEST  ADDR_FLASH_PAGE_SIZE / FLASH_PAGE_DIVIDER

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
 
 /*!
 * @brief Initializes the flash_user_start_addr to FLASH_USER_END_ADDR to avoid erase an occupied memory.
 */
//uint32_t flash_user_start_addr = FLASH_USER_END_ADDR;

/**
 * @brief  Gets the page of a given address
 * @param  address: Address of the FLASH Memory
 * @retval The page of a given address
 */
static uint32_t hal_flash_get_page( uint32_t address );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

smtc_hal_status_t hal_flash_init( void )
{
    uint8_t  status = SMTC_HAL_SUCCESS;

    return status;
}

smtc_hal_status_t hal_flash_erase_page( uint32_t addr, uint8_t nb_page )
{
    uint8_t  status                = SMTC_HAL_SUCCESS;
    
    return status;
}

uint8_t hal_flash_force_erase_page( uint32_t addr, uint8_t nb_page )
{
    uint8_t  status                = SMTC_HAL_SUCCESS;
    return status;
}

smtc_hal_status_t hal_flash_write_buffer( uint32_t addr, const uint8_t* buffer, uint32_t size )
{
    uint8_t  status       = SMTC_HAL_SUCCESS;

    return status;
}

void hal_flash_read_buffer( uint32_t addr, uint8_t* buffer, uint32_t size )
{

}

uint32_t hal_flash_get_user_start_addr( void ) { return 0; }

void hal_flash_set_user_start_addr( uint32_t addr ) {  }

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static uint32_t hal_flash_get_page( uint32_t address )
{
    return 0;
}
#endif
/* --- EOF ------------------------------------------------------------------ */
