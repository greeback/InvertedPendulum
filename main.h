/** 
* @file    main.h
* @author  Aleksander Dykman
* @email   aleksanderdykman@gmail.com
* @version v1.0
* @ide     IAR Embedded Wokbench for ARM
* @brief   Main header file
*
\verbatim
----------------------------------------------------------------------
Copyright (c) 2019 Aleksander Dykman
----------------------------------------------------------------------
\endverbatim
*/

#ifndef __MAIN_H__
#define __MAIN_H_

/**
 * @addtogroup main
 * @{
 */
/**
* @defgroup main_Macros
* @brief    main defines
* @{
*/

#define INTEGERATION_TIME_MS    10
#define RAD_TO_DEG				57.29578

/* 	It's an angle in degrees, where whole robot is in stable position
	(measuerd experimentally) */
#define ROBOT_OFF_BALANCE		-0.0
/**
 * @}
 */

/**
* @defgroup Main_Functions
* @brief    Main Functions used in main function
* @{
*/

/**
* @brief  Initializes every peripheral needed
*/
void Init();
/**
 * @}
 */
/**
 * @}
 */
#endif //__MAIN_H__
