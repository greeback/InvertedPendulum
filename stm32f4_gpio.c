/**	
* |----------------------------------------------------------------------
* | Copyright (c) 2019 Aleksander Dykman
* |
* |----------------------------------------------------------------------
*/

#include "stm32f4_gpio.h"

/* Private functions */
static void GPIO_EnableClock(GPIO_TypeDef* GPIOx);


void GPIO_Init (GPIO_Handle_t* Handle, GPIO_Config_t* Config)
{
	/* Enable clock for GPIO */
	GPIO_EnableClock(Handle->Base);
	
	/* Configure mode */
	Handle->Base->MODER |= (Config->Mode) << (Handle->Pin * 2);
	
	/* Configure output type */
	Handle->Base->OTYPER |= (Config->OType) << (Handle->Pin);
	
	/* Configure pull-up, pull-down resistors */
	Handle->Base->PUPDR |= (Config->PuPd) << (Handle->Pin *2 );
	
	/* Configure output speed */
	Handle->Base->OSPEEDR |= (Config->Speed) << (Handle->Pin * 2);
	
	/* Configure alternate function if it's different from 0 */
	if (Config->AltFun)
	{
		if (Handle->Pin < 8)
		{
			Handle->Base->AFR[0] |= (Config->AltFun) << (Handle->Pin * 4);
		}
		else
		{
			Handle->Base->AFR[1] |= (Config->AltFun) << ((Handle->Pin - 8) * 4);
		}
	}
}

void GPIO_SetPin (GPIO_Handle_t* Handle)
{
	Handle->Base->BSRR = 1 << (Handle->Pin);
}

void GPIO_ClearPin (GPIO_Handle_t* Handle)
{
	Handle->Base->BSRR = 1 << (Handle->Pin + 16);
}

void GPIO_TogglePin (GPIO_Handle_t* Handle)
{
	Handle->Base->ODR ^= 1 << (Handle->Pin);
}
	
uint8_t GPIO_ReadPin (GPIO_Handle_t* Handle)
{
	return (Handle->Base->IDR & (1 << Handle->Pin));
}
static void GPIO_EnableClock(GPIO_TypeDef* GPIOx)
{
	if(GPIOx == GPIOA) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	}else if (GPIOx == GPIOB) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
	}else if (GPIOx == GPIOC) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOCEN; 
	}else if (GPIOx == GPIOD) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIODEN;
	}else if (GPIOx == GPIOE) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOEEN;
	}else if (GPIOx == GPIOH) {
		RCC->AHB1ENR |=RCC_AHB1ENR_GPIOHEN;
	}
}