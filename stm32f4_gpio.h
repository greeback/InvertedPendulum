/** 
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   GPIO Library for STM32F4xx devices
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef STM32F4_GPIO_H
#define STM32F4_GPIO_H

#include "stm32f401xc.h"


/**
 * @defgroup GPIO_Typedefs
 * @brief    GPIO Typedefs used for GPIO library for initialization purposes
 */

/**
 * @brief GPIO Output type enumeration
 */
typedef enum 
{
	GPIO_Mode_IN	= 0x00,	/*!< GPIO Pin as General Purpose Input */
	GPIO_Mode_OUT	= 0x01,	/*!< GPIO Pin as General Purpose Output */
	GPIO_Mode_AF	= 0x02,	/*!< GPIO Pin as Alternate Function */
	GPIO_Mode_AN	= 0x03,	/*!< GPIO Pin as Analog input/output */
} GPIO_Mode_t;	

/**
 * @brief  GPIO Speed enumeration
 */
typedef enum 
{
	GPIO_OType_PP	= 0x00, /*!< GPIO Output Type Push-Pull */
	GPIO_OType_OD	= 0x01  /*!< GPIO Output Type Open-Drain */
} GPIO_OType_t;

/**
 * @brief  GPIO Speed enumeration
 */
typedef enum 
{
	GPIO_Speed_Low 		= 0x00,	/*!< GPIO Speed Low */
	GPIO_Speed_Medium	= 0x01, /*!< GPIO Speed Medium */
	GPIO_Speed_Fast 	= 0x02,	/*!< GPIO Speed Fast */
	GPIO_Speed_High 	= 0x03	/*!< GPIO Speed High */
} GPIO_Speed_t;

/**
 * @brief GPIO pull resistors enumeration
 */
typedef enum 
{
	GPIO_PuPd_NOPULL 	= 0x00,	/*!< No pull resistor */
	GPIO_PuPd_UP 		= 0x01,	/*!< Pull up resistor enabled */
	GPIO_PuPd_DOWN 		= 0x02	/*!< Pull down resistor enabled */
} GPIO_PuPd_t;



/**
 * @defgroup GPIO_Structures
 * @brief    GPIO Structures used for GPIO library initialization purposes
 */

/**
 * @brief Configuration structure for a GPIO pin
 */
typedef struct
{
	GPIO_Mode_t		Mode;			
	GPIO_Speed_t 	Speed;			
	GPIO_PuPd_t 	PuPd;
	GPIO_OType_t 	OType;
	uint8_t 		AltFun;
}GPIO_Config_t;

/**
 * @brief Handle structure for a GPIO pin - it is used for initialization/reading/writting
 */
typedef struct
{
	GPIO_TypeDef*	Base;			/*!< This holds the base address of the GPIO port to which the pin belongs >*/
	uint8_t 		Pin;			/*!< This holds GPIO pin configuration settings >*/
}GPIO_Handle_t;

/**
 * @defgroup GPIO_Functions
 * @brief    GPIO Functions used for GPIO handling
 */

/**
 * @brief  Initializes GPIO pins(s)
 * @note   This function also enables clock for GPIO port
 * @param  Handle: Pointer to GPIO handle
 * @param  Config: Pointer to GPIO Configuration data
 * @retval None
 */
void GPIO_Init (GPIO_Handle_t* Handle, GPIO_Config_t* Config);

/**
 * @brief  Sets output state as 1
 * @note   It's atomic operation
 * @param  Handle: Pointer to GPIO handle
 * @retval None
 */
void GPIO_SetPin (GPIO_Handle_t* Handle);

/**
 * @brief  Clears output state to 0
 * @note   It's atomic operation
 * @param  Handle: Pointer to GPIO handle
 * @retval None
 */
void GPIO_ClearPin (GPIO_Handle_t* Handle);

/**
 * @brief  Toggle output state between 1 and 0
 * @note   
 * @param  Handle: Pointer to GPIO handle
 * @retval None
 */
void GPIO_TogglePin (GPIO_Handle_t* Handle);

/**
 * @brief  Reads Pin input 
 * @note   Pin must be in input mode.
 * @param  Handle: Pointer to GPIO handle
 * @retval 1 - High state, 0 - low state
 */
uint8_t GPIO_ReadPin (GPIO_Handle_t* Handle);
#endif /* STM32F4_GPIO_H */