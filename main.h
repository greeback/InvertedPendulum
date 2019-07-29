#ifndef __MAIN_H__
#define __MAIN_H_


#define INTEGERATION_TIME_MS    10

/* 	It's an angle in degrees, where whole robot is in stable position
	(measuerd experimentally) */
#define ROBOT_OFF_BALANCE		-0.4

//Functions Prototypes
void Init();
void MOTORS (uint8_t direction, uint8_t duty);
#endif //__MAIN_H__