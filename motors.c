#include "stm32f401xc.h"
#include <stdlib.h>
#include <math.h>
#include "motors.h"
#include "Led.h"

uint8_t Duty;

/*! \brief Controls direction and speed for motors.
*
*  \param raw_data  Value from controller
*  \retvalue none
*/
void Motors (float raw_data)
{
	uint8_t duty;
	
	/* Change Direction */
	if (raw_data >= 0.0)
	{
		GPIOA->BSRR = GPIO_BSRR_BR10;
		GPIOE->BSRR = GPIO_BSRR_BR9;
		GPIOD->BSRR = GPIO_BSRR_BS0;
		GPIOE->BSRR = GPIO_BSRR_BS13;
		
		LED(GREEN, ON);
		LED(RED, OFF);
	}
	else
	{
		GPIOD->BSRR = GPIO_BSRR_BR0;
		GPIOE->BSRR = GPIO_BSRR_BR13;
		GPIOA->BSRR = GPIO_BSRR_BS10;
		GPIOE->BSRR = GPIO_BSRR_BS9;
		
		LED(GREEN, OFF);
		LED(RED, ON);
	}
	
	/* Change Speed */
	raw_data = abs(raw_data) + PWM_MIN;
	if (raw_data >= PWM_MAX)
		duty = PWM_MAX;
	else
		duty = (uint8_t)(raw_data);
	
	TIM1->CCR1=duty;
	TIM1->CCR2=duty; 
	
	TIM1->EGR |= TIM_EGR_UG;
	
	Duty = duty; 
}