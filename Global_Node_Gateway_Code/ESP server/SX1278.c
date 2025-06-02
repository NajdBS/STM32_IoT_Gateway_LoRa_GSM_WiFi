/**
 * Author Wojciech Domski <Wojciech.Domski@gmail.com>
 * www: www.Domski.pl
 *
 * work based on DORJI.COM sample code and
 * https://github.com/realspinner/SX1278_LoRa
 */

#include "SX1278.h"
#include <string.h>
#include "stm32f4xx.h"
#include "USART.h"
uint8_t rxBuffer1[256];
uint8_t readBytes;
int CMD;
//////////////////////////////////
// logic
//////////////////////////////////

void delay(int ncount)
{
	while(ncount--);
}

void config_register(uint8_t reg,uint8_t data)
{
	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (reg|0x80);

		 /* Wait until TXE flag is set to send data */
	 	 while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)	  
		SPI1->DR = data;
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1) 
	
	
}

uint8_t  read_adress(uint8_t adr)
{
	  uint8_t rxByte = 0x00;

	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (adr);

		 /* Wait until TXE flag is set to send data */
	 	 while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			
		  	

	  //SX1278_hw_SetNSS(hw, 0);
	  GPIOA->ODR&=(0<<4);//NSS=0 (PA4=0)
	  //HAL_SPI_TransmitReceive(hw->spi, &txByte, &rxByte, 1);
	
		while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  rxByte = (SPI1->DR);
			
			GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1) 
	
	//while (HAL_SPI_GetState(hw->spi) != HAL_SPI_STATE_READY);
	return rxByte;
	
	
}


 void SX1278_hw_init(SX1278_hw_t * hw) 
{
	//SX1278_hw_SetNSS(hw, 1);
	GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
	//HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_SET);
	GPIOB->ODR|=(1<<0); //RESET_PIN=1 (PB1=1)
}




//RESET SX1278 (une impulsion de 1ms sur PB1) 
void SX1278_hw_Reset(SX1278_hw_t * hw)
{
	//SX1278_hw_SetNSS(hw, 1);
	GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
	//HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_RESET);
	GPIOB->ODR&=~(1<<0); //RESET_PIN=0 (PB1=0)

	SX1278_hw_DelayMs(1);

	//HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_SET);
  GPIOB->ODR|=(1<<0); //RESET_PIN=1 (PB1=1)
	
	SX1278_hw_DelayMs(100);
}

void SX1278_hw_Reset1(void)
{
	//SX1278_hw_SetNSS(hw, 1);
	GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
	//HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_RESET);
	GPIOB->ODR&=~(1<<0); //RESET_PIN=0 (PB1=0)

	SX1278_hw_DelayMs(1);

	//HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_SET);
  GPIOB->ODR|=(1<<0); //RESET_PIN=1 (PB1=1)
	
	SX1278_hw_DelayMs(100);
}



 uint8_t SX1278_hw_SPIReadByte(SX1278_hw_t * hw) 
{
	uint8_t txByte = 0x00;
	uint8_t rxByte = 0x00;

	//SX1278_hw_SetNSS(hw, 0);
	GPIOA->ODR&=(0<<4);//NSS=0 (PA4=0)
	//HAL_SPI_TransmitReceive(hw->spi, &txByte, &rxByte, 1);
	
		while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  rxByte = (SPI1->DR);
	
	//while (HAL_SPI_GetState(hw->spi) != HAL_SPI_STATE_READY);
	return rxByte;
}

void SX1278_hw_DelayMs(uint32_t msec) 
	{
	//HAL_Delay(msec);
	delay(msec*2000);
}

 

//////////////////////////////////
// logic
//////////////////////////////////




void SX1278_SPIBurstRead(SX1278_t * module, uint8_t addr, uint8_t* rxBuf,uint8_t length) 
{
	  uint8_t i;	
	
	//SX1278_hw_SPICommand(module->hw, addr);
	     GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	     SPI1->CR1 |=  SPI_CR1_SPE;
		   SPI1->DR = (addr); //ADRESSE EN MODE LECTURE

		  /* Wait until TXE flag is set to send data */
	 	   while(!SPI1->SR&0x2){};
	     /* Wait until Busy flag is reset before disabling SPI */
		   while(SPI1->SR&1<<7){};
		   /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		   SPI1->DR;
       SPI1->SR;
	

		for (i =0; i < length; i++) 
		{
			 //*(rxBuf + i) = SX1278_hw_SPIReadByte(module->hw);
			     GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	         while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		       SPI1->DR = 0;  // send dummy data
		       while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	         *(rxBuf + i)= (SPI1->DR);		
		}
		//SX1278_hw_SetNSS(module->hw, 1);
		GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
}


