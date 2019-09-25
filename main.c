#include "stm32f401xc.h"
#include "delay.h"
#include "L3GD20.h"
#include "main.h"
#include "Led.h"
#include "stm32f4xx_it.h"
#include "LSM303DLHC.h"
#include <math.h>
#include "fuzzy_pid.h"
#include "pid.h"
#include "motors.h"

L3GD20_Data_t Gyro_Data;
LSM303DLHC_Data_t Accelerometer_Data;
float Tilt, Tilt_A, Tilt_G, dt,Processed_value;
pidData_typedef pidData;

uint8_t zmienna;
float srednia;
long double suma;
uint32_t i;

int main()
{	
	Init();
	
	while (1)
	{
		i++;
		L3GD20_read_rates (&Gyro_Data);
		suma += Gyro_Data.X;
		srednia = suma / i;
		
		if(dt_flag)
		{
			LED(ORANGE, TOGGLE);
			dt_flag = 0;
			/* Read data from Gyro and Accelerometer */
			L3GD20_read_rates (&Gyro_Data);
			LSM303DLHC_read_rates(&Accelerometer_Data);
			
			/* Calculate Tilt from Gyro and Accelerometer */
			Tilt_G = Tilt + (-1)*Gyro_Data.X * INTEGERATION_TIME_MS * 0.001;
			Tilt_A = atanf((float)Accelerometer_Data.Y / (float)Accelerometer_Data.Z) * RAD_TO_DEG;
			
			/* Complementary filter */
			Tilt=(0.9 * Tilt_G + 0.1 * Tilt_A);
			
			/* Control motors output */
			Processed_value = pid_Controller (0, (Tilt + ROBOT_OFF_BALANCE), &pidData);
			//Processed_value = fuzzy_pid_Controller (0, (Tilt + ROBOT_OFF_BALANCE), &pidData);
			
			Motors (Processed_value);
		}
	}
	return 0;
}

void Init()
{
	/******  Clocks ******/
	/*Wait until crystal is ready, turn it on and set as the system clock.
	8Mhz crystal is more stable than internal clock.*/
	while (RCC->CR & RCC_CR_HSERDY != RCC_CR_HSERDY); 
	RCC->CR |= RCC_CR_HSEON; 
	RCC->CFGR |= RCC_CFGR_SW_HSE;
	
	/*Enable peripherials*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	
	/******  LEDS ******/
	LED_Init();
	
	/******  Motors - TIM1 for PWM and GPIOs for direction ******/
	Motors_Init();
	
	/******  Systick - every 1ms ******/
	SysTick_Config(8000);
	
	/****** L3GD20 (Gyro) - SPI1 communication ******/
	L3GD20_Init();
	
	/****** LSM303DLHC (Accelerometer) - I2C1 communication ******/
	LSM303DLHC_Init();
	
	/******  TIM10 (1KHz) - Integration period calculation ******/
	TIM10->DIER |= TIM_DIER_UIE; //Update interrupt enable
	TIM10->PSC = INTEGERATION_TIME_MS-1;
	TIM10->ARR = 8000-1;
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	TIM10->CR1 |= TIM_CR1_CEN;
	
	/******  PID Init ******/
	pid_Init (K_P, K_I, K_D, &pidData);
}

