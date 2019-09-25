/** 
 * @file    fuzzy_pid.h
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   Fuzzy PID Library 
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef __FUZZY_PID_H__
#define __FUZZY_PID_H__
#include "pid.h"

/**
 * @addtogroup Fuzzy_PID
 * @{
 */
/**
* @defgroup Fuzzy_PID_Structures
* @brief    Fuzzy_PID Structures used for Fuzzy_PID handling
* @{
*/
/**
* @brief Error fuzzy linguistic terms
*/
typedef struct
{
	float NN;	/*!< Large Negative */
	float N;	/*!< Small Negative */
	float Z;	/*!< Around Zero */
	float P;	/*!< Small Positive */ 
	float PP;	/*!< Large Positive */ 
}fuzzy_input_typedef;


/**
* @brief Output fuzzy linguistic terms
*/
typedef struct
{
	float Y;	/*!< Chilled */  
	float YY;	/*!< Worried */  
	float YYY;	/*!< Desperate */  
}fuzzy_output_typedef;
/**
 * @}
 */
/**
* @defgroup Fuzzy_PID_Macros
* @brief    Library defines
* @{
*/
/* Error transition values */
#define E1	-30
#define E2	-15
#define E3	0
#define E4	15
#define E5	30

/* Kp transition values */
#define KP1	0
#define KP2	0
#define KP3	0

/* Ki transition values */
#define KI1	1
#define KI2	1
#define KI3	1

/* Kd transition values */
#define KD1	0
#define KD2	0
#define KD3	0
/**
 * @}
 */
/**
* @defgroup Fuzzy_PID_Functions
* @brief    Fuzzy_PID Functions used for Fuzzy_PID handling
* @{
*/
/**
* @brief  Calculates output value
* @param  setPoint: 	Desired value
* @param  processValue: Current value
* @param  *pid: 		Pointer to pid structure
* @retval pid output value
*/
float fuzzy_pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st);
/**
 * @}
 */
/**
 * @}
 */
#endif //__FUZZZY_PID_H__
