/**	
* |----------------------------------------------------------------------
* | Copyright (c) 2019 Aleksander Dykman
* |----------------------------------------------------------------------
*/
#include "stm32f4_i2c.h"

/* Private functions */
static void I2C_EnableClock 	(I2C_TypeDef* I2Cx);
static void I2C_Start 			(I2C_TypeDef* I2Cx);
static void I2C_Stop 			(I2C_TypeDef* I2Cx);
static void I2C_Addr 			(I2C_TypeDef* I2Cx, uint8_t adr, I2C_ReadWrite_t rw);
static void I2C_ClearAddr 		(I2C_TypeDef* I2Cx);

void I2C_Init (I2C_TypeDef* I2Cx, I2C_Config_t* Config)
{
	uint16_t tmp = 0;
	I2C_EnableClock(I2Cx);
	
	/* Set Peripheral Clock Frequency */
	I2Cx->CR2 |= Config->APB1Clock;
	
	/* Set Slave Own Address */
	tmp |= Config->DeviceAdrress <<  I2C_OAR1_ADD1_Pos;
	tmp |= (1 << 1);
	I2Cx->OAR1 |= tmp;
	
	
	/* Check in which mode we need I2C */
	if (Config->SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		/* Standard Mode up to 10kHz */
		/* Calculate CCR */
		tmp = (Config->APB1Clock * 1000) / (2 * Config->SCLSpeed);
		/* Calculate and assign Trise */
		I2Cx->TRISE = Config->APB1Clock + 1;
	}
	else
	{
		/* Fast Mode up to 40kHz */
		tmp = I2C_CCR_FS;
		
		/* Check for duty mode and calculate CCR */
		tmp |= I2C_CCR_DUTY;
		if (Config->FmDutyCycle == I2C_FmDuty_2)
		{
			tmp |= (Config->APB1Clock * 1000) / (3 * Config->SCLSpeed);
		}
		else
		{
			tmp |= (Config->APB1Clock * 1000) / (25 * Config->SCLSpeed);
		}
		I2Cx->TRISE = (uint8_t)(Config->APB1Clock * 0.3 + 1);
	}
	/* Set CCR register */
	I2Cx->CCR |= tmp;
	
	/* Enable_Disable Acknowledges */
	tmp = Config->AckControl << I2C_CR1_ACK_Pos;
	
	/* Enable I2C */
	tmp |= I2C_CR1_PE;
	
	/* Set CR1 register */
	I2Cx->CR1 |= tmp;
}

void I2C_MasterSendData (I2C_TypeDef* I2Cx, uint8_t *TxBuffer, uint32_t Len, uint8_t Addr, I2C_RS_t rs)
{
	I2C_Start(I2Cx);
	I2C_Addr(I2Cx, Addr, I2C_Write);
	I2C_ClearAddr(I2Cx);
	while (Len--)
	{
		while (! (I2Cx->SR1 & I2C_SR1_TXE));
		I2Cx->DR = *TxBuffer++;
	}
	while (! (I2Cx->SR1 & I2C_SR1_TXE));
	while (! (I2Cx->SR1 & I2C_SR1_BTF));
	if (rs == I2C_RS_NO)
	{
		I2C_Stop(I2Cx);
	}
}

void I2C_MasterReceiveData (I2C_TypeDef* I2Cx, uint8_t *RxBuffer, uint32_t Len, uint8_t Addr, I2C_RS_t rs)
{
	I2C_Start(I2Cx);
	I2C_Addr(I2Cx, Addr, I2C_Read);
	
	/* procedure to read only 1 byte from slave */
	if (Len == 1)
	{
		/* Disable Ack */
		I2Cx->CR1 &= ~I2C_CR1_ACK;
		
		I2C_ClearAddr (I2Cx);
		
		while (! (I2Cx->SR1 & I2C_SR1_RXNE));
		
		if (rs == I2C_RS_NO)
		{
			I2C_Stop(I2Cx);
		}
		*RxBuffer++ = I2Cx->DR;
	}
	
	/* procedure to read data from slave when Len > 1 */
	if (Len > 1)
	{
		I2C_ClearAddr (I2Cx);
		/* read the data until Len becomes zero */
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			while (! (I2Cx->SR1 & I2C_SR1_RXNE));
			if (i == 2) /* last 2 bytes remaining */
			{
				/* Disable Ack */
				I2Cx->CR1 &= ~I2C_CR1_ACK;
				if (rs == I2C_RS_NO)
				{
					I2C_Stop(I2Cx);
				}
			}
			*RxBuffer++ = I2Cx->DR;
		}
	}
	/* re-enable ACKing */
	I2Cx->CR1 |= I2C_CR1_ACK;
}

static void I2C_EnableClock(I2C_TypeDef* I2Cx)
{
	if(I2Cx == I2C1) {
		RCC->APB1ENR |=RCC_APB1ENR_I2C1EN;
	}else if (I2Cx == I2C2) {
		RCC->APB1ENR |=RCC_APB1ENR_I2C2EN;
	}else if (I2Cx == I2C3) {
		RCC->APB1ENR |=RCC_APB1ENR_I2C3EN; 
	}
}

static void I2C_Start (I2C_TypeDef* I2Cx)
{
	I2Cx->CR1 |= I2C_CR1_START;
	while (!(I2Cx->SR1 & I2C_SR1_SB));
}

static void I2C_Stop (I2C_TypeDef* I2Cx)
{
	I2Cx->CR1 |= I2C_CR1_STOP;
}

static void I2C_Addr (I2C_TypeDef* I2Cx, uint8_t adr, I2C_ReadWrite_t rw)
{
	if (rw == I2C_Read)
	{
		I2Cx->DR = adr | 0x01;
	}
	else
	{
		I2Cx->DR = adr;
	}
	while(!(I2Cx->SR1 & I2C_SR1_ADDR));
}

static void I2C_ClearAddr (I2C_TypeDef* I2Cx)
{
	/* To clear I2C_SR1_ADDR flag we need to read SR1 and SR2 */
	(void)*(__IO uint16_t *)I2C1->SR1;
	(void)*(__IO uint16_t *)I2C1->SR2;
}