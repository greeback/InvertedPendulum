/** 
* @author  Aleksander Dykman
* @email   aleksanderdykman@gmail.com
* @version v1.0
* @ide     IAR Embedded Wokbench for ARM
* @brief   I2C Library for STM32F4xx devices
*
\verbatim
----------------------------------------------------------------------
Copyright (c) 2019 Aleksander Dykman
----------------------------------------------------------------------
\endverbatim
*/
#ifndef STM32F4_I2C_H
#define STM32F4_I2C_H

#include "stm32f401xc.h"
#include "stm32f4_gpio.h"


/**
* @defgroup I2C_Typedefs
* @brief    I2C Typedefs used for I2C library for initialization purposes
*/

/**
* @brief I2C Acknowledge enable / disable
*/
typedef enum 
{
	I2C_Ack_Disable = 0x0,	/*!< Acking disabled */
	I2C_Ack_Enable	= 0x1 	/*!< Acking enabled */
} I2C_AckControl_t;	

/**
* @brief I2C Fast mode duty cycle
*/
typedef enum 
{
	I2C_FmDuty_2	= 0x0,	/*!< t_low / t_high =  2 */
	I2C_FmDuty_16_9	= 0x1 	/*!< t_low / t_high =  16/9 */
} I2C_FmDuty_t;	

/**
* @brief I2C Read or Write 8th bit of address byte
*/
typedef enum 
{
	I2C_Read	= 0x0,	/*!< t_low / t_high =  2 */
	I2C_Write	= 0x1 	/*!< t_low / t_high =  16/9 */
} I2C_ReadWrite_t;	

/**
* @brief I2C Repeated start
*/
typedef enum 
{
	I2C_RS_YES	= 0x0,	/*!< Stop won't be generated after sending/receiving */
	I2C_RS_NO	= 0x1 	/*!< Stop will be generated after sending/receiving */
} I2C_RS_t;	

/**
* @defgroup I2C_Macros
* @brief    Library defines
*/

/**
* @brief I2C Serial Clock Speed
*/
#define I2C_SCL_SPEED_SM	100	/*!< 100 kHz */
#define I2C_SCL_SPEED_FM2k	200	/*!< 200 kHz */
#define I2C_SCL_SPEED_FM4k	400	/*!< 400 kHz */

/**
* @defgroup I2C_Structures
* @brief    I2C Structures used for I2C library initialization purposes
*/

/**
* @brief Configuration structure for a I2C configuration
*/
typedef struct
{
	I2C_AckControl_t	AckControl;
	I2C_FmDuty_t		FmDutyCycle;
	uint8_t				SCLSpeed;		/*!< Frequency of I2C clock in KHz */
	uint8_t				APB1Clock;		/*!< Frequency of APB1 bus in MHz */
	uint8_t				DeviceAdrress:7;/*!< Address of device - only in slave mode */
}I2C_Config_t;

/**
* @defgroup I2C_Functions
* @brief    I2C Functions used for I2C handling
*/

/**
* @brief  Initializes I2C pins(s)
* @note   This function also enables clock for I2C port
* @param  Handle: Pointer to I2C handle
* @param  Config: Pointer to I2C Configuration data
* @retval None
*/
void I2C_Init (I2C_TypeDef* I2Cx, I2C_Config_t* Config);

/**
* @brief  Sends multiple bytes over I2C
* @note   This function checks for data frame format
* @param  I2Cx: Pointer to I2Cx peripheral you will use, where x is between 1 to 3
* @param  *TxBuffer: Pointer to array with data to send over I2C
* @param  Len: Number of elements to send over I2C
* @retval None
*/
void I2C_MasterSendData (I2C_TypeDef* I2Cx, uint8_t *TxBuffer, uint32_t Len, uint8_t Addr, I2C_RS_t rs);

/**
* @brief  Receives multiple bytes over I2C
* @note   This function checks for data frame format
* @param  I2Cx: Pointer to I2Cx peripheral you will use, where x is between 1 to 3
* @param  *RxBuffer: Pointer to 8-bit array to save data into
* @param  Len: Number of bytes you want read from device
* @retval None
*/
void I2C_MasterReceiveData (I2C_TypeDef* I2Cx, uint8_t *TxBuffer, uint32_t Len, uint8_t Addr, I2C_RS_t rs);



#endif /* STM32F4_I2C_H */