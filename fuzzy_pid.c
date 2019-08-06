#include "fuzzy_pid.h"

/*! \brief This function handles calculation of membership in rising slope 
*			of triangle shape membership function 
*
*  \param var  			X axis value. Value that need to be fuzyficated.
*  \param lim_start		Y axis value. Beginning of slope.
*  \param lim_end  		Y axis value. End of slope.
*  \retvalue 			membership value (0 - 1)
*/
static float rising_slope (float var, float lim_start, float lim_end)
{
	return ((var - lim_start) / (lim_end - lim_start));
}

/*! \brief This function handles calculation of membership in falling slope 
*			of triangle shape membership function 
*
*  \param var  			X axis value. Value that need to be fuzyficated.
*  \param lim_start		Y axis value. Beginning of slope.
*  \param lim_end  		Y axis value. End of slope.
*  \retvalue 			membership value (0 - 1)
*/
static float falling_slope (float var, float lim_start, float lim_end)
{
	return ((var - lim_end) / (lim_start - lim_end));
}

/*! \brief  This function calculates membership values of error. 
*			It's tailored to triangle shaped membership functions. Begining and 
*			end of each triangle is defined as E1...E5.
*
*  \param error  	difference between measured and set value
*  \retvalue 		membership values 
*/
static fuzzy_input_typedef error_fuzzyfication (float error)
{
	/* Initialize membership functions to 0 */
	fuzzy_input_typedef fe = {0, 0, 0, 0, 0}; 
	
	if (error < E1)
	{
		fe.NN = 1;
	}
	else if (error >= E1 && error < E2)
	{
		fe.NN = falling_slope(error, E1, E2); 
		fe.N = rising_slope(error, E1, E2);
	}
	else if (error >= E2 && error < E3)
	{
		fe.N = falling_slope(error, E2, E3);
		fe.Z = rising_slope(error, E2, E3); 
	}
	else if (error >= E3 && error < E4)
	{
		fe.Z = falling_slope(error, E3, E4);
		fe.P = rising_slope(error, E3, E4); 
	}
	else if (error >= E4 && error < E5)
	{
		fe.P = falling_slope(error, E4, E5);
		fe.PP = rising_slope(error, E4, E5); 
	}
	else if (error >= E5)
	{
		fe.PP = 1;
	}
	return fe;
}

/*! \brief  This function determines output pid coefficients.
*
*  \param fe  	calculated error membership functions
*  \param ret	pointer to pidData structure
*  \retvalue 	none 
*/
static void if_then_engine (fuzzy_input_typedef fe, pidData_typedef* ret)
{	
	fuzzy_output_typedef Kp = {0, 0, 0};
	fuzzy_output_typedef Ki = {0, 0, 0};
	fuzzy_output_typedef Kd = {0, 0, 0};
	
	if (fe.NN)	{Kp.YYY = fe.NN * KP3;		Ki.YYY = fe.NN * KI3;		Kd.YYY = fe.NN * KD3;}	
	if (fe.N)	{Kp.YY = fe.N * KP2;		Ki.YY = fe.N * KI2;			Kd.YY = fe.N * KD2;}	
	if (fe.Z)	{Kp.Y = fe.Z * KP1;			Ki.Y = fe.Z * KI1;			Kd.Y = fe.Z * KD1;}	
	if (fe.P)	{Kp.YY = fe.P * KP2;		Ki.YY = fe.P * KI2;			Kd.YY = fe.P * KD2;}	
	if (fe.PP)	{Kp.YYY = fe.PP * KP3;		Ki.YYY = fe.PP * KI3;		Kd.YYY = fe.PP * KD3;}	
	
	ret->P_Factor = Kp.Y + Kp.YY + Kp.YYY;
	ret->I_Factor = Ki.Y + Ki.YY + Ki.YYY;
	ret->D_Factor = Kd.Y + Kd.YY + Kd.YYY;
}

/*! \brief  This function calculates error, fuzzyficate it, determines pid coefficient 
*			and finally invokes standard pid controller.
*
*  \param setPoint  	Desired value.
*  \param processValue  Measured value.
*  \param pid_st  		PID status struct.		
*  \retvalue 			pid output value
*/
float fuzzy_pid_Controller(float setPoint, float processValue, pidData_typedef *pid_st)
{
	float error, ret;
	fuzzy_input_typedef fuzzy_error;
	error = setPoint - processValue;
	
	fuzzy_error = error_fuzzyfication(error);
	if_then_engine (fuzzy_error, pid_st);
	ret = (pid_Controller (0, processValue, pid_st));
	return ret;
}