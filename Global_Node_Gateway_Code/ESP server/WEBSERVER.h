#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdint.h>
#include <stdlib.h>
#include "ESP.h"
#include <stdbool.h>

#define MAX_SEND_SIZE 1024  

extern uint16_t Esp_ADC_Value[3];
extern float Esp_temperature;
extern float Lora_temperature;
extern uint16_t Lora_ADC_Value[3];

extern uint8_t linkID;
extern bool lora_active;
extern bool esp_active;
extern uint8_t pagenumb;

// ==== Main functions ====
void sendHTML(const char* htmlContent);     // Sends an HTML page using AT+CIPSEND
int extractLinkID(const char* buffer);      // Extracts the connection ID from incoming data
void processRequest(char* request);         // Parses HTTP requests and dispatches pages
void all_data_res(void);


#endif // WEBSERVER_H
