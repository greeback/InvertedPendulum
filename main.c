#include "stm32f4xx.h"
#include "delay.h"
#define PWM_MAX 100

//Functions Prototypes
void Init();
void MOTORS (uint8_t direction, uint8_t duty);

int main()
{
  Init();
  uint8_t zmienna = 0;
  while (1)
  {
    GPIOD->ODR ^= GPIO_ODR_ODR_12;
    if (zmienna)
    {
      MOTORS (1,30);
      zmienna = 0;
      
    }
    else 
    {
      MOTORS (0,80);
      zmienna = 1; 
    }
    
    delay_ms(1000);
  }
  return 0;
}


void Init()
{
  /******  Clocks **********************************************************************************/
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_SPI1EN;
  /******  LEDS**********************************************************************************/
  GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
  /******  Motor Direction **********************************************************************************/
  GPIOE->MODER |= GPIO_MODER_MODER9_0 | GPIO_MODER_MODER13_0;//PE9, PE13
  GPIOD->MODER |= GPIO_MODER_MODER0_0;//PD0
  GPIOA->MODER |= GPIO_MODER_MODER10_0;//PA10
  /******  Systick - every 1ms **********************************************************************************/
  SysTick_Config(16000);
  /******  TIM1 - Motor PWM **********************************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODER8_1;//PA8
  GPIOA->AFR[1] = 0x1;
  GPIOE->MODER |= GPIO_MODER_MODER11_1;//PE11
  GPIOE->AFR[1] = 0x00001000;
  TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
  TIM1->CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
  TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
  TIM1->BDTR |= TIM_BDTR_MOE;
  TIM1->ARR = 99;
  TIM1->PSC = 15;//10kHz
  TIM1->EGR |= TIM_EGR_UG;
  TIM1->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
  /******  SPI1 - L3GD20 (Gyro) **********************************************************************************/
  GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 |GPIO_MODER_MODER7_1;//PA5(SCK), PA6(MISO), PA7(MOSI)
  GPIOA->AFR[0] |= 0x5<<5 | 0x5<<6 | 0x5<<7;
  GPIOE->MODER |= GPIO_MODER_MODER3_0;//PE3 (CS)
  GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0;//Pull-Up
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_SPE;
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
    GPIOD->BSRRH=0x0001;
    GPIOE->BSRRH=0x2000;
    GPIOA->BSRRL=0x0400;
    GPIOE->BSRRL=0x0200;
  }
  else
  {
    GPIOA->BSRRH=0x0400;
    GPIOE->BSRRH=0x0200;
    GPIOD->BSRRL=0x0001;
    GPIOE->BSRRL=0x2000;
  }
}