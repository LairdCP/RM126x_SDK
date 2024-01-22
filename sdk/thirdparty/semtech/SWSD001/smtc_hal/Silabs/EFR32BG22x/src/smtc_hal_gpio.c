/*!
 * @file      smtc_hal_gpio.c
 *
 * @brief     Implements the gpio HAL functions
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

#include "smtc_hal.h"
#include "efr32bg22xx_hal.h"

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
 * GPIO setup data structure
 */
typedef struct
{
    hal_gpio_pin_names_t pin;
    uint32_t             mode;
    uint32_t             pull;
    uint32_t             alternate;
} hal_gpio_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * @brief Array holding attached IRQ gpio data context
 */
static hal_gpio_irq_t const* gpio_irq[16];

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*!
 * Generic gpio initialization
 *
 * @param [in/out] gpio  Holds MCU gpio parameters
 * @param [in]     value Initial MCU pin value
 * @param [in/out] irq   Pointer to IRQ data context.
 *                         NULL when setting gpio as output
 */
static void hal_gpio_init( const hal_gpio_t* gpio, const hal_gpio_state_t value, const hal_gpio_irq_t* irq );

/*!
 * Finds the port and pin from a passed HAL pin definition
 *
 * @param [in] pin  The pin to find the details for
 * @param [out] port The associated port structure
 * @param [out] port_pin The associated pin number
 */
static void hal_gpio_find( const hal_gpio_pin_names_t pin, GPIO_Port_TypeDef *port, unsigned int *port_pin);

/*!
 * Sets the pull down direction for a passed port and pin
 *
 * @param [in] port The associated port structure
 * @param [in] port_pin The associated pin number
 * @param [in] pull The associated pull behaviour
 */
static void hal_gpio_set_pull( GPIO_Port_TypeDef port, unsigned int port_pin, tPullDirection pull);

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void hal_gpio_init_out( const hal_gpio_pin_names_t pin, const hal_gpio_state_t value )
{
    hal_gpio_t gpio = {
        .pin = pin, .mode = gpioModePushPull, .pull = PULL_DIRECTION_NONE, .alternate = 0
    };
    hal_gpio_init( &gpio, ( value != 0 ) ? HAL_GPIO_SET : HAL_GPIO_RESET, NULL );

}

void hal_gpio_deinit( const hal_gpio_pin_names_t pin )
{
    hal_gpio_t gpio = {
        .pin = pin, .mode = gpioModeDisabled, .pull = PULL_DIRECTION_NONE, .alternate = 0
    };
    hal_gpio_init( &gpio, HAL_GPIO_RESET, NULL );
}

/**
 * @brief MCU input pin Handling
 */

void hal_gpio_init_in( const hal_gpio_pin_names_t pin, const hal_gpio_pull_mode_t pull_mode,
                       const hal_gpio_irq_mode_t irq_mode, hal_gpio_irq_t* irq )
{
    const uint32_t modes[] =  {
                                INPUT_BEHAVIOUR_INPUT,
                                INPUT_BEHAVIOUR_IT_RISING,
                                INPUT_BEHAVIOUR_IT_FALLING,
                                INPUT_BEHAVIOUR_IT_RISING_FALLING
                              };

    const uint32_t pulls[] =  {
                                PULL_DIRECTION_NONE,
                                PULL_DIRECTION_UP,
                                PULL_DIRECTION_DOWN
                              };

    hal_gpio_t gpio = {
        .pin = pin, .mode = modes[irq_mode], .pull = pulls[pull_mode], .alternate = 0
    };
    if( irq != NULL )
    {
        irq->pin = pin;
    }
    hal_gpio_init( &gpio, HAL_GPIO_RESET, irq );
}

void hal_gpio_irq_attach( const hal_gpio_irq_t* irq )
{
    if( ( irq != NULL ) && ( irq->callback != NULL ) )
    {
        gpio_irq[( irq->pin ) & 0x0F] = irq;
    }
}

void hal_gpio_irq_deatach( const hal_gpio_irq_t* irq )
{
    if( irq != NULL )
    {
        gpio_irq[( irq->pin ) & 0x0F] = NULL;
    }
}

void hal_gpio_irq_enable( void )
{
  NVIC_EnableIRQ( GPIO_ODD_IRQn );
}

void hal_gpio_irq_disable( void )
{
  NVIC_DisableIRQ( GPIO_ODD_IRQn );
}

/**
 * @brief MCU pin state control
 */

void hal_gpio_set_value( const hal_gpio_pin_names_t pin, const hal_gpio_state_t value )
{
  GPIO_Port_TypeDef port;
  unsigned int port_pin;

  hal_gpio_find(pin, &port, &port_pin);

  if (value) {
    GPIO_PinOutSet(port, port_pin);
  } else {
    GPIO_PinOutClear(port, port_pin);
  }
}

void hal_gpio_toggle( const hal_gpio_pin_names_t pin )
{
  GPIO_Port_TypeDef port;
  unsigned int port_pin;

  hal_gpio_find(pin, &port, &port_pin);

  GPIO_PinOutToggle(port, port_pin);
}

uint32_t hal_gpio_get_value( const hal_gpio_pin_names_t pin )
{
  GPIO_Port_TypeDef port;
  unsigned int port_pin;

  hal_gpio_find(pin, &port, &port_pin);

  return(GPIO_PinInGet(port, port_pin));
}

