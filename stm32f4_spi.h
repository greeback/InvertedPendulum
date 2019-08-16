/** 
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   SPI Library for STM32F4xx devices
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef STM32F4_SPI_H
#define STM32F4_SPI_H

#include "stm32f401xc.h"
#include "stm32f4_gpio.h"


/**
 * @defgroup SPI_Typedefs
 * @brief    SPI Typedefs used for SPI library for initialization purposes
 */

/**
 * @brief SPI Master/Slave enumeration
 */
typedef enum 
{
	SPI_Mode_Slave 	= 0x0,	/*!< SPI acting as slave */
	SPI_Mode_Master = 0x1 	/*!< SPI acting as master */
} SPI_Mode_t;	

/**
 * @brief  SPI Data Frame Format
 */
typedef enum 
{
	SPI_DFF_8 	= 0x0, /*!< SPI 8 bit DFF */
	SPI_DFF_16 	= 0x1  /*!< SPI 16 bit DFF */
} SPI_DFF_t;

/**
 * @brief  SPI Clock Polarity
 */
typedef enum 
{
	SPI_CPOL_0 	= 0x0,	/*!< SPI Clock to 0 when Idle */
	SPI_CPOL_1	= 0x1	/*!< SPI Clock to 1 when Idle */
} SPI_CPOL_t;

/**
 * @brief SPI Clock Phase
 */
typedef enum 
{
	SPI_CPHA_First 	= 0x0, 	/*!< SPI the first clock transition is the first data capture edge */
	SPI_CPHA_Second = 0x1	/*!< SPI the second clock transition is the first data capture edge */
} SPI_CPHA_t;

/**
 * @brief SPI Software Slave Management
 */
typedef enum 
{
	SPI_SSM_Disabled	= 0x0, 	/*!< SPI Software Slave Management Enabled */
	SPI_SSM_Enabled		= 0x1	/*!< SPI Software Slave Management Disabled */
} SPI_SSM_t;

/**
 * @brief SPI Internal Slave Select
 */
typedef enum 
{
	SPI_SSI_Slave	= 0x0, 	/*!< SPI value 0 is forced to NSS pin */
	SPI_SSI_Master	= 0x1	/*!< SPI value 1 is forced to NSS pin (if (Mode == Master && SSM==1) SSI = 1) */
} SPI_SSI_t;

/**
 * @brief SPI Baud Rate Select
 */
typedef enum 
{
	SPI_BR_2	= 0x000, 	/*!< SPI BR = ClockFreq / 2 */
	SPI_BR_4	= 0x001, 	/*!< SPI BR = ClockFreq / 4 */
	SPI_BR_8	= 0x010, 	/*!< SPI BR = ClockFreq / 8 */
	SPI_BR_16	= 0x011, 	/*!< SPI BR = ClockFreq / 16 */
	SPI_BR_32	= 0x100, 	/*!< SPI BR = ClockFreq / 32 */
	SPI_BR_64	= 0x101, 	/*!< SPI BR = ClockFreq / 64 */
	SPI_BR_128	= 0x110, 	/*!< SPI BR = ClockFreq / 128 */
	SPI_BR_256	= 0x111, 	/*!< SPI BR = ClockFreq / 256 */
} SPI_BR_t;

/**
 * @brief SPI application states
 */
typedef enum 
{
	SPI_READY	= 0x0, 	/*!< SPI is ready for action */
	SPI_BUSY_RX	= 0x1,	/*!< Ongoing receiving */
	SPI_BUSY_TX = 0x2	/*!< Ongoing transmitting */	
} SPI_APP_STATE_t;
 /**
 * @defgroup SPI_Macros
 * @brief    Library defines
 */

/**
 * @defgroup SPI_Structures
 * @brief    SPI Structures used for SPI library initialization purposes
 */

/**
 * @brief Configuration structure for a SPI configuration
 */
typedef struct
{
	SPI_Mode_t		MODE;
	SPI_DFF_t 		DFF;
	SPI_CPOL_t 		CPOL;
	SPI_CPHA_t		CPHA;
	SPI_SSM_t 		SSM;
	SPI_SSI_t		SSI;
	SPI_BR_t		BR;
}SPI_Config_t;

/**
 * @defgroup SPI_Functions
 * @brief    SPI Functions used for SPI handling
 */

/**
 * @brief  Initializes SPI pins(s)
 * @note   This function also enables clock for SPI port
 * @param  Handle: Pointer to SPI handle
 * @param  Config: Pointer to SPI Configuration data
 * @retval None
 */
void SPI_Init (SPI_TypeDef* SPIx, SPI_Config_t* Config);

/**
 * @brief  Sends multiple bytes over SPI
 * @note   This function checks for data frame format
 * @param  SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 3
 * @param  *TxBuffer: Pointer to array with data to send over SPI
 * @param  Len: Number of elements to send over SPI
 * @retval None
 */
void SPI_SendData (SPI_TypeDef* SPIx, uint8_t *TxBuffer, uint32_t Len);

/**
 * @brief  Receives multiple bytes over SPI
 * @note   This function checks for data frame format
 * @param  SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 3
 * @param  *RxBuffer: Pointer to 8-bit array to save data into
 * @param  Len: Number of bytes you want read from device
 * @retval None
 */
void SPI_ReceiveData (SPI_TypeDef* SPIx, uint8_t *RxBuffer, uint32_t Len);



#endif /* STM32F4_SPI_H */