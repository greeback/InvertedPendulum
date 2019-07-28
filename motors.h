#ifndef __MOTORS_H__
#define __MOTORS_H__

#define PWM_MAX         100 // Above this value motors won't produce more power
#define PWM_MIN         20  // Below this value motors are not responding

//Functions Prototypes
void Motors (float raw_data);
#endif //__MOTORS_H__