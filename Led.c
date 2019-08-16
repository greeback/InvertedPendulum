#include "Led.h"

GPIO_Handle_t led_green, led_orange, led_red, led_blue;

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

void LED_Init ()
{
	GPIO_Config_t led_config;
	
	led_config.Mode 	= GPIO_Mode_OUT;
	led_config.OType 	= GPIO_OType_PP;
	led_config.Speed 	= GPIO_Speed_Low;
	led_config.PuPd 	= GPIO_PuPd_NOPULL;
	
	led_green.Base = led_orange.Base = led_red.Base = led_blue.Base = GPIOD;
	
	led_green.Pin 	= 12;
	led_orange.Pin 	= 13;
	led_red.Pin 	= 14;
	led_blue.Pin 	= 15;
	
	GPIO_Init(&led_green, &led_config);
	GPIO_Init(&led_orange, &led_config);
	GPIO_Init(&led_red, &led_config);
	GPIO_Init(&led_blue, &led_config);
}