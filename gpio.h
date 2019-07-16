#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f401xc.h"

typedef struct
{
  uint8_t       GPIO_PinNumber;
  uint8_t       GPIO_PinMode;
  uint8_t       GPIO_PinSpeed;
  uint8_t       GPIO_PinPuPdControl;
  uint8_t       GPIO_PinOPType;
  uint8_t       GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct
{
  GPIO_TypeDef*         pGPIOx;
  GPIO_PinConfig_t      GPIO_PinConfig;
}GPIO_Handle_t;

void GPIO_PeriClockControl(GPIO_TypeDef* pGPIOx, uint8_t EnorDi);
void gpio_read_pin (void);
#endif /* __GPIO_H */