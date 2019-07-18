#include "stm32f401xc.h"
#include "delay.h"
#include "L3GD20.h"
#include "main.h"

L3GD20_Data_t Gyro_Data;
uint8_t zmienna;
float Tilt = 0.0;
int32_t time_now, time_before;
float dt = 0.0;

int main()
{
  Init();
  
  int16_t cos = 0xA6B7;
  
  
  
  while (1)
  {
    
    
    L3GD20_read_rates (&Gyro_Data);
    dt = TIM10->CNT * 0.000000125;
    TIM10->CNT = 0;
    Tilt += Gyro_Data.X * dt;
    
    if (zmienna)
    {
      GPIOD->ODR ^= GPIO_ODR_ODR_12;
      MOTORS (1,30);
      zmienna = 0;
      
    }
    else 
    {
      GPIOD->ODR ^= GPIO_ODR_ODR_13;
      MOTORS (0,80);
      zmienna = 1; 
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
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SPI4EN | RCC_APB2ENR_TIM10EN;
  
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
  I2C1->CR1 |= I2C_CR1_PE;
  
  /******  TIM10 (1KHz) - Integration period calculation ******/
  //TIM10->PSC = 999;
  //TIM10->ARR = 7;
  TIM10->CR1 |= TIM_CR1_CEN;
  
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