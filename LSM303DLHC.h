/** 
 * @file    LSM303DLHC.h
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   LSM303DLHC Library for STM32F4xx devices
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef __LSM303DLHC_H__
#define __LSM303DLHC_H__

/**
 * @addtogroup LSM303DLHC_Driver
 * @{
 */
/**
 * @defgroup LSM303DLHC_Typedefs
 * @brief    LSM303DLHC Typedefs used for Accelerometer initialiation, reading and writing
 * @{
 */
	 
/**
 * @}
 */

 /**
 * @defgroup LSM303DLHC Macros
 * @brief    Library defines
 * @{
 */
/* LSM303DLHC registers */
#define LSM303DLHC_CTRL_REG1_A			0x20
#define LSM303DLHC_CTRL_REG2_A			0x21
#define LSM303DLHC_CTRL_REG3_A			0x22
#define LSM303DLHC_CTRL_REG4_A			0x23
#define LSM303DLHC_CTRL_REG5_A			0x24
#define LSM303DLHC_CTRL_REG6_A			0x25
#define LSM303DLHC_REFFERENCE_A			0x26
#define LSM303DLHC_STATUS_REG_A			0x27
#define LSM303DLHC_OUT_X_L_A			0x28
#define LSM303DLHC_OUT_X_H_A			0x29
#define LSM303DLHC_OUT_Y_L_A			0x2A
#define LSM303DLHC_OUT_Y_H_A			0x2B
#define LSM303DLHC_OUT_Z_L_A			0x2C
#define LSM303DLHC_OUT_Z_H_A			0x2D
#define LSM303DLHC_FIFO_CTRL_REG_A		0x2E
#define LSM303DLHC_FIFO_SRC_REG_A		0x2F
#define LSM303DLHC_INT1_CFG_A   		0x30
#define LSM303DLHC_INT1_SOURCE_A   		0x31
#define LSM303DLHC_INT1_THS_A   		0x32
#define LSM303DLHC_INT1_DURATION_A   	0x33
#define LSM303DLHC_INT2_CFG_A   		0x34
#define LSM303DLHC_INT2_SOURCE_A   		0x35
#define LSM303DLHC_INT2_THS_A   		0x36
#define LSM303DLHC_INT2_DURATION_A   	0x37
#define LSM303DLHC_CLICK_CFG_A   		0x38
#define LSM303DLHC_CLICK_SRC_A   		0x39
#define LSM303DLHC_CLICK_THS_A   		0x3A
#define LSM303DLHC_TIME_LIMIT_A   		0x3B
#define LSM303DLHC_TIME_LATENCY_A   	0x3C
#define LSM303DLHC_TIME_WINDOW_A   		0x3D
#define LSM303DLHC_CRA_REG_M       		0x00
#define LSM303DLHC_CRB_REG_M       		0x01
#define LSM303DLHC_MR_REG_M       		0x02
#define LSM303DLHC_OUT_X_L_M			0x03
#define LSM303DLHC_OUT_X_H_M			0x04
#define LSM303DLHC_OUT_Y_L_M			0x05
#define LSM303DLHC_OUT_Y_H_M			0x06
#define LSM303DLHC_OUT_Z_L_M			0x07
#define LSM303DLHC_OUT_Z_H_M			0x08
#define LSM303DLHC_SR_REG_Mg			0x09
#define LSM303DLHC_IRA_REG_Mg			0x0A
#define LSM303DLHC_IRB_REG_Mg			0x0B
#define LSM303DLHC_IRC_REG_Mg			0x0C
#define LSM303DLHC_TEMP_OUT_H_M			0x31
#define LSM303DLHC_TEMP_OUT_L_M			0x32

#define LSM303D_ADDR			        0x32
#define X_A_Enable                      0x01
#define Y_A_Enable                      0x02
#define Z_A_Enable                      0x04
#define Data_rate_100Hz                 0x50
#define HIGH_RESOLUTION_EN              0X08

/**
 * @}
 */	 
	 
/**
 * @defgroup LSM303DLHC_Structures
 * @brief    LSM303DLHC Structures use for handling reading from accelerometer
 * @{
 */

/**
 * @brief  LSM303DLHC main working structure
 */
typedef struct
{
  int16_t X;
  int16_t Y;
  volatile int16_t Z;
}LSM303DLHC_Data_t;

/**
 * @}
 */

/**
 * @defgroup LSM303DLHC_Functions
 * @brief    LSM303DLHC Functions used for gyro handling
 * @{
 */

/**
 * @brief  Writes one byte to given register
 * @param  reg:		SubAddress to which data is written
 * @param  data:	It will be written to SubAddress
 */
void LSM303DLHC_write_reg (uint8_t reg, uint8_t data);

/**
 * @brief  Reads one byte from given register
 * @note   No need to set 8th bit, function does that
 * @param  reg:			SubAddress from which data is read
 * @param  *RxBuffer:	pointer to table where data will be stored
 */
void LSM303DLHC_read_reg (uint8_t reg, uint8_t *RxBuffer);

/**
 * @brief  Reads many bytes from given register
 * @note   No need to set 8th bit, function does that, it also autoincrement
 *			subaddress, so you can read subsequent registers
 * @param  reg:			SubAddress from which data is read
 * @param  *RxBuffer:	pointer to table where data will be stored
 * @param  Len:			How many registers are to be read
 */
void LSM303DLHC_read_multi_reg (uint8_t reg, uint8_t *RxBuffer, uint32_t Len);

/**
 * @brief  Reads acceleration on Y and Z axis
 * @param  *Data: Pointer to structure where output accelerations will be written
 */
void LSM303DLHC_read_rates (LSM303DLHC_Data_t* Data);

/**
 * @brief  Initializes LSM303DLHC
 * @note   This function initialize I2C1 with apropriate GPIO pins,
 * 		   sets accelerometer to read all axes with max Output Data Rate
 */
void LSM303DLHC_Init (void);

/**
 * @}
 */
/**
 * @}
 */
#endif //__LSM303DLHC_H__