void SX1278_BurstRead( uint8_t addr, uint8_t* rxBuf,uint8_t length) 
{
	  uint8_t i;	
	
	//SX1278_hw_SPICommand(module->hw, addr);
	     GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	     SPI1->CR1 |=  SPI_CR1_SPE;
		   SPI1->DR = (addr); //ADRESSE EN MODE LECTURE

		  /* Wait until TXE flag is set to send data */
	 	   while(!SPI1->SR&0x2){};
	     /* Wait until Busy flag is reset before disabling SPI */
		   while(SPI1->SR&1<<7){};
		   /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		   SPI1->DR;
       SPI1->SR;
	

		for (i =0; i < length; i++) 
		{
			 //*(rxBuf + i) = SX1278_hw_SPIReadByte(module->hw);
			     GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	         while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		       SPI1->DR = 0;  // send dummy data
		       while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	         *(rxBuf + i)= (SPI1->DR);		
		}
		//SX1278_hw_SetNSS(module->hw, 1);
		GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
}


void SX1278_SPIBurstWrite(SX1278_t * module, uint8_t addr, uint8_t* txBuf,uint8_t length)
{
	  unsigned char i;
	
		//SX1278_hw_SetNSS(module->hw, 0);
		GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
		
	  //SX1278_hw_SPICommand(module->hw, addr | 0x80);
	    GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	    SPI1->CR1 |=  SPI_CR1_SPE;
		  SPI1->DR = (addr|0x80);

		  /* Wait until TXE flag is set to send data */
	 	   while(!SPI1->SR&0x2){};
	     /* Wait until Busy flag is reset before disabling SPI */
		   while(SPI1->SR&1<<7){};
		   /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		   SPI1->DR;
       SPI1->SR;
		
	  for (i = 0; i < length; i++) 
		{
			//SX1278_hw_SPICommand(module->hw, *(txBuf + i));
			   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	       SPI1->CR1 |=  SPI_CR1_SPE;
		     SPI1->DR =*(txBuf + i);

		     /* Wait until TXE flag is set to send data */
	 	      while(!SPI1->SR&0x2){};
	       /* Wait until Busy flag is reset before disabling SPI */
		      while(SPI1->SR&1<<7){};
		     /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		      SPI1->DR;
          SPI1->SR;
			
		}
		//SX1278_hw_SetNSS(module->hw, 1);
		GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
}


void SX1278_BurstWrite(uint8_t addr, uint8_t* txBuf,uint8_t length)
{
	  unsigned char i;
	
		//SX1278_hw_SetNSS(module->hw, 0);
		GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
		
	  //SX1278_hw_SPICommand(module->hw, addr | 0x80);
	    GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	    SPI1->CR1 |=  SPI_CR1_SPE;
		  SPI1->DR = (addr|0x80);

		  /* Wait until TXE flag is set to send data */
	 	   while(!SPI1->SR&0x2){};
	     /* Wait until Busy flag is reset before disabling SPI */
		   while(SPI1->SR&1<<7){};
		   /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		   SPI1->DR;
       SPI1->SR;
		
	  for (i = 0; i < length; i++) 
		{
			//SX1278_hw_SPICommand(module->hw, *(txBuf + i));
			   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	       SPI1->CR1 |=  SPI_CR1_SPE;
		     SPI1->DR =*(txBuf + i);

		     /* Wait until TXE flag is set to send data */
	 	      while(!SPI1->SR&0x2){};
	       /* Wait until Busy flag is reset before disabling SPI */
		      while(SPI1->SR&1<<7){};
		     /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		      SPI1->DR;
          SPI1->SR;
			
		}
		//SX1278_hw_SetNSS(module->hw, 1);
		GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	
}


void SX1278_defaultConfig(SX1278_t * module)
{
	//SX1278_config(module, module->frequency, module->power, module->LoRa_Rate,module->LoRa_BW);
	SX1278_config1();
}

void SX1278_defaultConfig1(void)
{
	//SX1278_config(module, module->frequency, module->power, module->LoRa_Rate,module->LoRa_BW);
	SX1278_config1();
}

