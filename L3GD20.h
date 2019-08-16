/** 
 * @author  Aleksander Dykman
 * @email   aleksanderdykman@gmail.com
 * @version v1.0
 * @ide     IAR Embedded Wokbench for ARM
 * @brief   L3GD20 Library for STM32F4xx devices
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2019 Aleksander Dykman
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef __L3GD20_H__
#define __L3GD20_H__

/**
 * @defgroup L3GD20_Typedefs
 * @brief    L3GD20 Typedefs used for Gyro initialiation, reading and writing
 */

/**
 * @brief  SPI Slave Select ON/OFF
 */
enum {deselect=0, select = 1};

 /**
 * @defgroup L3GD20_Macros
 * @brief    Library defines
 */

/* Register addresses */
#define L3GD20_WHO_AM_I      0x0F
#define L3GD20_CTRL_REG1     0x20
#define L3GD20_CTRL_REG2     0x21
#define L3GD20_CTRL_REG3     0x22
#define L3GD20_CTRL_REG4     0x23
#define L3GD20_CTRL_REG5     0x24
#define L3GD20_REFERENCE     0x25
#define L3GD20_OUT_TEMP      0x26
#define L3GD20_STATUS_REG    0x27
#define L3GD20_OUT_X_L       0x28
#define L3GD20_OUT_X_H       0x29
#define L3GD20_OUT_Y_L       0x2A
#define L3GD20_OUT_Y_H       0x2B
#define L3GD20_OUT_Z_L       0x2C
#define L3GD20_OUT_Z_H       0x2D
#define L3GD20_FIFO_CTRL_REG 0x2E
#define L3GD20_FIFO_SRC_REG  0x2F
#define L3GD20_INT1_CFG      0x30
#define L3GD20_INT1_SRC      0x31
#define L3GD20_INT1_THS_XH   0x32
#define L3GD20_INT1_THS_XL   0x33
#define L3GD20_INT1_THS_YH   0x34
#define L3GD20_INT1_THS_YL   0x35
#define L3GD20_INT1_THS_ZH   0x36
#define L3GD20_INT1_THS_ZL   0x37 
#define L3GD20_INT1_DURATION 0x38

/* CTRL_REG1 options */ 
#define X_G_Enable              0x01
#define Y_G_Enable              0x02
#define Z_G_Enable              0x04
#define PD_Normal               0x08
#define Data_G_rate_190Hz       0x50
#define Data_G_rate_95Hz        0x30

/* CTRL_REG5 options */
#define L3GD20_CTRL_REG5_HPEN   0x10

/* Sensitivity factors, datasheet pg. 9 */
#define L3GD20_SENSITIVITY_250		8.75	/*!< 8.75 mdps/digit */
#define L3GD20_SENSITIVITY_500		17.5	/*!< 17.5 mdps/digit */
#define L3GD20_SENSITIVITY_2000		70      /*!< 70 mdps/digit */

/* Full scale selection */
#define L3GD20_CTRL_REG4_FS_250         0x00
#define L3GD20_CTRL_REG4_FS_500         0x10
#define L3GD20_CTRL_REG4_FS_2000        0x20

/* Offset values for each scale. It is tailored to particular gyro */
#define GYRO_OFFSET_250     (0.3)
#define GYRO_OFFSET_500     (-0.16)
#define GYRO_OFFSET_2000    (-0.56)

/* Choose which scale you need - only one */
//#define RANGE_250
//#define RANGE_500
#define RANGE_2000

#ifdef	RANGE_250
#define L3GD20_CTRL_REG4_FS     L3GD20_CTRL_REG4_FS_250
#define L3GD20_SENSITIVITY      L3GD20_SENSITIVITY_250
#define GYRO_OFFSET             GYRO_OFFSET_250
#endif

#ifdef	RANGE_500
#define L3GD20_CTRL_REG4_FS     L3GD20_CTRL_REG4_FS_500
#define L3GD20_SENSITIVITY      L3GD20_SENSITIVITY_500
#define GYRO_OFFSET             GYRO_OFFSET_500
#endif

#ifdef	RANGE_2000
#define L3GD20_CTRL_REG4_FS     L3GD20_CTRL_REG4_FS_2000
#define L3GD20_SENSITIVITY      L3GD20_SENSITIVITY_2000
#define GYRO_OFFSET             GYRO_OFFSET_2000
#endif

/* Cut-Off settings, datasheet pg. 32 */
#define L3GD20_CUTOFF_100		0xF0	

/*Choose which axis you need*/
#define X_AXIS_ENABLE
//#define Y_AXIS_ENABLE
//#define Z_AXIS_ENABLE

/**
 * @defgroup L3GD20_Structures
 * @brief    L3GD20 Structures use for handling reading from gyro
 */

/**
 * @brief  L3GD20 main working structure
 */
typedef struct
{
  volatile float X;
  volatile float Y;
  volatile float Z;
}L3GD20_Data_t;

/**
 * @defgroup L3GD20_Functions
 * @brief    L3GD20 Functions used for gyro handling
 */

/**
 * @brief  Reads one byte from given register
 * @note   No need to set 8th bit, function does that
 * @param  addr: gyro addres from which data is read
 * @retval None
 */
uint8_t L3GD20_read_reg(uint8_t addr);

/**
 * @brief  Writes one byte to given register
 * @param  addr: gyro addres to which data is written
 * @param  value: it will be written to given address
 * @retval None
 */
void L3GD20_write_reg(uint8_t addr, uint8_t value);

/**
 * @brief  Reads multiple bytes to given register
 * @param  addr: gyro addres from which data will be read
 * @param  data: pointer to the table to which data will be written
 * @retval None
 */
void L3GD20_read_reg_multi (uint8_t addr, uint8_t* data, uint32_t Len);

/**
 * @brief  Initializes L3GD20
 * @note   This function initialize SPI1 with apropriate GPIO pins,
 * 		   sets gyro to read all axes with max Output Data Rate
 * @retval None
 */
void L3GD20_Init(void);

/**
 * @brief  Reads Data from X axis and then condition data to degrees/s
 * @param  Data: pointer to main working structure
 * @retval None
 */
void L3GD20_read_rates (L3GD20_Data_t* Data);

#endif //__L3GD20_H__
