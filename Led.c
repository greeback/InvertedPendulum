#include "Led.h"

void LED(uint32_t Colour, ActionTypeDef Action)
{
  switch(Action)
  {
  case ON:
    GPIOD->ODR |= Colour;
    break;
  case OFF:
    GPIOD->ODR &= ~Colour;
    break;
  case TOGGLE:
    GPIOD->ODR ^= Colour;
    break;
  }
}