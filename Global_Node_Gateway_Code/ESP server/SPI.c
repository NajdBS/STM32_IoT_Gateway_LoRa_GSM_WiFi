#include "SPI.h"


/* SPI1 init function */
void SPI1_Init(void)
{
	  RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
  //PA4     ------> SPI1_NSS (Hardware NSS) used as software nss   
	*/
	  GPIOA->MODER|=0xA800;
		GPIOA->OSPEEDR|=0xFC00;
    GPIOA->AFR[0]|=0x55500000;

  SPI1->CR1 &=  ~SPI_CR1_SPE;
	 
  SPI1->CR1 =0x304;
 
  SPI1->CR2 =0x0;

  SPI1->CRCPR =0xA;
}

void GPIO_Init(void)
{ 
	
	RCC->AHB1ENR|=0x3;
	
	//PA4(NSS) output
	GPIOA->MODER|=0x1<<8;
	
	//PB0 PB1 INPUT
	GPIOB->MODER|=0x00;
}
