#include "USART.h"

void USART1_Init(void)
{
    // 1. Enable GPIOB clock (bit 1 in AHB1ENR)
    RCC->AHB1ENR |= (1 << 1);

    // 2. Set PB6 and PB7 to Alternate Function mode (10)
    GPIOB->MODER &= ~((3 << (2 * 6)) | (3 << (2 * 7)));  // Clear mode bits
    GPIOB->MODER |=  ((2 << (2 * 6)) | (2 << (2 * 7)));  // Set to AF mode

    // 3. Set Alternate Function 7 (AF7 = USART1) for PB6 and PB7
    GPIOB->AFR[0] &= ~((0xF << (4 * 6)) | (0xF << (4 * 7)));  // Clear AF bits
    GPIOB->AFR[0] |=  ((7 << (4 * 6)) | (7 << (4 * 7)));      // Set AF7

    // 4. Enable USART1 clock on APB2 bus (bit 4 in APB2ENR)
    RCC->APB2ENR |= (1 << 4);

    // 5. Set baud rate to 115200 (assuming 16 MHz clock, BRR ˜ 0x008B)
    USART1->BRR = 0x008B;

    // 6. Enable transmitter and USART module
    USART1->CR1 |= (1 << 3);   // TE: Transmitter Enable
    USART1->CR1 |= (1 << 13);  // UE: USART Enable
}
void SendChar_usart1(char c)
{
    // Wait until transmit data register is empty (TXE = 1)
    while ((USART1->SR & 0x80) == 0);
    USART1->DR = c;  // Load the data into the data register
}

void SendTxt_usart1(char *str)
{
    // Send each character of the string
    while (*str)
    {
        SendChar_usart1(*str++);
    }
}
/*
void USART6_Init(void)
{
    // Enable GPIOC clock (GPIOC is used for USART6 TX/RX on STM32F407VG)
    RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC clock (bit 2 in AHB1ENR)

    // Set PC6 (TX) and PC7 (RX) to alternate function (AF)
    GPIOC->MODER &= ~( (3 << (2 * 6)) | (3 << (2 * 7)) );   // Clear mode bits for PC6 and PC7
    GPIOC->MODER |= ( (2 << (2 * 6)) | (2 << (2 * 7)) );    // Set PC6 and PC7 to alternate function (AF)

    // Set AF8 (USART6) for PC6 and PC7
    GPIOC->AFR[0] &= ~( (0xF << (4 * 6)) | (0xF << (4 * 7)) );  // Clear AF bits
    GPIOC->AFR[0] |= ( (8 << (4 * 6)) | (8 << (4 * 7)) );       // Set AF8 (USART6) for PC6 and PC7

    // Configure USART6
    RCC->APB2ENR |= (1 << 5);  // Enable USART6 clock (bit 5 in APB2ENR)
    
    USART6->BRR = 0x0683;         // Baud rate 115200 for 16 MHz clock 
    
    USART6->CR1 |= (1 << 3);    // Enable transmitter (TE)
    USART6->CR1 |= (1 << 13);   // Enable USART6 (UE)
    
    //NVIC_EnableIRQ(USART6_IRQn);  // Enable USART6 global interrupt
}
*/
// Function to send a character via USART6
void SendChar_usart6(char C)
{
    while ((USART6->SR & 0x80) == 0) {} // Wait until the Transmit Data Register (TDR) is empty
    USART6->DR = C;                     // Load the character into the Data Register for transmission
}

// Function to send a string via USART6
void SendTxt_usart6(char *ADR)
{
    while (*ADR)
    {
        SendChar_usart6(*ADR);           // Send each character
        ADR++;                           // Move to the next character
    }
}

void USART3_Init(void)
{
    // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock(bit 1 AHB1ENR)

    // Set PB10 (TX) and PB11 (RX) to alternate function (AF)
    GPIOB->MODER &= ~( (3 << (2 * 10)) | (3 << (2 * 11)) );   // Clear mode bits for PB10 and PB11
    GPIOB->MODER |= ( (2 << (2 * 10)) | (2 << (2 * 11)) );    // Set PB10 and PB11 to alternate function (AF)

    // Set AF7 (USART3) for PB10 and PB11
    GPIOB->AFR[1] &= ~( (0xF << (4 * (10 - 8))) | (0xF << (4 * (11 - 8))) );  // Clear AF bits
    GPIOB->AFR[1] |= ( (7 << (4 * (10 - 8))) | (7 << (4 * (11 - 8))) );       // Set AF7 (USART3) for PB10 and PB11
    //AF7 is the alternate function assigned to USART3_TX and USART3_RX on PB10 (transmit) and PB11 (receive), respectively.
		
	  //Config USART3		
		RCC->APB1ENR |= 1<<18; // Enable USART3 clock
		//USART3->BRR=834; 	// for 8 MHZ and 9600 Baudrate 
		USART3->BRR=0x008B; 	// for 16 MHZ and 9600 Baudrate 
		USART3->CR1 |= 1<<3; 	// Enable TE
	  USART3->CR1 |= 1<<2; 	// Enable RE
		USART3->CR1 |= 1<<5; 	// Enable RXNEIE
		USART3->CR1 |= 1<<4; 	// Enable IDLEIE
	  USART3->CR1 |= 1<<13; // Enable USART3
	  NVIC_EnableIRQ(USART3_IRQn);    // Enable USART3 global interrupt
		
}
// Function to send a character via USART3
void SendChar_usart3(char C)
{
	while((USART3->SR & 0x80) == 0){}// Check if the Transmit Data Register (TDR) is empty
  USART3->DR=C;  // Load the character into the Data Register (DR) for transmission
   
}


