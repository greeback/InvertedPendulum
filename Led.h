/** 
* @file    Led.h
* @author  Aleksander Dykman
* @email   aleksanderdykman@gmail.com
* @version v1.0
* @ide     IAR Embedded Wokbench for ARM
* @brief   LED Library for stm32f401vc discovery board
*
\verbatim
----------------------------------------------------------------------
Copyright (c) 2019 Aleksander Dykman
----------------------------------------------------------------------
\endverbatim
*/
#ifndef __Led_h__
#define __Led_h__
#include "stm32f401xc.h"
#include "stm32f4_gpio.h"

/**
 * @addtogroup LED
 * @{
 */
/**
* @defgroup LED_Macros
* @brief    Library defines
* @{
*/
#define GREEN   GPIO_ODR_ODR_12
#define ORANGE  GPIO_ODR_ODR_13 
#define RED     GPIO_ODR_ODR_14
#define BLUE    GPIO_ODR_ODR_15
/**
 * @}
 */
extern GPIO_Handle_t led_green, led_orange, led_red, led_blue;
/**
* @defgroup LED_Typedefs
* @brief    LED Typedefs used for initialization and operating lEDs
* @{
*/
typedef enum
{
  ON, OFF, TOGGLE
}ActionTypeDef;
/**
 * @}
 */
/**
* @defgroup LED_Functions
* @brief    LED Functions
* @{
*/
/**
 * @brief  This function lights up LEDS which are placed on STM32F401 Discovery board
 * @param  Colour: choose from GREEN, ORANGE, RED, BLUE
 * @param  Action: choose from ON, OFF, TOGGLE
 */
void LED(uint32_t Colour, ActionTypeDef Action);
/**
 * @brief  This function initialize appropriate GPIOS to operate LEDs
 */
void LED_Init (void);
/**
 * @}
 */
/**
 * @}
 */
#endif
