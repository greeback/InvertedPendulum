#include "stm32f401xc.h"
#include "pid.h"
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
	pid->maxError    = 1000;
	pid->maxSumError = 1000;
	//	pid->maxError    = MAX_INT / (pid->P_Factor + 1);
	//	pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
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
	
	/* Calculate Pterm and limit error overflow */
//	if (errors > pid_st->maxError) 
//	{
//		p_term = MAX_INT;
//	} 
//	else if (errors < -pid_st->maxError) 
//	{
//		p_term = -MAX_INT;
//	} 
//	else 
//	{
		p_term = pid_st->P_Factor * errors;
//	}
	
	/* Calculate Iterm and limit integral runaway */
	temp = pid_st->sumError + errors;
//	if (temp > pid_st->maxSumError) 
//	{
//		i_term           = MAX_I_TERM;
//		pid_st->sumError = pid_st->maxSumError;
//	} 
//	else if (temp < -pid_st->maxSumError) 
//	{
//		i_term           = -MAX_I_TERM;
//		pid_st->sumError = -pid_st->maxSumError;
//	} 
//	else 
//	{
		pid_st->sumError = temp;
		i_term = pid_st->I_Factor * pid_st->sumError * INTEGERATION_TIME_MS * 0.001;
//	}
	
	/* Calculate Dterm */
	d_term = pid_st->D_Factor * (pid_st->lastProcessValue - processValue) / (INTEGERATION_TIME_MS * 0.001);
	
	pid_st->lastProcessValue = processValue;
	
	ret = p_term + i_term + d_term;
//	if (ret > MAX_INT) 
//	{
//		ret = MAX_INT;
//	} 
//	else if (ret < -MAX_INT) 
//	{
//		ret = -MAX_INT;
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