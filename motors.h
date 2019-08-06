#ifndef __MOTORS_H__
#define __MOTORS_H__



/* Below this value motors are not able to move robot (measuerd experimentally) */
#define PWM_MIN         0

/* Above this value motors won't produce more power (measuerd experimentally) */
#define PWM_MAX         100.0

//Functions Prototypes
void Motors (float raw_data);
#endif //__MOTORS_H__