void SX1278_config(SX1278_t * module, uint8_t frequency, uint8_t power,uint8_t LoRa_Rate, uint8_t LoRa_BW)
{
	//SX1278_sleep(module); //Change modem mode Must in Sleep mode      OK
	SX1278_sleep1();
	
	
	SX1278_hw_DelayMs(15);

	//SX1278_entryLoRa(module); //OK
	SX1278_entryLoRa1();
	//SX1278_SPIWrite(module, 0x5904); //?? Change digital regulator form 1.6V to 1.47V: see errata note

	//SX1278_SPIBurstWrite(module, LR_RegFrMsb,(uint8_t*) SX1278_Frequency[frequency], 3); //setting  frequency parameter     OK
	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegFrMsb|0x80);

		 /* Wait until TXE flag is set to send data */
	 	 while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			  
		SPI1->DR = (0x6C);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(!SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		SPI1->DR = (0x80);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		SPI1->DR = (0x00);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1) 
	
	
	
	//setting base parameter
	//SX1278_SPIWrite(module, LR_RegPaConfig, SX1278_Power[power]); //Setting output power parameter   OK
      config_register(LR_RegPaConfig,SX1278_Power[power]);  
	   
	
	//SX1278_SPIWrite(module, LR_RegOcp, 0x0B);			//RegOcp,Close Ocp     //OK
			config_register(LR_RegOcp, 0x0B);
	   
	
	
	//SX1278_SPIWrite(module, LR_RegLna, 0x23);		//RegLNA,High & LNA Enable       OK
	  config_register(LR_RegLna, 0x23);
			
			  
	
	

		//SX1278_SPIWrite(module,LR_RegModemConfig1,((SX1278_LoRaBandwidth[LoRa_BW] << 4) + (SX1278_CR << 1) + 0x00)); //Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
   // GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
		config_register(LR_RegModemConfig1,((SX1278_LoRaBandwidth[LoRa_BW] << 4) + (SX1278_CR << 1) + 0x00));
		
		//SX1278_SPIWrite(module,LR_RegModemConfig2,((SX1278_SpreadFactor[LoRa_Rate] << 4) + (SX1278_CRC << 2)+ 0x03)); //SFactor &  LNA gain set by the internal AGC loop
	                                               //8<<4+ (1<<2)+0x30
	   config_register(LR_RegModemConfig2,((SX1278_SpreadFactor[LoRa_Rate] << 4) + (SX1278_CRC << 2)+ 0x03));
	
	


	//SX1278_SPIWrite(module, LR_RegSymbTimeoutLsb, 0xFF); //RegSymbTimeoutLsb Timeout = 0x3FF(Max)   OK
	 config_register(LR_RegSymbTimeoutLsb, 0xFF);  
	   
	    
   //SX1278_SPIWrite(module, LR_RegPreambleMsb, 0x00); //RegPreambleMsb            OK
		 config_register( LR_RegPreambleMsb, 0x00);
	
	  		
	//SX1278_SPIWrite(module, LR_RegPreambleLsb, 12); //RegPreambleLsb 8+4=12byte Preamble         OK
	   config_register( LR_RegPreambleLsb, 12);
	  
	  
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING2, 0x01); //RegDioMapping2 DIO5=00, DIO4=01		  OK
	   config_register( REG_LR_DIOMAPPING2, 0x01);
	     	
			
	//module->readBytes = 0;
	//SX1278_standby(module); //Entry standby mode   OK
	SX1278_standby1();
}

