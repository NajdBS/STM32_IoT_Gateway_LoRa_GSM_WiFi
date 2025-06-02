#include "stm32f4xx.h"
#include "string.h"
#include <stdio.h>
#include "SX1278.h"
#include "I2C.h"

#define dataUploadInterval 24999 //ARR (ms)
#define IWDG_TimeOut 59999

extern uint8_t rxBuffer1[256];
typedef enum {STATE0,STATE1,STATE2}STATES;
int PSTATE=STATE0;
int i;
int ADC_VALUE_REC[2];
uint16_t ADC_Value[3];

char msg1[100];
char msg2[100]="Master ...\r\n";
char msg3[100]="Mode: Slave\r\n";
char msg4[100]="Configuring LoRa module\r\n";
char msg5[100]="Done configuring LoRaModule\r\n";
char msg6[100]="Sending package...\r\n";
char msg7[100]="Package sent...\r\n";

char msg8[100]="Slave ...\r\n";
char msg9[100]="Receiving package...\r\n";
char msg10[100]="Package received ...\r\n";

int ret;

char buffer[64];
char buffer1[64];
char buffer2[128];
char buffer3[128];
char buffer4[64];
char buffer5[64];
int message;
int message_length;

void config_SPI1(void);
void config_GPIO(void);
void function_state0(void);
void function_state1(void);
void function_state2(void);

bool flag = false;
float temperature;

void Usart2_Init() //TX(PA2) RX(PA3)
{
  // Enable clock for GPIOA
  RCC->AHB1ENR |= 0x00000001;
  // Enable clock for USART2
  RCC->APB1ENR|=0x00020000;
  //enable USART3_TX to PA2 and USART3_RX to PA3
  GPIOA->AFR[0]|=0x00007700;
  // configuring the USART2 ALTERNATE function  to PA2 and PA3
  GPIOA->MODER|=0x000000A0;
  //  BaudRate:115200
  USART2->BRR=1667;
  //USART3->BRR     = 0x0890;      // 19200 baud
  //USART4->BRR     = 0x0450;      // 38400 baud
  //USART4->BRR     = 0x02e0;      // 57600 baud
  //USART4->BRR     = 0x016d;      // 115200 baud

  // USART3 enable
  USART2->CR1|=0x0000202C;
  // PERMISSION D'INTERRUPTION
  // NVIC->ISER[1]|=0x80;
 // NVIC_EnableIRQ(USART2_IRQn);
}

void SendChar_usart2(char Tx)
{
	while((USART2->SR&0x80)==0);  // On attend Ã  ce que TXBUFF soit vide (libere) ou (FLAG TXNE=1) ou Le caractere est envoyÃ©
	USART2->DR=Tx;
}

/**********************************************************************************
    //TRANSMISION d'une chaine caractÃ¨re: STRING
 **********************************************************************************/
void SendTxt_usart2(char *Adr)
{
  while(*Adr)
  {
    SendChar_usart2(*Adr);
    Adr++;
  }
}


