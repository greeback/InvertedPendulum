#ifndef __Led_h__
#define __Led_h__
#include "stm32f401xc.h"
#include "stm32f4_gpio.h"

#define GREEN   GPIO_ODR_ODR_12
#define ORANGE  GPIO_ODR_ODR_13 
#define RED     GPIO_ODR_ODR_14
#define BLUE    GPIO_ODR_ODR_15

extern GPIO_Handle_t led_green, led_orange, led_red, led_blue;

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
void LED_Init (void);
#endif