void SX1278_config1(void)
{
	//SX1278_sleep(module); //Change modem mode Must in Sleep mode      OK
	SX1278_sleep1();
	
	
	SX1278_hw_DelayMs(15);

	//SX1278_entryLoRa(module); //OK
	SX1278_entryLoRa1();
	//SX1278_SPIWrite(module, 0x5904); //?? Change digital regulator form 1.6V to 1.47V: see errata note

	//SX1278_SPIBurstWrite(module, LR_RegFrMsb,(uint8_t*) SX1278_Frequency[frequency], 3); //setting  frequency parameter     OK
	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegFrMsb|0x80);

		 /* Wait until TXE flag is set to send data */
	 	 while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			  
		SPI1->DR = (0x6C);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(!SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		SPI1->DR = (0x80);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		SPI1->DR = (0x00);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1) 
	
	
	
	//setting base parameter
	//SX1278_SPIWrite(module, LR_RegPaConfig, SX1278_Power[power]); //Setting output power parameter   OK
      config_register(LR_RegPaConfig,SX1278_Power[SX1278_POWER_17DBM]);  
	   
	
	//SX1278_SPIWrite(module, LR_RegOcp, 0x0B);			//RegOcp,Close Ocp     //OK
			config_register(LR_RegOcp, 0x0B);
	   
	
	
	//SX1278_SPIWrite(module, LR_RegLna, 0x23);		//RegLNA,High & LNA Enable       OK
	  config_register(LR_RegLna, 0x23);
			
			  
	
	
	
		//SX1278_SPIWrite(module,LR_RegModemConfig1,((SX1278_LoRaBandwidth[LoRa_BW] << 4) + (SX1278_CR << 1) + 0x00)); //Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
   // GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
		config_register(LR_RegModemConfig1,((SX1278_LoRaBandwidth[SX1278_LORA_BW_20_8KHZ] << 4) + (SX1278_CR << 1) + 0x00));
		
		//SX1278_SPIWrite(module,LR_RegModemConfig2,((SX1278_SpreadFactor[LoRa_Rate] << 4) + (SX1278_CRC << 2)+ 0x03)); //SFactor &  LNA gain set by the internal AGC loop
	                                               //8<<4+ (1<<2)+0x30
	   config_register(LR_RegModemConfig2,((SX1278_SpreadFactor[SX1278_LORA_SF_8] << 4) + (SX1278_CRC << 2)+ 0x03));
	
	


	//SX1278_SPIWrite(module, LR_RegSymbTimeoutLsb, 0xFF); //RegSymbTimeoutLsb Timeout = 0x3FF(Max)   OK
	 config_register(LR_RegSymbTimeoutLsb, 0xFF);  
	   
	    
   //SX1278_SPIWrite(module, LR_RegPreambleMsb, 0x00); //RegPreambleMsb            OK
		 config_register( LR_RegPreambleMsb, 0x00);
	
	  		
	//SX1278_SPIWrite(module, LR_RegPreambleLsb, 12); //RegPreambleLsb 8+4=12byte Preamble         OK
	   config_register( LR_RegPreambleLsb, 12);
	  
	  
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING2, 0x01); //RegDioMapping2 DIO5=00, DIO4=01		  OK
	   config_register( REG_LR_DIOMAPPING2, 0x01);
	     	
			
	//module->readBytes = 0;
	//SX1278_standby(module); //Entry standby mode   OK
	SX1278_standby1();
}

void SX1278_standby(SX1278_t * module)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x09);
	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegOpMode|0x80);

		 /* Wait until TXE flag is set to send data */
	 	while(!SPI1->SR&0x2){};
	/* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			  
		SPI1->DR = (0x09);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)
	    
    module->status = STANDBY;
}

void SX1278_standby1(void)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x09);
	   GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegOpMode|0x80);

		 /* Wait until TXE flag is set to send data */
	 	while(!SPI1->SR&0x2){};
	/* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			  
		SPI1->DR = (0x09);
		/* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
	  /* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)
}

void SX1278_sleep(SX1278_t * module)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x08);	
	
	  GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegOpMode|0x80);
     /* Wait until TXE flag is set to send data */
		  while(!SPI1->SR&0x2){};
		  /* Wait until Busy flag is reset before disabling SPI */
		  while(SPI1->SR&1<<7){};
      /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		 SPI1->DR;
     SPI1->SR;
				
	  GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)	 
		SPI1->DR = (0x08);
		 /* Wait until TXE flag is set to send data */
		 while(!SPI1->SR&0x2){};
			/* Wait until Busy flag is reset before disabling SPI */
		 while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		 SPI1->DR;
     SPI1->SR;
		 GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)	 
		 
	  // module->status = SLEEP;
}

void SX1278_sleep1(void)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x08);	
	
	  GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	   SPI1->CR1 |=  SPI_CR1_SPE;
		 SPI1->DR = (LR_RegOpMode|0x80);
     /* Wait until TXE flag is set to send data */
		  while(!SPI1->SR&0x2){};
		  /* Wait until Busy flag is reset before disabling SPI */
		  while(SPI1->SR&1<<7){};
      /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		 SPI1->DR;
     SPI1->SR;
				
	  GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)	 
		SPI1->DR = (0x08);
		 /* Wait until TXE flag is set to send data */
		 while(!SPI1->SR&0x2){};
			/* Wait until Busy flag is reset before disabling SPI */
		 while(SPI1->SR&1<<7){};
		 /* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		 SPI1->DR;
     SPI1->SR;
		 GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)	 
		 
	  // module->status = SLEEP;
}

void SX1278_entryLoRa(SX1278_t * module)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x88);
	
	 GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	 SPI1->CR1 |=  SPI_CR1_SPE;
	 SPI1->DR = (LR_RegOpMode|0x80);
	 /* Wait until TXE flag is set to send data */
	 while(!SPI1->SR&0x2){};
	 /* Wait until Busy flag is reset before disabling SPI */
	 while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			
	  
		SPI1->DR = (0x88);
	 /* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
		/* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)			
}

void SX1278_entryLoRa1(void)
{
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x88);
	
	 GPIOA->ODR&=(0<<4); //NSS=0 (PA4=0)
	 SPI1->CR1 |=  SPI_CR1_SPE;
	 SPI1->DR = (LR_RegOpMode|0x80);
	 /* Wait until TXE flag is set to send data */
	 while(!SPI1->SR&0x2){};
	 /* Wait until Busy flag is reset before disabling SPI */
	 while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
			
	  
		SPI1->DR = (0x88);
	 /* Wait until TXE flag is set to send data */
		while(!SPI1->SR&0x2){};
		/* Wait until Busy flag is reset before disabling SPI */
		while(SPI1->SR&1<<7){};
		/* Clear OVERRUN flag in 2 Lines communication mode because received is not read */
		SPI1->DR;
    SPI1->SR;
		GPIOA->ODR|=(1<<4); //NSS=1 (PA4=1)			
}

