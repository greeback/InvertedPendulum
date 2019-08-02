#ifndef __FUZZY_PID_H__
#define __FUZZY_PID_H__
#include "pid.h"

/* Error fuzzy linguistic terms */
typedef struct
{
	float NN;	// Large Negative
	float N;	// Small Negative
	float Z;	// Around Zero
	float P;	// Small Positive
	float PP;	// Large Positive
}fuzzy_input_typedef;

/* Output fuzzy linguistic terms */
typedef struct
{
	float Y;	// Chilled
	float YY;	// Worried
	float YYY;	// Desperate
}fuzzy_output_typedef;

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

/* Fuctions prototypes */
float fuzzy_pid_Controller(float setPoint, float processValue, struct PID_DATA *pid_st);
#endif //__FUZZZY_PID_H__
