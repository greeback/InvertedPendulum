#ifndef __Led_h__
#define __Led_h__
#include "stm32f401xc.h"

#define GREEN   GPIO_ODR_ODR_12
#define ORANGE  GPIO_ODR_ODR_13 
#define RED     GPIO_ODR_ODR_14
#define BLUE    GPIO_ODR_ODR_15

typedef enum
{
  ON, OFF, TOGGLE
}ActionTypeDef;


/**
 * @brief  This function lights up LEDS which are placed on STM32F401 Discovery board
 * @param  Colour: choose from GREEN, ORANGE, RED, BLUE
           Action: choose from ON, OFF, TOGGLE
 * @retval NULL
 */
void LED(uint32_t Colour, ActionTypeDef Action);
#endif