void SX1278_clearLoRaIrq(SX1278_t * module)
{
	//SX1278_SPIWrite(module, LR_RegIrqFlags, 0xFF);
	config_register(LR_RegIrqFlags,0xFF);
}

void SX1278_clearLoRaIrq1(void)
{
	//SX1278_SPIWrite(module, LR_RegIrqFlags, 0xFF);
	config_register(LR_RegIrqFlags,0xFF);
}

int SX1278_LoRaEntryRx(SX1278_t * module, uint8_t length, uint32_t timeout)
{
	uint8_t addr;

	module->packetLength = length;

	//SX1278_defaultConfig(module);		//Setting base parameter
	SX1278_defaultConfig1();
	
	
	//SX1278_SPIWrite(module, REG_LR_PADAC, 0x84);	//Normal and RX
	config_register(REG_LR_PADAC,0x84);
	
	
	//SX1278_SPIWrite(module, LR_RegHopPeriod, 0xFF);	//No FHSS
	  config_register(LR_RegHopPeriod,0xFF);
	
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING1, 0x01);//DIO=00,DIO1=00,DIO2=00, DIO3=01
	  config_register(REG_LR_DIOMAPPING1,0x01);
	
	//SX1278_SPIWrite(module, LR_RegIrqFlagsMask, 0x3F);//Open RxDone interrupt & Timeout
	  config_register(REG_LR_DIOMAPPING1,0x01);
	
	//SX1278_clearLoRaIrq(module);
	  SX1278_clearLoRaIrq1();
	
	//SX1278_SPIWrite(module, LR_RegPayloadLength, length);//Payload Length 21byte(this register must difine when the data long of one byte in SF is 6)
	  config_register(LR_RegPayloadLength,length);
	
	//addr = SX1278_SPIRead(module, LR_RegFifoRxBaseAddr); //Read RxBaseAddr
	  addr=read_adress(LR_RegFifoRxBaseAddr);
	
	
	//SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RxBaseAddr->FiFoAddrPtr
	  config_register(LR_RegFifoAddrPtr,addr);
	
	
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x8d);	//Mode//Low Frequency Mode
	  config_register(LR_RegOpMode,0x8d);
	
	//SX1278_SPIWrite(module, LR_RegOpMode,0x05);	//Continuous Rx Mode //High Frequency Mode
	  module->readBytes = 0;

	while (1)
	{
		//if ((SX1278_SPIRead(module, LR_RegModemStat) & 0x04) == 0x04)  //Rx-on going RegModemStat
		//{
			
		//	return 1;
		//}
		
		if ((read_adress(LR_RegModemStat) & 0x04) == 0x04)  //Rx-on going RegModemStat
		{		
			return 1;
		}
		timeout--;
		if (timeout == 0)
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
		SX1278_hw_DelayMs(1);
	}
}

