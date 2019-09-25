#include "stm32f401xc.h"
#include <stddef.h>
#include "L3GD20.h"
#include "main.h"
#include "stm32f4_spi.h"

/* Private functions */
static void SPI_slaveSelect_ctrl(uint8_t state);
static void L3GD20_SPI_Init();

/* Private variables */
static GPIO_Handle_t SS;

uint8_t L3GD20_read_reg(uint8_t addr)
{
	uint8_t ret;
	addr |= 0x80;
	SPI_slaveSelect_ctrl (select);
	
	SPI_SendData (SPI1, &addr, 1);
	
	SPI_ReceiveData(SPI1, &ret, 1);
	
	SPI_slaveSelect_ctrl (deselect);
	return ret;
}

void L3GD20_write_reg(uint8_t addr, uint8_t value)
{
	SPI_slaveSelect_ctrl(select);
	SPI_SendData (SPI1, &addr, 1);
	SPI_SendData (SPI1, &value, 1);
	SPI_slaveSelect_ctrl(deselect);
}

void L3GD20_read_reg_multi (uint8_t addr, uint8_t* data, uint32_t Len)
{
	/* set Read bit and Multiple reading bit */
	addr |= 0xC0;
	SPI_slaveSelect_ctrl (select);
	
	SPI_SendData (SPI1, &addr, 1);
	
	SPI_ReceiveData(SPI1, data, Len);
	
	SPI_slaveSelect_ctrl (deselect);
}

void L3GD20_Init()
{
	/* Initialize SPI1 */
	L3GD20_SPI_Init();
	
	/* Full scale selection */
	L3GD20_write_reg(L3GD20_CTRL_REG4, L3GD20_CTRL_REG4_FS);
	
	/* Power on, turn on all axes, maximum ODR - output data rate */
	L3GD20_write_reg(L3GD20_CTRL_REG1, 0xFF); 
}

void L3GD20_read_rates (L3GD20_Data_t* Data)
{
	uint8_t tmp[2];
	int16_t RawData;
	float s;
	
	s = L3GD20_SENSITIVITY * 0.001;
	
	/* Read X axis and check for drift*/  
	L3GD20_read_reg_multi(L3GD20_OUT_X_L, tmp, 2);
	RawData = tmp[0];
	RawData |= tmp[1] << 8;
	Data->X = (float)RawData * s - GYRO_OFFSET;
}

static void SPI_slaveSelect_ctrl(uint8_t state)
{
	if(state)
	{
		GPIO_ClearPin(&SS);
	}
	else 
	{
		while(SPI1->SR & SPI_SR_BSY);
		GPIO_SetPin(&SS);
	}
}

static void L3GD20_SPI_Init()
{
	GPIO_Config_t	SPI_Pins_Config;
	GPIO_Handle_t	Miso, Mosi, Sck;
	
	/* Configure PA6(MISO), PA7(MOSI) and PA5(SCK) */
	SPI_Pins_Config.Mode 	= GPIO_Mode_AF;
	SPI_Pins_Config.Speed	= GPIO_Speed_Low;
	SPI_Pins_Config.PuPd	= GPIO_PuPd_NOPULL;
	SPI_Pins_Config.OType	= GPIO_OType_PP;
	SPI_Pins_Config.AltFun 	= 5;
	
	Miso.Base = Mosi.Base = Sck.Base = GPIOA;
	Miso.Pin = 6;
	Mosi.Pin = 7;
	Sck.Pin  = 5;
	
	GPIO_Init(&Miso, &SPI_Pins_Config);
	GPIO_Init(&Mosi, &SPI_Pins_Config);
	GPIO_Init(&Sck, &SPI_Pins_Config);
	
	/* Configure PE3 (SS) - Slave Select */
	SPI_Pins_Config.Mode 	= GPIO_Mode_OUT;
	SPI_Pins_Config.Speed	= GPIO_Speed_Low;
	SPI_Pins_Config.PuPd	= GPIO_PuPd_UP;
	SPI_Pins_Config.OType	= GPIO_OType_PP;
	
	SS.Base = GPIOE;
	SS.Pin  = 3;
	
	GPIO_Init(&SS, &SPI_Pins_Config);
	SPI_slaveSelect_ctrl (deselect);
	
	/* Configure SPI1 */
	SPI_Config_t SPI_Config;
	
	SPI_Config.MODE = SPI_Mode_Master;
	SPI_Config.DFF	= SPI_DFF_8;
	SPI_Config.CPOL	= SPI_CPOL_1;
	SPI_Config.CPHA	= SPI_CPHA_Second;
	SPI_Config.SSM	= SPI_SSM_Enabled;
	SPI_Config.SSI	= SPI_SSI_Master;
	SPI_Config.BR	= SPI_BR_2;			// 4MHz
	
	SPI_Init(SPI1, &SPI_Config);
}

