/**	
* |----------------------------------------------------------------------
* | Copyright (c) 2019 Aleksander Dykman
* |
* |----------------------------------------------------------------------
*/

#include "stm32f4_spi.h"

/* Private functions */
static void SPI_EnableClock(SPI_TypeDef* SPIx);

void SPI_Init (SPI_TypeDef* SPIx, SPI_Config_t* Config)
{
	uint16_t tmp = 0;
	
	SPI_EnableClock(SPIx);
	
	/* Configure Mode */
	tmp |= Config->MODE << SPI_CR1_MSTR_Pos;
	
	/* Configure Data Frame Format */
	tmp |= Config->DFF << SPI_CR1_DFF_Pos;
	
	/* Configure Clock Polarity */
	tmp |= Config->CPOL << SPI_CR1_CPOL_Pos;
	
	/* Configure Clock Phase */
	tmp |= Config->CPHA << SPI_CR1_CPHA_Pos;
	
	/* Configure Slave Select Management */
	tmp |= Config->SSM << SPI_CR1_SSM_Pos;
	
	/* Configure Internal Slave Select */
	tmp |= Config->SSI << SPI_CR1_SSI_Pos;
	
	/* Configure Baud Rate */
	tmp |= Config->BR << SPI_CR1_BR_Pos;
	
	/* Enable SPI */
	tmp |= SPI_CR1_SPE;
	
	SPIx->CR1 |= tmp;
}

void SPI_SendData (SPI_TypeDef* SPIx, uint8_t *TxBuffer, uint32_t Len)
{
	while (Len--)
	{
		/* Wait until Transmit Buffer Empty */
		while (!(SPIx->SR & SPI_SR_TXE));
		SPIx->DR = *TxBuffer++;
		
		/* Wait until Receive Buffer Not Empty */
		while (!(SPIx->SR & SPI_SR_RXNE));
		(void)*(__IO uint16_t *)SPIx->DR;
	}
}

void SPI_ReceiveData (SPI_TypeDef* SPIx, uint8_t *RxBuffer, uint32_t Len)
{
	uint8_t dummy = 0xFF;
	while (Len--)
	{
		/* Wait until Transmit Buffer Empty */
		while (!(SPIx->SR & SPI_SR_TXE));
		SPIx->DR = dummy;
		
		/* Wait until Receive Buffer Not Empty */
		while (!(SPIx->SR & SPI_SR_RXNE));
		*RxBuffer++ = SPIx->DR;
	}
}

static void SPI_EnableClock(SPI_TypeDef* SPIx)
{
	if(SPIx == SPI1) {
		RCC->APB2ENR |=RCC_APB2ENR_SPI1EN;
	}else if (SPIx == SPI2) {
		RCC->APB1ENR |=RCC_APB1ENR_SPI2EN;
	}else if (SPIx == SPI3) {
		RCC->APB1ENR |=RCC_APB1ENR_SPI3EN; 
	}
}
