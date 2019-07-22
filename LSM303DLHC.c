#include "stm32f401xc.h"
#include "LSM303DLHC.h"

void I2C_Start ()
{
  I2C1->CR1 |= I2C_CR1_START; //send start condition
  while (!(I2C1->SR1 & I2C_SR1_SB)); //wait untill start condition is generated
}

void LSM303DLHC_read_reg()
{
  I2C_Start();
  I2C1->DR = LSM303D_ADDR;
  while (!(I2C1->SR1 & I2C_SR1_ADDR));
  uint32_t dummyRead = I2C1->SR2;
  dummyRead = I2C1->SR2;
  (void)dummyRead;
  
}