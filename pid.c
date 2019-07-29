#include "stm32f401xc.h"
#include "pid.h"
#include "motors.h"
#include "main.h"

/*! \brief Initialization of PID controller parameters.
*
*  Initialize the variables used by the PID algorithm.
*
*  \param p_factor  Proportional term.
*  \param i_factor  Integral term.
*  \param d_factor  Derivate term.
*  \param pid  Struct with PID status.
*/
void pid_Init(float p_factor, float i_factor, float d_factor, struct PID_DATA *pid)
{
	/* Start values for PID controller */
	pid->sumError         = 0;
	pid->lastProcessValue = 0;
	/* Tuning constants for PID loop */
	pid->P_Factor = p_factor;
	pid->I_Factor = i_factor;
	pid->D_Factor = d_factor;
	/* Limits to avoid overflow */
	pid->maxValue	= PWM_MAX;
	pid->minValue 	= PWM_MIN;
	pid->prev_i_term = 0;
}

/*! \brief PID control algorithm.
*
*  Calculates output from setpoint, process value and PID status.
*
*  \param setPoint  Desired value.
*  \param processValue  Measured value.
*  \param pid_st  PID status struct.
*/
float pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st)
{
	float errors, p_term, d_term;
	float i_term, temp;
	float ret;
	
	errors = setPoint - processValue;
	
	/* Calculate Pterm */
	p_term = pid_st->P_Factor * errors;
	
	/* Calculate Iterm */	
	i_term = pid_st->I_Factor * INTEGERATION_TIME_MS * 0.001 * (pid_st->prevError + errors) / 2 + pid_st->prev_i_term;
	
	/* Calculate Dterm */
	d_term = pid_st->D_Factor * (errors - pid_st->prevError) / (INTEGERATION_TIME_MS * 0.001);
	pid_st->prevError = errors;
	
	/* Calculate output value */
	ret = p_term + i_term + d_term;
	
	/* Limit output value */
//	if((ret > pid_st->maxValue) || (ret < pid_st->minValue)) 
//	{ 
//		if(ret > pid_st->maxValue) 
//		{ 
//			ret = pid_st->maxValue; 
//		} 
//		else if(ret < pid_st->minValue) 
//		{ 
//			ret = pid_st->minValue;
//		} 
//	} 
//	else 
//	{ 
		pid_st->prev_i_term = i_term; 
//	} 
	
	return ret;
}

/*! \brief Resets the integrator.
*
*  Calling this function will reset the integrator in the PID regulator.
*/
void pid_Reset_Integrator(pidData_t *pid_st)
{
	pid_st->sumError = 0;
}