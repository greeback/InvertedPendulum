#include "fuzzy_pid.h"
#include "pid.h"

static fuzzy_error_typedef error_fuzzyfication (float error)
{
	fuzzy_error_typedef fe;
	if (error < E1)
	{
		fe.NP =0;
	}
	else if (error >= E1 && error < E2)
	{
		
	}
	else if (error >= E2 && error < E3)
	{
		
	}
	else if (error >= E3 && error < E4)
	{
		
	}
	else if (error >= E4 && error < E5)
	{
		
	}
	else if (error >= E5)
	{
		
	}
}

float fuzzy_pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st)
{
	
}