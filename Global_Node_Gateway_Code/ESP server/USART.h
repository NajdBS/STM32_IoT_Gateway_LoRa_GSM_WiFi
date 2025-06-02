#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"   // Header for STM32F4
#include "UTIL.h"
#include "string.h"  
#include "ESP.h" 
#include "WEBSERVER.h"
#include <stdbool.h>

#define TX_RX_BUFFER_SIZE 500  // Buffer size
#define CPY_BUFFER_SIZE 500 
#define GSM_BUFFER_SIZE 500

extern char CpyBuffer[CPY_BUFFER_SIZE];
extern char CpyBuffer2[CPY_BUFFER_SIZE];
extern char RX2_Buffer[TX_RX_BUFFER_SIZE]; 
extern char RX6_BUFFER[GSM_BUFFER_SIZE];//gsm reception here
extern int j;
extern int i;
extern bool browrser_request;


// Initialization function for USART3
void USART3_Init(void);

// Function to send a single character via USART3
void SendChar_usart3(char C);

// Function to send a string via USART3
void SendTxt_usart3(char *ADR);

// Initialization function for USART2
void USART2_Init(void);

// Function to send a single character via USART2
void SendChar_usart2(char C);

// Function to send a string via USART2
void SendTxt_usart2(char *ADR);

// USART2 interrupt handler declaration
void USART2_IRQHandler(void);

// Initialization function for USART6
void USART6_Init(void);

// Function to send a character via USART6
void SendChar_usart6(char C);

// Function to send a string via USART6
void SendTxt_usart6(char *ADR);

// USART6 interrupt handler declaration
void USART6_IRQHandler(void);

// Initialization function for USART1
void USART1_Init(void);

// Function to send a character via USART1
void SendChar_usart1(char C);

// Function to send a string via USART1
void SendTxt_usart1(char *ADR);

#endif // USART_H
