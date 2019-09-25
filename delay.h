
/** 
* @file    delay.h
* @author  Aleksander Dykman
* @email   aleksanderdykman@gmail.com
* @version v1.0
* @ide     IAR Embedded Wokbench for ARM
* @brief   delay Library for STM32F4xx devices
*
\verbatim
----------------------------------------------------------------------
Copyright (c) 2019 Aleksander Dykman
----------------------------------------------------------------------
\endverbatim
*/
#ifndef __delay_h__
#define __delay_h__
#include "stm32f401xc.h"

/**
 * @addtogroup delay
 * @{
 */
/**
* @defgroup delay_Functions
* @{
*/
/**
* @brief  Decrements sysTickCounter until it reaches 0
* @param  n: how many system clock ticks shall we wait till delay is over
*/
void TimeTick_Decrement(void);
/**
* @brief  Sets sysTickCounter to appropriate value and waits untill it reaches 0
* @param  n: how many system clock ticks shall we wait till delay is over
*/
void delay_ms(uint32_t n); 
/**
 * @}
 */
/**
 * @}
 */
#endif