/* SPI1 init function */
void SPI1_Init(void)
{
	  RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
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

void config_GPIO_ADC_Channnel(void)
{
    RCC->AHB1ENR |= 0x1;   // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 2); // Enable GPIOC clock (for PC4 and PC5)

    // Configure PA1, PC4, and PC5 as Analog Input
    GPIOA->MODER |= (0x3 << 2*1);    // PA1 as Analog (00: Analog mode)
    GPIOC->MODER |= (0x3 << 2*4);    // PC4 as Analog (00: Analog mode)
    GPIOC->MODER |= (0x3 << 2*5);    // PC5 as Analog (00: Analog mode)
}


void config_TIMER3(void)
{
    // Enable clock for Timer 3 (APB1)
    RCC->APB1ENR |= (1 << 1); //enable TIM3
	
    // Configure the prescaler (PSC) and auto-reload value (ARR)
    TIM3->PSC = 15999;     // Prescaler: 16 MHz / (15999 + 1) = 1 kHz
    TIM3->ARR = dataUploadInterval;       // Auto-reload value for 1-second interval
	  
		// Configure TRGO (Trigger Output) to send an "Update Event" signal to ADC
		TIM3->CR2 &= ~(7 << 4); // Clear MMS bits (bits 6:4)
		TIM3->CR2 |= (2 << 4);  // Set MMS = 010 (Update Event as TRGO)
	
		// Start Timer 3
    TIM3->CR1 |= 0x1;
}

void config_ADC1(void)
{
    // Enable ADC1 clock
    RCC->APB2ENR |= (1 << 8);    // Enable ADC1 clock

    // Configure ADC1
    ADC1->CR1 |= (1 << 8);       // SCAN mode enabled (multi-channel)
    ADC1->CR1 |= (1 << 11);      // Discontinuous mode enabled (DISCEN)
    ADC1->CR1 &= ~(0b111 << 13); // Clear [15:13] bits DISCNUM 
    ADC1->CR1 |= (2 << 13);      // Set DISCNUM = 010 (3 channel)

    // Configure sequence for channels 1, 14, and 15
    ADC1->SQR1 &= ~(0xF << 20);  // Clear L bits [23:20]
    ADC1->SQR1 |= 2 << 20;       // Set L = 2 (Total 3 conversions: L+1 = 3)
    ADC1->SQR3 |= (1 << 0) | (14 << 5) | (15 << 10); // Configure channels 1, 14, and 15
    
    // Configure sampling time for the channels
    // ADC1->SMPR2 &= ~((0b111 << 3)); // Clear [5:3] bits, N1 = 3
    // ADC1->SMPR1 &= ~((0b111 << 12) | (0b111 << 15)); // Clear [5:3] bits, N1 = 3
		// Sampling time: tconv = (3+12) * (1/8) ~= 2µs / PCLK2 = 2

    // Configure ADC/DMA
    ADC1->CR2 |= (1 << 8) | (1 << 9);  // Enable DMA Mode and Continuous DMA Requests

    // Select External Trigger Source: TIM3_TRGO (EXTSEL = 1000)
    ADC1->CR2 &= ~(15 << 24);  // Clear EXTSEL bits [27:24]
    ADC1->CR2 |= (8 << 24);    // Set EXTSEL = 1000 (TIM3_TRGO)
    ADC1->CR2 &= ~(3 << 28);   // Clear EXTEN bits [29:28]
    ADC1->CR2 |= (1 << 28);    // Set EXTEN = 01 (Trigger on rising edge)
    
    // Enable ADC
    ADC1->CR2 |= 1;            // Turn on ADC
}

void config_DMA2(void)
{
		RCC->AHB1ENR |= 1<<22;// Enable DMA2 clock
	
		DMA2_Stream0->CR &= ~(0b111);// Select Channel 0 for ADC1
		//DMA2_Stream0->CR &= ~(0b11<<16);// Low Level Priority
		DMA2_Stream0->CR |= 1<<8;// Enable circular mode
		
		DMA2_Stream0->CR |= 1<<13;// Memory data size 16-bit
		DMA2_Stream0->CR |= 1<<11;// Peripheral data size 16-bit
		//DMA2_Stream0->CR &= ~(1<<6);// Data direction : Peripheral to Memory
		DMA2_Stream0->CR |= 1<<10;// Memory increment
		DMA2_Stream0->CR |= 1<<4;// Enable TCIE
		//DMA2_Stream0->CR &= ~(1<<18);// DBM=0;
		
		DMA2_Stream0->NDTR = 3;// Number of data to transfer
		DMA2_Stream0->M0AR = (uint32_t)&ADC_Value[0];// Set memory address : Address destination
		DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;// Set peripheral address : Address source
		
		//DMA2_Stream0->FCR &=~ (1<<2);// Enable Direct Mode and disable FIFO
		NVIC_EnableIRQ(DMA2_Stream0_IRQn);   // Enable DMA2_Stream0 global interrupt	
		DMA2_Stream0->CR |= 1<<0;// Enable STREAM   
}
// Function to clear a buffer
void ClearBuffer(char *buffer, int size) 
{
    for (int j = 0; j < size; j++) {

			buffer[j] = '\0';
    }
}	
void delay_ms(uint32_t ms) {
    SysTick->LOAD = (SystemCoreClock / 1000) - 1; // Load the value for 1ms delay
    SysTick->VAL = 0; // Reset the current value
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick

    for (uint32_t i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for the overflow flag
    }

    SysTick->CTRL = 0; // Disable SysTick after usage
}

//IWDG
void IWDG_Init(void)
{
	IWDG->KR	|=	0x5555;
	IWDG->PR	|=	0x06;
	IWDG->RLR	=	IWDG_TimeOut * (32000/256)/1000;
	IWDG->KR	|=	0xAAAA;
	IWDG->KR	|=	0xCCCC;
}
void IWDG_Refresh(void)
{
	//SendTxt_usart2("\t#################### System REFRESHING ####################\n");
	IWDG->KR	|=	0xAAAA;
}


void enter_sleep_mode(void)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;  // Sleep mode 
    //SCB->SCR &= ~(1<<2); //sleepdeep bit =0
    __WFI();  // Wait For Interrupt
}

int main(void)
{
	IWDG_Init();
  GPIO_Init();
  config_GPIO_ADC_Channnel();
	SPI1_Init();
  Usart2_Init();
  config_ADC1();  
	config_TIMER3();
	config_DMA2();
  I2C1_Init();
	DS1621_Init();
  
	// Send initial messages to USART2
  SendTxt_usart2(msg2); // "Master ..."
  SendTxt_usart2(msg4); // "Configuring LoRa module"
  
  // Initialize LoRa module
  SX1278_begin1();
  
  // Enter TX mode
  SX1278_LoRaEntryTx1(16, 2000);
  
  SendTxt_usart2(msg5); // "Done configuring LoRa module"
  
  RCC->AHB1ENR|=0x8;
  GPIOD->MODER=0x55<<24;
 
  while (1)
  {
	if (flag == true){
		temperature=DS1621_Read_Temp();
    sprintf(buffer1,"CH1=%d CH2=%d CH3=%d Temp_DS1621=%.2f C\n",ADC_Value[0],ADC_Value[1],ADC_Value[2],temperature);
    // Indicate package sending
    SendTxt_usart2(msg6); // "Sending package..."
    
    // Send the message via LoRa
    SX1278_LoRaEntryTx1(strlen(buffer1), 2000);         
    SX1278_LoRaTxPacket1((uint8_t *)buffer1, strlen(buffer1), 2000);    
    
    // Indicate package sent
    SendTxt_usart2(msg7); // "Package sent..."
    ClearBuffer(buffer1, sizeof(buffer1));
    // Delay between transmissions
    //delay_ms(5000);
		flag = false;
	}
	 IWDG_Refresh();
	
	 SendTxt_usart2("Entering Sleep Mode...\r\n");
	 
		// Enter mode Sleep
   enter_sleep_mode();
	
	 SendTxt_usart2("Woke up from Sleep Mode.\r\n");
  }
}

void DMA2_Stream0_IRQHandler(void) {
		if (DMA2->LISR & (1 << 5)) // Check if Transfer Complete (TCIF0) is set
		{  
		flag=true;
		DMA2->LIFCR |= (1 << 5);// Clear the interrupt flag
    }
}












