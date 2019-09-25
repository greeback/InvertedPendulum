#include "delay.h"
static __IO uint32_t sysTickCounter;

void TimeTick_Decrement(void) 
{
  if (sysTickCounter != 0x00) 
  {
    sysTickCounter--;
  }
}

void delay_ms(uint32_t n) 
{
  sysTickCounter = n;
  while (sysTickCounter != 0); 
}



