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
*  \retvalue none
*/
void pid_Init(float p_factor, float i_factor, float d_factor, struct PID_DATA *pid)
{
	/* Start values for PID controller */
	pid->sumError         = 0;
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
*  \retvalue pid output value
*/
float pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st)
{
	float error, p_term, d_term, i_term, ret;

	error = setPoint - processValue;
	
	/* Calculate Pterm */
	p_term = pid_st->P_Factor * error;
	
	/* Calculate Iterm */	
	i_term = pid_st->I_Factor * INTEGERATION_TIME_MS * 0.001 * (pid_st->prevError + error) / 2 + pid_st->prev_i_term;
	
	/* Calculate Dterm */
	d_term = pid_st->D_Factor * (error - pid_st->prevError) / (INTEGERATION_TIME_MS * 0.001);
	pid_st->prevError = error;
	
	/* Calculate output value */
	ret = p_term + i_term + d_term;
	
	/* Update prev_i_term only when output is in acceptable range. */
	if((ret <= pid_st->maxValue) && (ret >= -pid_st->maxValue)) 
		pid_st->prev_i_term = i_term; 

	
	return ret;
}
