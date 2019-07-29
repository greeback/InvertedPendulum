#ifndef __PID_H__
#define __PID_H__

#define SCALING_FACTOR 128

/*! \brief P, I and D parameter values
 *
 * The K_P, K_I and K_D values (P, I and D gains)
 * need to be modified to adapt to the application at hand
 */
#define K_P 0.00
#define K_I 0.00
#define K_D 1.00

/**
* @brief  Setpoints and data used by the PID control algorithm
*/
typedef struct PID_DATA 
{
	//! Last process value, used to find derivative of process value.
	float lastProcessValue;
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
} pidData_t;

/*! \brief Maximum values
*
* Needed to avoid sign/overflow problems
*/
/* Maximum value of variables */
#define MAX_INT INT16_MAX
#define MAX_LONG INT32_MAX
#define MAX_I_TERM (MAX_LONG / 2)



/* Fuctions prototypes */
void pid_Init(float p_factor, float i_factor, float d_factor, struct PID_DATA *pid);
float pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st);
void    pid_Reset_Integrator(pidData_t *pid_st);
#endif //__PID_H__