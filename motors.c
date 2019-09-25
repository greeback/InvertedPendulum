/**	
* |----------------------------------------------------------------------
* | Copyright (c) 2019 Aleksander Dykman
* |
* |----------------------------------------------------------------------
*/
#include "stm32f401xc.h"
#include <stdlib.h>
#include <math.h>
#include "motors.h"
#include "Led.h"
#include "stm32f4_gpio.h"

uint8_t Duty;
static GPIO_Handle_t LeftWheelForw, LeftWheelBackw, RightWheelForw, RightWheelBackw;

void Motors (float raw_data)
{
	uint8_t duty;
	
	/* Change Direction */
	if (raw_data >= 0.0)
	{
		GPIO_ClearPin	(&LeftWheelForw);
		GPIO_ClearPin	(&RightWheelForw);
		GPIO_SetPin		(&LeftWheelBackw);
		GPIO_SetPin		(&RightWheelBackw);
		
		LED(GREEN, ON);
		LED(RED, OFF);
	}
	else
	{
		GPIO_ClearPin	(&LeftWheelBackw);
		GPIO_ClearPin	(&RightWheelBackw);
		GPIO_SetPin		(&LeftWheelForw);
		GPIO_SetPin		(&RightWheelForw);
		
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

void Motors_Init ()
{
	/* GPIO direction configuration */
	GPIO_Config_t DirectionConfig;
	
	DirectionConfig.Mode 	= GPIO_Mode_OUT;
	DirectionConfig.OType	= GPIO_OType_PP;
	DirectionConfig.PuPd	= GPIO_PuPd_NOPULL;
	DirectionConfig.Speed	= GPIO_Speed_Low;
	
	LeftWheelForw.Base	= GPIOA;
	LeftWheelForw.Pin	= 10;
	GPIO_Init(&LeftWheelForw, &DirectionConfig);
	
	LeftWheelBackw.Base = GPIOD;
	LeftWheelBackw.Pin	= 0;
	GPIO_Init(&LeftWheelBackw, &DirectionConfig);
	
	RightWheelForw.Base	= GPIOE;
	RightWheelForw.Pin	= 9;
	GPIO_Init(&RightWheelForw, &DirectionConfig);
	
	RightWheelBackw.Base = GPIOE;
	RightWheelBackw.Pin	= 13;
	GPIO_Init(&RightWheelBackw, &DirectionConfig);
	
	/* PWM Configuration */
	GPIO_Config_t PWMConfig;
	GPIO_Handle_t LeftWheelPWM, RightWheelPWM;
	
	PWMConfig.Mode 		= GPIO_Mode_AF;
	PWMConfig.PuPd 		= GPIO_PuPd_NOPULL;
	PWMConfig.AltFun 	= 1;
	
	LeftWheelPWM.Base 	= GPIOA;
	LeftWheelPWM.Pin	= 8;
	GPIO_Init(&LeftWheelPWM, &PWMConfig);
	
	RightWheelPWM.Base	= GPIOE;
	RightWheelPWM.Pin	= 11;
	GPIO_Init(&RightWheelPWM, &PWMConfig);
	
	/* Timers driver is not ready yet so this is only temporary */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->ARR = 99;
	TIM1->PSC = 15; //10kHz
	TIM1->EGR |= TIM_EGR_UG;
	TIM1->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}