/** 
 * @file    pid.h
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   PID Library 
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef __PID_H__
#define __PID_H__

/**
 * @addtogroup PID
 * @{
 */
/**
* @defgroup PID_Macros
* @brief    Library defines
* @{
*/

#define SCALING_FACTOR 128
#define K_P 5.00	//! need to be modified to adapt to the application at hand
#define K_I 0.00	//! need to be modified to adapt to the application at hand
#define K_D 0.00	//! need to be modified to adapt to the application at hand
/**
 * @}
 */
/**
* @defgroup PID_Structures
* @brief    PID Structures used for storing data connected with pid
* @{
*/

/**
* @brief  Setpoints and data used by the PID control algorithm
*/
typedef struct PID_DATA 
{
	//! Summation of errors, used for integrate calculations
	float sumError;
	//! The Proportional tuning constant, multiplied with SCALING_FACTOR
	float P_Factor;
	//! The Integral tuning constant, multiplied with SCALING_FACTOR
	float I_Factor;
	//! The Derivative tuning constant, multiplied with SCALING_FACTOR
	float D_Factor;
	//! Maximum allowed output value
	float maxValue;
	//! Minimum allowed output value
	float minValue;
	//! Error from previous iteration
	float prevError;
	//! Error from previous iteration
	float prev_i_term;
} pidData_typedef;
/**
 * @}
 */

/**
* @defgroup PID_Functions
* @brief    PID Functions used for initialization and processing the output value
* @{
*/
/**
* @brief  Initializes PID 
* @param  p_factor: Proportional coefficient
* @param  i_factor: Integral coefficient
* @param  d_factor: Differential coefficient
* @param  *pid: 	Pointer to pid structure
*/
void pid_Init(float p_factor, float i_factor, float d_factor, struct PID_DATA *pid);
/**
* @brief  Calculates output value
* @param  setPoint: 	Desired value
* @param  processValue: Current value
* @param  *pid: 		Pointer to pid structure
* @retval pid output value
*/
float pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st);
/**
 * @}
 */
/**
 * @}
 */
#endif //__PID_H__
