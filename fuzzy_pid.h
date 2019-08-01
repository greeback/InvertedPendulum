#ifndef __FUZZY_PID_H__
#define __FUZZY_PID_H__

/* Fuzzy linguistic terms */
typedef struct
{
	float NN;
	float N;
	float Z;
	float P;
	float PP;
}fuzzy_error_typedef;


/* Error transition values */
#define E1	-30
#define E2	-15
#define E3	-0
#define E4	15
#define E5	30

#endif //__FUZZZY_PID_H__
