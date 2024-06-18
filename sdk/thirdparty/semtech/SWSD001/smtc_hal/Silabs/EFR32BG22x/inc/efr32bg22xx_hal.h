/*****************************************************************************
  Filename:       efr32bg22xx_hal.h
  Revised:        13 Nov 2022
  Revision:       1.0

  Description: GPIO hal definitions

  Copyright 2022 Ezurio. All rights reserved.

  The information contained herein is property of Ezurio.
  Terms and conditions of usage are described in detail in EZURIO STANDARD
  SOFTWARE LICENSE AGREEMENT.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  EZURIO OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE,
  STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO
  ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST
  PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,
  SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY
  DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Ezurio at www.ezurio.com.
*******************************************************************************/

#ifndef EFR32BG22XX_HAL_H_
#define EFR32BG22XX_HAL_H_

#include "em_gpio.h"
#include "smtc_hal_gpio_pin_names.h"

typedef enum _tPullDirection
{
  PULL_DIRECTION_NONE = 0,
  PULL_DIRECTION_UP,
  PULL_DIRECTION_DOWN,
  PULL_DIRECTION_COUNT
}tPullDirection;

typedef enum _tInputBehaviour
{
  INPUT_BEHAVIOUR_INPUT = 0,
  INPUT_BEHAVIOUR_IT_RISING,
  INPUT_BEHAVIOUR_IT_FALLING,
  INPUT_BEHAVIOUR_IT_RISING_FALLING,
  INPUT_BEHAVIOUR_COUNT
}tInputBehaviour;

#endif /* EFR32BG22XX_HAL_H_ */
