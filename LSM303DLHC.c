#include "stm32f401xc.h"
#include "LSM303DLHC.h"
#include "Led.h"

static void I2C_Start ()
{
  I2C1->CR1 |= I2C_CR1_START; //send start condition
  while (!(I2C1->SR1 & I2C_SR1_SB)); //wait untill start condition is generated
}

static void I2C_Stop ()
{
  I2C1->CR1 |= I2C_CR1_STOP;
}

static void I2C_Addr(uint8_t adr, read_write_typedef rw)
{
  if (rw == READ)
    I2C1->DR = (LSM303D_ADDR | 0x01);
  else
    I2C1->DR = LSM303D_ADDR;
  I2C_Clear_Addr_Flag();
}

static void I2C_Clear_Addr_Flag()
{
  while (!(I2C1->SR1 & I2C_SR1_ADDR));
  /*to clear I2C_SR1_ADDR flag we need to read SR1 and SR2 */
  uint32_t dummyRead = I2C1->SR1;
  dummyRead = I2C1->SR2;
  (void)dummyRead; // avoiding compiler error
}

static void I2C_Write (uint8_t data)
{
  while (!(I2C1->SR1 & I2C_SR1_TXE)); //Wait until Data Register is empty
  I2C1->DR = data;
  while (!(I2C1->SR1 & I2C_SR1_TXE)); //Wait until Data Register is empty
  while (!(I2C1->SR1 & I2C_SR1_BTF)); //Wait until Byte Transfer is Finished
}

static void LSM303DLHC_set_reg(uint8_t reg)
{
  I2C_Start();
  I2C_Addr(LSM303D_ADDR, WRITE);
  I2C_Write (reg);
}

void LSM303DLHC_write_reg (uint8_t reg, uint8_t data)
{
  LSM303DLHC_set_reg (reg);
  I2C_Write(data);
  I2C_Stop();
}

void LSM303DLHC_read_reg (uint8_t reg, uint8_t* data, uint8_t size)
{
  LSM303DLHC_set_reg(reg);
  I2C_Start(); // repeated start
  I2C_Addr(LSM303D_ADDR, READ);
  
  
  if (size == 1)
  {
    I2C1->CR1 &= ~I2C_CR1_ACK;//Disable acking
    //I2C_Clear_Addr_Flag();
    I2C_Stop();
    
    
    while (!(I2C1->SR1 & I2C_SR1_RXNE)); //Wait until Data Register is not empty 
    *data = I2C1->DR;
  }
  
  if (size >1)
  {
    
    //I2C_Clear_Addr_Flag();
        
    for (uint8_t i=size ; i>0 ; i--)
    {
      while (!(I2C1->SR1 & I2C_SR1_RXNE)); //Wait until Data Register is not empty
      if (i == 2) //if last 2 bytes are remaining
      {
        I2C1->CR1 &= ~I2C_CR1_ACK;//Disable acking
        I2C_Stop();
      }
      *data = I2C1->DR;
      data++;
    }
  }
  I2C1->CR1 |= I2C_CR1_ACK;//Re-enable acking
}

void LSM303DLHC_init ()
{
  LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG1_A, Y_A_Enable | Z_A_Enable | Data_rate_100Hz);
  LSM303DLHC_write_reg(LSM303DLHC_CTRL_REG4_A, HIGH_RESOLUTION_EN);
}

void LSM303DLHC_read_rates (LSM303DLHC_Data_t* Data)
{
  
  uint8_t temp[4];
  LSM303DLHC_read_reg (LSM303DLHC_OUT_Y_L_A, temp, 4);
  Data->Y = ((temp[1]<<8) | temp[0]);
  Data->Z = temp[3]<<8 | temp[2];
}