void hal_gpio_clear_pending_irq( const hal_gpio_pin_names_t pin )
{
    (void)pin;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void hal_gpio_init( const hal_gpio_t* gpio, const hal_gpio_state_t value, const hal_gpio_irq_t* irq )
{
  GPIO_Port_TypeDef port;
  unsigned int port_pin;

  (void)irq;

  hal_gpio_find(gpio->pin, &port, &port_pin);

  if (gpio->mode == gpioModePushPull) {
    GPIO_PinModeSet(port, port_pin, gpioModePushPull, value);
  } else if ((gpio->mode == INPUT_BEHAVIOUR_INPUT) ||
              (gpio->mode == INPUT_BEHAVIOUR_IT_RISING) ||
              (gpio-> mode == INPUT_BEHAVIOUR_IT_FALLING) ||
              (gpio->mode == INPUT_BEHAVIOUR_IT_RISING_FALLING)) {
    if (gpio->pull == PULL_DIRECTION_NONE) {
      GPIO_PinModeSet(port, port_pin, gpioModeInput, 0);
    } else {
      GPIO_PinModeSet(port, port_pin, gpioModeInputPull, 0);
      hal_gpio_set_pull(port, port_pin, gpio->pull);
    }
  }

  if( ( gpio->mode == INPUT_BEHAVIOUR_IT_RISING ) ||
      ( gpio->mode == INPUT_BEHAVIOUR_IT_FALLING ) ||
      ( gpio->mode == INPUT_BEHAVIOUR_IT_RISING_FALLING ) )
  {
  }
}

/*!
 * Finds the port and pin from a passed HAL pin definition
 *
 * @param [in] pin  The pin to find the details for
 * @param [out] port The associated port structure
 * @param [out] port_pin The associated pin number
 */
static void hal_gpio_find( const hal_gpio_pin_names_t pin, GPIO_Port_TypeDef *port, unsigned int *port_pin)
{
  if (pin < PA_COUNT) {
      *port = gpioPortA;
      *port_pin = pin;
  } else if (pin < (PA_COUNT + PB_COUNT)) {
      *port = gpioPortB;
      *port_pin = pin - PA_COUNT;
  } else if (pin  < (PA_COUNT + PB_COUNT + PC_COUNT)) {
      *port = gpioPortC;
      *port_pin = pin - (PA_COUNT + PB_COUNT);
  } else if (pin  < (PA_COUNT + PB_COUNT + PC_COUNT + PD_COUNT)) {
      *port = gpioPortD;
      *port_pin = pin - (PA_COUNT + PB_COUNT + PC_COUNT);
  } else {

  }
}

/*!
 * Sets the pull down direction for a passed port and pin
 *
 * @param [in] port The associated port structure
 * @param [in] port_pin The associated pin number
 * @param [in] pull The associated pull behaviour
 */
static void hal_gpio_set_pull( GPIO_Port_TypeDef port, unsigned int port_pin, tPullDirection pull)
{
  if (pull == PULL_DIRECTION_UP) {
    GPIO_PinOutSet(port, port_pin);
  } else if (pull == PULL_DIRECTION_DOWN) {
    GPIO_PinOutClear(port, port_pin);
  }
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles EXTI line0 interrupt.
 */
void EXTI0_IRQHandler( void )
{
}

/**
 * @brief This function handles EXTI line1 interrupt.
 */
void EXTI1_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI1_IRQn 0 */

    /* USER CODE END EXTI1_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_1 );

    /* USER CODE BEGIN EXTI1_IRQn 1 */

    /* USER CODE END EXTI1_IRQn 1 */
}

/**
 * @brief This function handles EXTI line2 interrupt.
 */
void EXTI2_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI2_IRQn 0 */

    /* USER CODE END EXTI2_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_2 );
    /* USER CODE BEGIN EXTI2_IRQn 1 */

    /* USER CODE END EXTI2_IRQn 1 */
}

/**
 * @brief This function handles EXTI line3 interrupt.
 */
void EXTI3_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI3_IRQn 0 */

    /* USER CODE END EXTI3_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_3 );
    /* USER CODE BEGIN EXTI3_IRQn 1 */

    /* USER CODE END EXTI3_IRQn 1 */
}

/**
 * @brief This function handles EXTI line4 interrupt.
 */
void EXTI4_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI4_IRQn 0 */

    /* USER CODE END EXTI4_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_4 );
    /* USER CODE BEGIN EXTI4_IRQn 1 */

    /* USER CODE END EXTI4_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    /* USER CODE END EXTI9_5_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_6 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_7 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_9 );
    /* USER CODE BEGIN EXTI9_5_IRQn 1 */

    /* USER CODE END EXTI9_5_IRQn 1 */
}
/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
void EXTI15_10_IRQHandler( void )
{
    /* USER CODE BEGIN EXTI15_15_IRQn 0 */

    /* USER CODE END EXTI15_15_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_10 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11 );
    //H/AL_GPIO_EXTI_IRQHandler( GPIO_PIN_12 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_13 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_14 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
    /* USER CODE BEGIN EXTI15_15_IRQn 1 */

    /* USER CODE END EXTI15_15_IRQn 1 */
}

void HAL_GPIO_EXTI_Callback( uint8_t gpio_pin )
{
    if( ( gpio_irq[gpio_pin] != NULL ) && ( gpio_irq[gpio_pin]->callback != NULL ) )
    {
        gpio_irq[gpio_pin]->callback( gpio_irq[gpio_pin]->context );
    }
}

/* --- EOF ------------------------------------------------------------------ */