int SX1278_LoRaEntryRx1(uint8_t length, uint32_t timeout)
{
	uint8_t addr;

	

	//SX1278_defaultConfig(module);		//Setting base parameter
	SX1278_defaultConfig1();
	
	
	//SX1278_SPIWrite(module, REG_LR_PADAC, 0x84);	//Normal and RX
	config_register(REG_LR_PADAC,0x84);
	
	
	//SX1278_SPIWrite(module, LR_RegHopPeriod, 0xFF);	//No FHSS
	  config_register(LR_RegHopPeriod,0xFF);
	
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING1, 0x01);//DIO=00,DIO1=00,DIO2=00, DIO3=01
	  config_register(REG_LR_DIOMAPPING1,0x01);
	
	//SX1278_SPIWrite(module, LR_RegIrqFlagsMask, 0x3F);//Open RxDone interrupt & Timeout
	  config_register(REG_LR_DIOMAPPING1,0x01);
	
	//SX1278_clearLoRaIrq(module);
	  SX1278_clearLoRaIrq1();
	
	//SX1278_SPIWrite(module, LR_RegPayloadLength, length);//Payload Length 21byte(this register must difine when the data long of one byte in SF is 6)
	  config_register(LR_RegPayloadLength,length);
	
	//addr = SX1278_SPIRead(module, LR_RegFifoRxBaseAddr); //Read RxBaseAddr
	  addr=read_adress(LR_RegFifoRxBaseAddr);
	
	
	//SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RxBaseAddr->FiFoAddrPtr
	  config_register(LR_RegFifoAddrPtr,addr);
	
	
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x8d);	//Mode//Low Frequency Mode
	  config_register(LR_RegOpMode,0x8d);
	
	//SX1278_SPIWrite(module, LR_RegOpMode,0x05);	//Continuous Rx Mode //High Frequency Mode
	  readBytes = 0;

	while (1)
	{
		//if ((SX1278_SPIRead(module, LR_RegModemStat) & 0x04) == 0x04)  //Rx-on going RegModemStat
		//{
			
		//	return 1;
		//}
		
		if ((read_adress(LR_RegModemStat) & 0x04) == 0x04)  //Rx-on going RegModemStat
		{		
			return 1;
		}
		timeout--;
		if (timeout == 0)
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
		SX1278_hw_DelayMs(1);
	}
}

uint8_t SX1278_LoRaRxPacket(SX1278_t * module)
{
	unsigned char addr;
	unsigned char packet_size;

	//if (SX1278_hw_GetDIO0(module->hw)) 
	if(GPIOB->IDR&0x1)
	{
		 memset(rxBuffer1, 0x00, SX1278_MAX_PACKET);

		//addr = SX1278_SPIRead(module, LR_RegFifoRxCurrentaddr); //last packet addr
		      addr=read_adress(LR_RegFifoRxCurrentaddr);
		      
		 //SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RxBaseAddr -> FiFoAddrPtr
           config_register(LR_RegFifoAddrPtr,addr);
		
		
		if (module->LoRa_Rate == SX1278_LORA_SF_6)  //When SpreadFactor is six,will used Implicit Header mode(Excluding internal packet length)
		{	
			packet_size = module->packetLength;
		} 
		else 
		{
		 //	packet_size = SX1278_SPIRead(module, LR_RegRxNbBytes); //Number for received bytes
         packet_size=read_adress(LR_RegRxNbBytes);                			
		}

		//SX1278_SPIBurstRead(module, 0x00,&rxBuffer1[0], packet_size);
		SX1278_BurstRead( 0x00,&rxBuffer1[0], packet_size);
		
		
		module->readBytes = packet_size;
		//SX1278_clearLoRaIrq(module);
		  SX1278_clearLoRaIrq1();
	}
	return module->readBytes;
}


uint8_t SX1278_LoRaRxPacket1(void)
{
	unsigned char addr;
	unsigned char packet_size;

	//if (SX1278_hw_GetDIO0(module->hw)) 
	if(GPIOB->IDR&0x1)
	{
		 memset(rxBuffer1, 0x00, SX1278_MAX_PACKET);

		//addr = SX1278_SPIRead(module, LR_RegFifoRxCurrentaddr); //last packet addr
		      addr=read_adress(LR_RegFifoRxCurrentaddr);
		      
		 //SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RxBaseAddr -> FiFoAddrPtr
           config_register(LR_RegFifoAddrPtr,addr);
		
		

		
		packet_size=read_adress(LR_RegRxNbBytes); 

		//SX1278_SPIBurstRead(module, 0x00,&rxBuffer1[0], packet_size);
		SX1278_BurstRead( 0x00,&rxBuffer1[0], packet_size);
		
		
		readBytes = packet_size;
		//SX1278_clearLoRaIrq(module);
		SX1278_clearLoRaIrq1();
	}
	return readBytes;
}