// Function to send a string via USART3
void SendTxt_usart3(char *ADR)
{
		while(*ADR)
		{
		SendChar_usart3(*ADR);
		ADR++;
		}
}


void USART2_Init(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock (bit 0 in AHB1ENR)

    // Set PA2 (TX) and PA3 (RX) to alternate function (AF)
    GPIOA->MODER &= ~( (3 << (2 * 2)) | (3 << (2 * 3)) );   // Clear mode bits for PA2 and PA3
    GPIOA->MODER |= ( (2 << (2 * 2)) | (2 << (2 * 3)) );    // Set PA2 and PA3 to alternate function (AF)

    // Set AF7 (USART2) for PA2 and PA3
    GPIOA->AFR[0] &= ~( (0xF << (4 * 2)) | (0xF << (4 * 3)) );  // Clear AF bits
    GPIOA->AFR[0] |= ( (7 << (4 * 2)) | (7 << (4 * 3)) );       // Set AF7 (USART2) for PA2 and PA3

    // Configure USART2
    RCC->APB1ENR |= (1 << 17);  // Enable USART2 clock (bit 17 in APB1ENR)
    
    USART2->BRR = 0x008B;         // Baud rate 9600 for 16 MHz clock (BRR = 16000000 / 9600)
    
    USART2->CR1 |= (1 << 3);    // Enable transmitter (TE)
    USART2->CR1 |= (1 << 2);    // Enable receiver (RE)
    USART2->CR1 |= (1 << 5);    // Enable RXNE interrupt (RXNEIE)
    USART2->CR1 |= (1 << 4);    // Enable IDLE interrupt (IDLEIE)
    USART2->CR1 |= (1 << 13);   // Enable USART2 (UE)
    
    NVIC_EnableIRQ(USART2_IRQn);  // Enable USART2 global interrupt
}

// Function to send a character via USART2
void SendChar_usart2(char C)
{
	while ((USART2->SR & 0x80) == 0) {} // Wait until the Transmit Data Register (TDR) is empty
	USART2->DR = C;                     // Load the character into the Data Register for transmission
}

// Function to send a string via USART2
void SendTxt_usart2(char *ADR)
{
	while (*ADR)
	{
		SendChar_usart2(*ADR);           // Send each character
		ADR++;                           // Move to the next character
	}
}

void USART2_IRQHandler()
{
    // Check if RXNE (Receive Not Empty) flag is set
    if ((USART2->SR & 0x20) != 0)  
    {
        if (j < TX_RX_BUFFER_SIZE - 1) 
        {
				j++;  
				RX2_Buffer[j-1] = USART2->DR;  
				}
    }
    
    // Check if IDLE flag is set (indicates end of transmission)
    if ((USART2->SR & 0x10) != 0)  
    {
        SendTxt_usart3(RX2_Buffer); 
				memcpy(CpyBuffer, RX2_Buffer, TX_RX_BUFFER_SIZE);
				Parse_ESP_Data(CpyBuffer);
				if (strstr(CpyBuffer, "Host: 192.168.4.1") != NULL)
				{
				browrser_request = true;
				}
				ClearBuffer(RX2_Buffer, sizeof(RX2_Buffer));
        j = 0;                       
        USART2->SR;                  
        USART2->DR;                 
    }
}

void USART6_Init(void) {
    // 1. Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;       // Enable GPIOC clock (for PC6, PC7)
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;      // Enable USART6 clock

    // 2. Configure PC6 (TX) and PC7 (RX) as Alternate Function
    GPIOC->MODER |= (0x02 << 12); // PC6 = AF
    GPIOC->MODER |= (0x02 << 14); // PC7 = AF
    GPIOC->AFR[0] |= (8 << 24);   // PC6 = AF8
    GPIOC->AFR[0] |= (8 << 28);   // PC7 = AF8

    // 3. Set baud rate (assuming 16 MHz clock)
    USART6->BRR = 16000000 / 9600;

    // 4. Enable TX, RX, USART, RXNE interrupt, and IDLE interrupt
    USART6->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_IDLEIE;

    // 5. Enable USART6 interrupt in NVIC
    NVIC_EnableIRQ(USART6_IRQn);
}

void USART6_IRQHandler(void) {
    // Check if data is received
    if (USART6->SR & USART_SR_RXNE) {
        if (i < sizeof(RX6_BUFFER) - 1) {   // Check buffer bounds
            RX6_BUFFER[i++] = (char)USART6->DR; // Store the character in the buffer
    }
	 }
    // Check for IDLE line detection (end of message)
    if (USART6->SR & USART_SR_IDLE) {
        (void)USART6->DR;                     // Clear the IDLE flag by reading DR
				SendTxt_usart3(RX6_BUFFER); 
				memcpy(CpyBuffer2, RX6_BUFFER, GSM_BUFFER_SIZE);
        ClearBuffer(RX6_BUFFER, sizeof(RX6_BUFFER));
        i = 0; 
    }

}
