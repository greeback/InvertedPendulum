#include "stm32f401xc.h"
#include "LSM303DLHC.h"
#include "Led.h"
#include "stm32f4_i2c.h"
#include "stm32f4_gpio.h"

//static void I2C_Start ()
//{
//  I2C1->CR1 |= I2C_CR1_START; //send start condition
//  while (!(I2C1->SR1 & I2C_SR1_SB)); //wait untill start condition is generated
//}
//
//static void I2C_Stop ()
//{
//  I2C1->CR1 |= I2C_CR1_STOP;
//}
//
//static void I2C_Addr(uint8_t adr, read_write_typedef rw)
//{
//  if (rw == READ)
//    I2C1->DR = (LSM303D_ADDR | 0x01);
//  else
//    I2C1->DR = LSM303D_ADDR;
//  I2C_Clear_Addr_Flag();
//}
//
//static void I2C_Clear_Addr_Flag()
//{
//  while (!(I2C1->SR1 & I2C_SR1_ADDR));
//  /*to clear I2C_SR1_ADDR flag we need to read SR1 and SR2 */
//  uint32_t dummyRead = I2C1->SR1;
//  dummyRead = I2C1->SR2;
//  (void)dummyRead; // avoiding compiler error
//}
//
//static void I2C_Write (uint8_t data)
//{
//  while (!(I2C1->SR1 & I2C_SR1_TXE)); //Wait until Data Register is empty
//  I2C1->DR = data;
//  while (!(I2C1->SR1 & I2C_SR1_TXE)); //Wait until Data Register is empty
//  while (!(I2C1->SR1 & I2C_SR1_BTF)); //Wait until Byte Transfer is Finished
//}
//
//static uint8_t I2C_Read (ack_typedef ack_nack)
//{
//  if (ack_nack == ACK)
//    I2C1->CR1 |= I2C_CR1_ACK;//Enable acking
//  else 
//    I2C1->CR1 &= ~I2C_CR1_ACK;//Disable acking
//  while (!(I2C1->SR1 & I2C_SR1_RXNE)); //Wait until Data Register is not empty 
//  return (I2C1->DR);
//}
//
//static void LSM303DLHC_set_reg(uint8_t reg)
//{
//  I2C_Start();
//  I2C_Addr(LSM303D_ADDR, WRITE);
//  I2C_Write (reg);
//}
//
//void LSM303DLHC_write_reg (uint8_t reg, uint8_t data)
//{
//  LSM303DLHC_set_reg (reg);
//  I2C_Write(data);
//  I2C_Stop();
//}
//
//void LSM303DLHC_read_reg (uint8_t reg, uint8_t* data, uint8_t size)
//{
//  uint8_t i;
//  uint8_t* buffer = (uint8_t*)data;
//  
//  LSM303DLHC_set_reg(reg);
//  I2C_Start(); // repeated start
//  I2C_Addr(LSM303D_ADDR, READ);
//  
//  for (i = 0; i < size - 1; i++)
//  {
//    buffer[i] = I2C_Read(ACK);
//  }
//  I2C_Stop();
//  buffer[i] = I2C_Read(NACK);
//}
//
//void LSM303DLHC_read_rates (LSM303DLHC_Data_t* Data)
//{
//  
////  uint8_t temp[] = {0,0};
////  LSM303DLHC_read_reg (LSM303DLHC_OUT_Y_L_A, temp, 2);
////  Data->Y = ((temp[1]<<8) | temp[0]);
////  Data->Z = temp[3]<<8 | temp[2];
//  
//  uint8_t tempL, tempH;
//  LSM303DLHC_read_reg (LSM303DLHC_OUT_Y_L_A, &tempL, 1);
//  LSM303DLHC_read_reg (LSM303DLHC_OUT_Y_H_A, &tempH, 1);
//  Data->Y = ((tempH<<8) | tempL);
//  LSM303DLHC_read_reg (LSM303DLHC_OUT_Z_L_A, &tempL, 1);
//  LSM303DLHC_read_reg (LSM303DLHC_OUT_Z_H_A, &tempH, 1);
//  Data->Z = ((tempH<<8) | tempL);
//}
//
//void LSM303DLHC_init ()
//{
//  LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG1_A, Y_A_Enable | Z_A_Enable | Data_rate_100Hz);
//  //LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG2_A, LSM303DLHC_CTRL_REG2_A_);
//  //LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG4_A, HIGH_RESOLUTION_EN);
//}

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

void LSM303DLHC_read_reg (uint8_t reg, uint8_t *RxBuffer, uint32_t Len)
{
  I2C_MasterSendData(I2C1, &reg, 1, (LSM303D_ADDR | 0x80), I2C_RS_YES);
  I2C_MasterReceiveData(I2C1, RxBuffer, Len, LSM303D_ADDR, I2C_RS_NO);
}
static void LSM303DLHC_I2C_Init()
{
	GPIO_Config_t	I2C_Pins_Config;
	GPIO_Handle_t	Sda ,Sck;
	
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