int SX1278_LoRaEntryTx(SX1278_t * module, uint8_t length, uint32_t timeout) 
{
	uint8_t addr;
	uint8_t temp;

	//module->packetLength = length;

	//SX1278_defaultConfig(module); //setting base parameter
	SX1278_defaultConfig1();
	
	
	//SX1278_SPIWrite(module, REG_LR_PADAC, 0x87);	//Tx for 20dBm
	config_register(REG_LR_PADAC,0x87);
	
	
	//SX1278_SPIWrite(module, LR_RegHopPeriod, 0x00); //RegHopPeriod NO FHSS
	config_register(LR_RegHopPeriod,0x00);
	
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING1, 0x41); //DIO0=01, DIO1=00,DIO2=00, DIO3=01
	  config_register(REG_LR_DIOMAPPING1,0x41);
	
	//SX1278_clearLoRaIrq(module);
	  SX1278_clearLoRaIrq1();
	
	//SX1278_SPIWrite(module, LR_RegIrqFlagsMask, 0xF7); //Open TxDone interrupt
	config_register(LR_RegIrqFlagsMask,0xF7);
	
	//SX1278_SPIWrite(module, LR_RegPayloadLength, length); //RegPayloadLength 21byte
	config_register(LR_RegPayloadLength,length);
	
	//addr = SX1278_SPIRead(module, LR_RegFifoTxBaseAddr); //RegFiFoTxBaseAddr
	    addr=read_adress(LR_RegFifoTxBaseAddr);
	
	
	//SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RegFifoAddrPtr
  config_register(LR_RegFifoAddrPtr,addr);
	while (1) 
	{
		//temp = SX1278_SPIRead(module, LR_RegPayloadLength);
		temp=read_adress(LR_RegPayloadLength);
		
		if (temp == length) 
		{
			//module->status = TX;
			return 1;
		}

		if (--timeout == 0) 
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
	}
}

int SX1278_LoRaEntryTx1( uint8_t length, uint32_t timeout) 
{
	uint8_t addr;
	uint8_t temp;

	//module->packetLength = length;

	//SX1278_defaultConfig(module); //setting base parameter
	SX1278_defaultConfig1();
	
	
	//SX1278_SPIWrite(module, REG_LR_PADAC, 0x87);	//Tx for 20dBm
	config_register(REG_LR_PADAC,0x87);
	
	
	//SX1278_SPIWrite(module, LR_RegHopPeriod, 0x00); //RegHopPeriod NO FHSS
	config_register(LR_RegHopPeriod,0x00);
	
	//SX1278_SPIWrite(module, REG_LR_DIOMAPPING1, 0x41); //DIO0=01, DIO1=00,DIO2=00, DIO3=01
	  config_register(REG_LR_DIOMAPPING1,0x41);
	
	//SX1278_clearLoRaIrq(module);
	  SX1278_clearLoRaIrq1();
	
	//SX1278_SPIWrite(module, LR_RegIrqFlagsMask, 0xF7); //Open TxDone interrupt
	config_register(LR_RegIrqFlagsMask,0xF7);
	
	//SX1278_SPIWrite(module, LR_RegPayloadLength, length); //RegPayloadLength 21byte
	config_register(LR_RegPayloadLength,length);
	
	//addr = SX1278_SPIRead(module, LR_RegFifoTxBaseAddr); //RegFiFoTxBaseAddr
	    addr=read_adress(LR_RegFifoTxBaseAddr);
	
	
	//SX1278_SPIWrite(module, LR_RegFifoAddrPtr, addr); //RegFifoAddrPtr
  config_register(LR_RegFifoAddrPtr,addr);
	while (1) 
	{
		//temp = SX1278_SPIRead(module, LR_RegPayloadLength);
		temp=read_adress(LR_RegPayloadLength);
		
		if (temp == length) 
		{
			//module->status = TX;
			return 1;
		}

		if (--timeout == 0) 
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
	}
}


int SX1278_LoRaTxPacket(SX1278_t * module, uint8_t* txBuffer, uint8_t length,uint32_t timeout) 
{
	//SX1278_SPIBurstWrite(module, 0x00, txBuffer, length);
	SX1278_BurstWrite(0x00, txBuffer, length);
	
	
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x8b);	//Tx Mode
	 config_register(LR_RegOpMode,0x8b);
	
	while (1) 
	{
		//if (SX1278_hw_GetDIO0(module->hw))  //if(Get_NIRQ()) //Packet send over  
		if(GPIOB->IDR&0x1)
		{
			//SX1278_SPIRead(module, LR_RegIrqFlags);
			read_adress(LR_RegIrqFlags);
			
			
			//SX1278_clearLoRaIrq(module); //Clear irq
			SX1278_clearLoRaIrq1();
			
			//SX1278_standby(module); //Entry Standby mode
			SX1278_standby1();
			return 1;
		}

		if (--timeout == 0) 
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
		SX1278_hw_DelayMs(1);
	}
}

