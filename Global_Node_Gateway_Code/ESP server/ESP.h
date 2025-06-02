#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include "stm32f4xx.h"  
#include <stdio.h>       
#include "string.h"
#include "USART.h"
#include "UTIL.h"
#include <stdbool.h>

#define ssid "MonServeur"
#define password "12345678"
#define port "80"

/*#define IP_ADDRESS "192.168.8.101" //(ESP1)
#define IP_ADDRESS2 "192.168.0.112" //(ESP2)*/


#define CMD_BUFFER_SIZE 500
#define DATA_BUFFER_SIZE 500

//#define serverIP "192.168.8.101"
//#define serverPort "80"
//extern uint8_t ESP_Configured; 

extern uint16_t Esp_ADC_Value[3];
extern float Esp_temperature;
extern char cmdBuffer[CMD_BUFFER_SIZE]; 
//extern char DataBuffer[DATA_BUFFER_SIZE];
extern bool flag_esp;


void ESP_Init_Station_Server(void);

void ESP_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout);

void waitForResponse(char* expectedResponse);

void Parse_ESP_Data(const char* buffer);

/*void ESP_OpenTCPConnection(const char *serverIP, uint16_t serverPort);

void ESP_CloseTCPConnection(void);

void ESP_SendDataToServer(const char *serverIP, uint16_t serverPort);*/

#endif // ESP_UTILS_H
