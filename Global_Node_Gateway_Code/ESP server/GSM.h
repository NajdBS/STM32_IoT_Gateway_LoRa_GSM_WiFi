#ifndef GSM_H
#define GSM_H
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>
#include "UTIL.h"
#include "USART.h"

#define KEY "DC5X07T7HHEM499F"

extern char RX6_BUFFER[GSM_BUFFER_SIZE];//gsm reception here



void waitForResponse_GSM(char* expectedResponse);
void GSM_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout);
void GSM_Init(const char* apn);
void GSM_SendData(uint16_t data[3],float temp);


#endif // GSM_H