int SX1278_LoRaTxPacket1(uint8_t* txBuffer, uint8_t length,uint32_t timeout) 
{
	//SX1278_SPIBurstWrite(module, 0x00, txBuffer, length);
	SX1278_BurstWrite(0x00, txBuffer, length);
	
	
	//SX1278_SPIWrite(module, LR_RegOpMode, 0x8b);	//Tx Mode
	 config_register(LR_RegOpMode,0x8b);
	
	while (1) 
	{
		//if (SX1278_hw_GetDIO0(module->hw))  //if(Get_NIRQ()) //Packet send over  
		if(GPIOB->IDR&0x1)
		{
			//SX1278_SPIRead(module, LR_RegIrqFlags);
			read_adress(LR_RegIrqFlags);
			
			
			//SX1278_clearLoRaIrq(module); //Clear irq
			SX1278_clearLoRaIrq1();
			
			//SX1278_standby(module); //Entry Standby mode
			SX1278_standby1();
			return 1;
		}

		if (--timeout == 0) 
		{
			//SX1278_hw_Reset(module->hw);
			SX1278_hw_Reset1();
			
			//SX1278_defaultConfig(module);
			SX1278_defaultConfig1();
			return 0;
		}
		SX1278_hw_DelayMs(1);
	}
}

void SX1278_begin(SX1278_t * module, uint8_t frequency, uint8_t power,uint8_t LoRa_Rate, uint8_t LoRa_BW, uint8_t packetLength)
{
	//SX1278_hw_init(module->hw);
   	//SX1278_hw_SetNSS(hw, 1);
	  GPIOA->ODR|=(1<<4); //(NSS=1) PA4=1
	  //HAL_GPIO_WritePin(hw->reset.port, hw->reset.pin, GPIO_PIN_SET);
	  GPIOB->ODR|=(1<<0); //RESET_PIN=1 (PB1=1)
	
	
	module->frequency = frequency;
	module->power = power;
	module->LoRa_Rate = LoRa_Rate;
	module->LoRa_BW = LoRa_BW;
	module->packetLength = packetLength;
	//SX1278_defaultConfig(module);
	SX1278_defaultConfig1();
}

void SX1278_begin1(void)
{
	SX1278_defaultConfig1();
}

int SX1278_transmit(SX1278_t * module, uint8_t* txBuf, uint8_t length,uint32_t timeout) 
{
	if (SX1278_LoRaEntryTx(module, length, timeout)) 
	{
		return SX1278_LoRaTxPacket(module, txBuf, length, timeout);
	}
	return 0;
}

int SX1278_receive(SX1278_t * module, uint8_t length, uint32_t timeout) 
{
	return SX1278_LoRaEntryRx(module, length, timeout);
}

uint8_t SX1278_available(SX1278_t * module) 
{
	return SX1278_LoRaRxPacket(module);
}

uint8_t SX1278_read(SX1278_t * module, uint8_t* rxBuf, uint8_t length) 
{
	//if (length != module->readBytes)
	//	length = module->readBytes;
	memcpy(rxBuf, rxBuffer1, length);
	rxBuf[length] = '\0';
	module->readBytes = 0;
	return length;
}

uint8_t SX1278_read1( uint8_t* rxBuf, uint8_t length) 
{
	//if (length != module->readBytes)
	//	length = module->readBytes;
	memcpy(rxBuf, rxBuffer1, length);
	rxBuf[length] = '\0';
	readBytes = 0;
	return length;
}

/*************************************************/
void LoRa_Receive_Mode(void)
{
    // Display the initialization message
    SendTxt_usart1("Configuring LoRa module\r\n");

    // Initialize the LoRa module (SX1278)
    SX1278_begin1();

    // Display that initialization is done
    SendTxt_usart1("Done configuring LoRa Module\r\n");

    // Enter receive mode with a timeout of 2000 ms and a threshold of 16 bytes
    SX1278_LoRaEntryRx1(16, 2000);

    // Display the current operating mode
    //SendTxt_usart1("Slave ...\r\n");
}



void Parse_LoRa_Data(const char* buffer)
{
		if (!strstr(buffer, " C\n"))
        return;
    const char* ch1_str = strstr(buffer, "CH1=");
    const char* ch2_str = strstr(buffer, "CH2=");
    const char* ch3_str = strstr(buffer, "CH3=");
    const char* temp_str = strstr(buffer, "Temp_DS1621=");

    if (ch1_str && ch2_str && ch3_str && temp_str)
    {
        sscanf(ch1_str, "CH1=%hu", &Lora_ADC_Value[0]); 
        sscanf(ch2_str, "CH2=%hu", &Lora_ADC_Value[1]);
        sscanf(ch3_str, "CH3=%hu", &Lora_ADC_Value[2]);
        sscanf(temp_str, "Temp_DS1621=%f", &Lora_temperature);
    }
		
	flag_lora=1;
}


