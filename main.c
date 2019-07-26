#include "stm32f401xc.h"
#include "delay.h"
#include "L3GD20.h"
#include "main.h"
#include "Led.h"
#include "stm32f4xx_it.h"
#include "LSM303DLHC.h"
#include <math.h>
#include "pid.h"

L3GD20_Data_t Gyro_Data;
LSM303DLHC_Data_t Accelerometer_Data;
float Tilt, Tilt_A, Tilt_G;
float dt = 0.0;
pidData_t pidData;
float Processed_value;


float srednia = 0.0;
long double suma = 0.0;
uint32_t i = 0;
int main()
{
	Init();
	
	while (1)
	{
		LED(RED, TOGGLE);
		
		i++;
		L3GD20_read_rates (&Gyro_Data);
		suma += Gyro_Data.X;
		srednia = suma / i;
		
		if(dt_flag)
		{
			
			
			/* Read data from Gyro and Accelerometer */
			L3GD20_read_rates (&Gyro_Data);
			LSM303DLHC_read_rates(&Accelerometer_Data);
			
			/* Calculate Tilt from Gyro and Accelerometer */
			Tilt_G = Tilt + (-1)*Gyro_Data.X * INTEGERATION_TIME_MS * 0.001;
			Tilt_A = atanf((float)Accelerometer_Data.Y / (float)Accelerometer_Data.Z) * 57.29578;
			
			/* Complementary filter */
			Tilt=(0.98*Tilt_G+0.02*Tilt_A);
	
			
			Processed_value = pid_Controller (0, Tilt, &pidData);
			//      if (zmienna)
			//      {
			//        LED(GREEN, TOGGLE);
			//        MOTORS (1,30);
			//        zmienna = 0;
			//        
			//      }
			//      else 
			//      {
			//        LED(ORANGE, TOGGLE);
			//        MOTORS (0,80);
			//        zmienna = 1; 
			//      }
			dt_flag = 0;
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
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_TIM10EN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	/******  LEDS ******/
	GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	
	/******  Motor Direction ******/
	//GPIOE->MODER |= GPIO_MODER_MODER9_0 | GPIO_MODER_MODER13_0; //PE9, PE13
	GPIOD->MODER |= GPIO_MODER_MODER0_0; //PD0
	GPIOA->MODER |= GPIO_MODER_MODER10_0; //PA10
	
	/******  Systick - every 1ms ******/
	SysTick_Config(8000);
	
	/******  TIM1 - Motor PWM ******/
	GPIOA->MODER |= GPIO_MODER_MODER8_1; //PA8
	GPIOA->AFR[1] = 0x00000001;
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->ARR = 99;
	TIM1->PSC = 15; //10kHz
	TIM1->EGR |= TIM_EGR_UG;
	TIM1->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
	
	/******  SPI1 - L3GD20 (Gyro) ******/
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 |GPIO_MODER_MODER7_1; //PA5(SCK), PA6(MISO), PA7(MOSI)
	GPIOA->AFR[0]|= 0x55500000; //PA5 - SCK, PA6 - MISO, PA7 - MOSI
	GPIOE->MODER |= GPIO_MODER_MODER3_0; //PE3 (CS)
	GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0; //Pull-Up
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_MSTR;
	L3GD20_init();
	
	/******  I2C1 - LSM303DLHC (Accelerometer) ******/
	GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER9_1; //PB6(SCL), PB9(SDA)
	GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT9; //Open-drain
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD9_0; //Pull-up
	GPIOB->AFR[0] |= 0x04000000; //PB6 - SCL
	GPIOB->AFR[1] |= 0x00000040; //PB9 - SDA
	
	I2C1->CR2 |= 8; //Assign 8MHz peripherial clock
	I2C1->CCR |= 40; //Standard mode, SCL frequency = 100kHz
	I2C1->TRISE = 9; //Calculated according to reference manual
	I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK; //Enable I2C and Acknowledges
	//I2C1->CR1 |= I2C_CR1_PE; //Enable I2C
	LSM303DLHC_init ();
	
	/******  TIM10 (1KHz) - Integration period calculation ******/
	TIM10->DIER |= TIM_DIER_UIE; //Update interrupt enable
	TIM10->PSC = INTEGERATION_TIME_MS-1;
	TIM10->ARR = 8000-1;
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	TIM10->CR1 |= TIM_CR1_CEN;
	
	/******  PID Init ******/
	pid_Init (K_P, K_I, K_D, &pidData);
}

void MOTORS (uint8_t direction, uint8_t duty)
{
	if (duty>=PWM_MAX)
	{
		TIM1->CCR1=PWM_MAX;
		TIM1->CCR2=PWM_MAX;
	}
	else
	{
		TIM1->CCR1=duty;
		TIM1->CCR2=duty; 
	}
	TIM1->EGR |= TIM_EGR_UG;
	if (direction)
	{
		GPIOD->BSRR = GPIO_BSRR_BR0;
		GPIOE->BSRR = GPIO_BSRR_BR13;
		GPIOA->BSRR = GPIO_BSRR_BS10;
		GPIOE->BSRR = GPIO_BSRR_BS9;
	}
	else
	{
		GPIOA->BSRR = GPIO_BSRR_BR10;
		GPIOE->BSRR = GPIO_BSRR_BR9;
		GPIOD->BSRR = GPIO_BSRR_BS0;
		GPIOE->BSRR = GPIO_BSRR_BS13;
	}
}