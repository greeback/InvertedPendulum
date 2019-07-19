#include "stm32f401xc.h"
#include "L3GD20.h"
#include "main.h"

uint16_t timeout;

void SPI_slaveSelect_ctrl(uint8_t state){
  if(state)
  {
    GPIOE->BSRR = GPIO_BSRR_BR3;
  }
  else 
  {
    while(SPI1->SR & SPI_SR_BSY);
    GPIOE->BSRR = GPIO_BSRR_BS3;
  }
}

uint8_t Spi_sendrecv (uint8_t data)
{
  timeout = TIMEOUT_TIME;
  while( !(SPI1->SR & SPI_SR_TXE) &(timeout != 0))
  {
    timeout--;
  }	
  SPI1->DR = data;
  timeout = TIMEOUT_TIME;
  while( !(SPI1->SR & SPI_SR_RXNE)&(timeout != 0) )
  {
    timeout--;
  }	
  data = SPI1->DR;
  return data;
}

void L3GD20_write_reg(uint8_t addr, uint8_t value)
{
  SPI_slaveSelect_ctrl(select);
  Spi_sendrecv(addr);
  Spi_sendrecv(value);
  SPI_slaveSelect_ctrl(deselect);
}

uint8_t L3GD20_read_reg(uint8_t addr)
{
  SPI_slaveSelect_ctrl(select);
  /* Send address with read command */
  Spi_sendrecv(addr | 0x80);
  uint8_t value = Spi_sendrecv(0xff);
  SPI_slaveSelect_ctrl(deselect);
  return value;
}

void L3GD20_init(void)
{
  /* Set high-pass filter settings */
  L3GD20_write_reg(L3GD20_CTRL_REG2, 0x09);
  
  /* Enable high-pass filter */
  L3GD20_write_reg(L3GD20_CTRL_REG5, 0x10);
  
  /* Full scale selection */
  L3GD20_write_reg(L3GD20_CTRL_REG4, L3GD20_CTRL_REG4_FS_2000);
  
  /* Power on, turn on all axes, maximym ODR - output data rate */
  while(L3GD20_read_reg(L3GD20_CTRL_REG1) != 0xFF)
    L3GD20_write_reg(L3GD20_CTRL_REG1, 0xFF); 
}

void L3GD20_read_rates (L3GD20_Data_t* Data)
{
  int16_t RawData;
  float s;
  
  s = L3GD20_SENSITIVITY_2000 * 0.001;
  
#ifdef X_AXIS_ENABLE
  /* Read X axis and check for drift*/ 
  RawData = L3GD20_read_reg(L3GD20_OUT_X_L);
  RawData |= L3GD20_read_reg(L3GD20_OUT_X_H) << 8;
  //Data->X = (float)RawData * s;
  Data->X = (float)RawData * s - GYRO_OFFSET_2000;
  //if(Data->X>-1.9 && Data->X<1.4) Data->X=0;
#endif
  
#ifdef Y_AXIS_ENABLE
  /* Read Y axis */ 
  RawData = L3GD20_read_reg(L3GD20_OUT_Y_L);
  RawData |= L3GD20_read_reg(L3GD20_OUT_Y_H) << 8;
  Data->Y = (float)RawData * s;
#endif
  
#ifdef Z_AXIS_ENABLE
  /* Read Z axis */ 
  RawData = L3GD20_read_reg(L3GD20_OUT_Z_L);
  RawData |= L3GD20_read_reg(L3GD20_OUT_Z_H) << 8;
  Data->Z = (float)RawData * s;
#endif
}

void L3GD20_read_reg_multi(uint8_t addr, void* data, int size)
{
  int i;
  uint8_t* buffer = (uint8_t*)data;
  
  SPI_slaveSelect_ctrl(select);
  Spi_sendrecv(addr);
  
  for (i = 0; i < size - 1; i++) 
    buffer[i] = Spi_sendrecv(0xff);
  SPI_slaveSelect_ctrl(deselect);
}