#ifndef __MAIN_H__
#define __MAIN_H_

#define PWM_MAX 100
#define INTEGERATION_TIME_MS    10

//Functions Prototypes
void Init();
void MOTORS (uint8_t direction, uint8_t duty);
#endif //__MAIN_H__