#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include "stm32f4xx.h"  
#include <stdio.h>       
#include "string.h"
#include "USART.h"
#include "UTIL.h"
#include "ADC.h"
#include "I2C.h"

#define Network "MonServeur"
#define Password "12345678"
/*#define IP_ADDRESS "192.168.8.101" //(ESP1)
#define IP_ADDRESS2 "192.168.0.112" //(ESP2)

//#define PORT "80"

#define HTML_PAGE "<html>\r\n" \
                  "<body>\r\n" \
                  "<center>\r\n" \
                  "<h1>WiFi LED on off demo: 1</h1><br>\r\n" \
                  "Click to turn <a href=\"ledOn\">LED ON</a><br>\r\n" \
                  "Click to turn <a href=\"ledOff\">LED OFF</a><br>\r\n" \
                  "<hr>\r\n" \
                  "<a href=\"https://circuits4you.com\">circuits4you.com</a>\r\n" \
                  "</center>\r\n" \
                  "</body>\r\n" \
                  "</html>\r\n"
*/
#define CMD_BUFFER_SIZE 500
#define DATA_BUFFER_SIZE 500
//#define serverIP "192.168.8.101"
//#define serverPort "80"

extern uint16_t ADC_Value[BUFFER_SIZE];
extern float temperature;
extern char cmdBuffer[CMD_BUFFER_SIZE]; 
extern char DataBuffer[DATA_BUFFER_SIZE];



void ESP_Init_Station_Client(void);

void ESP_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout);

void waitForResponse(char* expectedResponse);

void ESP_OpenTCPConnection(const char *serverIP, uint16_t serverPort);

void ESP_CloseTCPConnection(void);

void ESP_SendDataToServer(const char *serverIP, uint16_t serverPort);

#endif // ESP_UTILS_H
