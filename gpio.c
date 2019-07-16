#include "gpio.h"

void GPIO_PeriClockControl(GPIO_TypeDef* pGPIOx, uint8_t EnorDi)
{
  if(EnorDi)
  {
    if (pGPIOx = GPIOA_BASE) 
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (pGPIOx = GPIOB_BASE)
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (pGPIOx = GPIOC_BASE)
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (pGPIOx = GPIOD_BASE)
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (pGPIOx = GPIOE_BASE)
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    else if (pGPIOx = GPIOH_BASE)
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
  }
  else return;
}