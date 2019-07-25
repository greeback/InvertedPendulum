#include "stm32f401xc.h"
#include "pid.h"


int8_t PID (float error, float Kp, float Ki, float Kd)
{
  float processed_value;
  processed_value = Kp * error;
  return (int8_t)processed_value;
}