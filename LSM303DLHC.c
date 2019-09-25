#include "stm32f401xc.h"
#include "LSM303DLHC.h"
#include "Led.h"
#include "stm32f4_i2c.h"
#include "stm32f4_gpio.h"

/* Private functions */
static void LSM303DLHC_I2C_Init(void);

void LSM303DLHC_Init ()
{
	LSM303DLHC_I2C_Init();
	LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG1_A, Y_A_Enable | Z_A_Enable | Data_rate_100Hz); 
}

void LSM303DLHC_write_reg (uint8_t reg, uint8_t data)
{
	uint8_t TxBuffer[2];
	TxBuffer[0] = reg;
	TxBuffer[1] = data;
	I2C_MasterSendData(I2C1, TxBuffer, 2, LSM303D_ADDR, I2C_RS_NO);
}

void LSM303DLHC_read_reg (uint8_t reg, uint8_t *RxBuffer)
{
	I2C_MasterSendData(I2C1, &reg, 1, LSM303D_ADDR, I2C_RS_YES);
	I2C_MasterReceiveData(I2C1, RxBuffer, 1, LSM303D_ADDR, I2C_RS_NO);
}

void LSM303DLHC_read_multi_reg (uint8_t reg, uint8_t *RxBuffer, uint32_t Len)
{
	/* MSB of sub address need to be 1 for autoincrement*/
	reg |= 0x80;
	I2C_MasterSendData(I2C1, &reg, 1, LSM303D_ADDR, I2C_RS_YES);
	I2C_MasterReceiveData(I2C1, RxBuffer, Len, LSM303D_ADDR, I2C_RS_NO);
}

void LSM303DLHC_read_rates (LSM303DLHC_Data_t* Data)
{
	uint8_t temp[4];
	LSM303DLHC_read_multi_reg (LSM303DLHC_OUT_Y_L_A, temp, 4);
	Data->Y = ((temp[1]<<8) | temp[0]);
	Data->Z = ((temp[3]<<8) | temp[2]);
}

static void LSM303DLHC_I2C_Init()
{
	GPIO_Config_t	I2C_Pins_Config;
	GPIO_Handle_t	Sda, Sck;
	
	/* Configure PB6(SCK), PB9(SDA) */
	I2C_Pins_Config.Mode 	= GPIO_Mode_AF;
	I2C_Pins_Config.OType 	= GPIO_OType_OD;
	I2C_Pins_Config.PuPd 	= GPIO_PuPd_UP;
	I2C_Pins_Config.AltFun	= 4;
	
	Sda.Base = Sck.Base = GPIOB;
	Sck.Pin = 6;
	Sda.Pin = 9;
	
	GPIO_Init(&Sck, &I2C_Pins_Config);
	GPIO_Init(&Sda, &I2C_Pins_Config);
	
	/* Configure I2C1 */
	I2C_Config_t	I2C_Config;
	
	I2C_Config.AckControl	= I2C_Ack_Enable;
	I2C_Config.SCLSpeed		= I2C_SCL_SPEED_SM;
	I2C_Config.APB1Clock	= 8;
	
	I2C_Init(I2C1, &I2C_Config);
}