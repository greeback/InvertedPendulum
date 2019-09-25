/** 
* @file    motors.h
* @author  Aleksander Dykman
* @email   aleksanderdykman@gmail.com
* @version v1.0
* @ide     IAR Embedded Wokbench for ARM
* @brief   Motor Control for STM32F401VC discovery
*
\verbatim
----------------------------------------------------------------------
Copyright (c) 2019 Aleksander Dykman
----------------------------------------------------------------------
\endverbatim
*/
#ifndef __MOTORS_H__
#define __MOTORS_H__

/**
 * @addtogroup Motors_Driver
 * @{
 */
/**
* @defgroup Motors_Macros
* @brief    Library defines
* @{
*/
/* Below this value motors are not able to move robot (measuerd experimentally) */
#define PWM_MIN         0

/* Above this value motors won't produce more power (measuerd experimentally) */
#define PWM_MAX         100.0

/**
 * @}
 */

/**
* @defgroup Motors_Functions
* @brief    Motors Functions used for Motors handling
* @{
*/

/**
* @brief  Controls Motors output PWM and direction
* @param  raw_data: value to be transform to PWM and direction
*/
void Motors (float raw_data);

/**
* @brief  Initializes Motors GPIOS and PWM timers
*/
void Motors_Init (void);

/**
 * @}
 */
/**
 * @}
 */
#endif //__MOTORS_H__