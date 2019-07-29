#include "stm32f401xc.h"
#include <stdlib.h>
#include <math.h>
#include "motors.h"
#include "Led.h"

uint8_t Duty;

void Motors (float raw_data)
{
	uint8_t duty;
	duty = (uint8_t)(abs(raw_data)) + PWM_MIN; 
	
	if (duty >= PWM_MAX)
		duty = PWM_MAX;

	TIM1->CCR1=duty;
	TIM1->CCR2=duty; 
	
	TIM1->EGR |= TIM_EGR_UG;
	
	if (raw_data >= 0.0)
	{
		GPIOA->BSRR = GPIO_BSRR_BR10;
		GPIOE->BSRR = GPIO_BSRR_BR9;
		GPIOD->BSRR = GPIO_BSRR_BS0;
		GPIOE->BSRR = GPIO_BSRR_BS13;
		
		LED(GREEN, ON);
		LED(ORANGE, OFF);
	}
	else
	{
		GPIOD->BSRR = GPIO_BSRR_BR0;
		GPIOE->BSRR = GPIO_BSRR_BR13;
		GPIOA->BSRR = GPIO_BSRR_BS10;
		GPIOE->BSRR = GPIO_BSRR_BS9;
		
		LED(GREEN, OFF);
		LED(ORANGE, ON);
	}
	Duty = duty; 
}