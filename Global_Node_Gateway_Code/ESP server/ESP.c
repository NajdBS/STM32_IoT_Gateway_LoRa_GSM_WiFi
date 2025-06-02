#include "ESP.h"


// Function to check if the ESP response matches the expected response
void waitForResponse(char* expectedResponse) {
    while (1) {
        if (strstr(CpyBuffer, expectedResponse) != NULL) {
            break;  // Response matches, exit the loop
        }
        delay_ms(100);
    }
    //IWDG_Refresh(); 
		//ClearBuffer(RX2_Buffer, sizeof(RX2_Buffer));
    //j = 0;
}

// Function to send a command to the ESP and wait for the expected response
void ESP_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout) {
    SendTxt_usart2(cmd);   // Send the command to the ESP
    waitForResponse(expectedResponse);  // Wait for the expected response
    delay_ms(timeout);  // Wait for the specified timeout
		IWDG_Refresh();  // Refresh watchdog timer to prevent reset
}

void ESP_Init_Station_Server(void) {
    
		ESP_SendCommand("AT\r\n", "\r\nOK\r\n", 1000);
    ESP_SendCommand("AT+RST\r\n", "\r\nOK\r\n", 5000);
    ESP_SendCommand("AT+CWMODE=2\r\n", "\r\nOK\r\n", 1000);

    sprintf(cmdBuffer, "AT+CWSAP=\"%s\",\"%s\",5,3\r\n", ssid, password);
    ESP_SendCommand(cmdBuffer, "\r\nOK\r\n", 3000);
		ClearBuffer(cmdBuffer, sizeof(cmdBuffer));
    
		ESP_SendCommand("AT+CIFSR\r\n", "\r\nOK\r\n", 2000);
    ESP_SendCommand("AT+CIPMUX=1\r\n", "\r\nOK\r\n", 1000);

    sprintf(cmdBuffer, "AT+CIPSERVER=1,%s\r\n", port);
    ESP_SendCommand(cmdBuffer, "\r\nOK\r\n", 2000);
		ClearBuffer(cmdBuffer, sizeof(cmdBuffer));
		//ESP_Configured = 1;		
}

/*
// Function to open a TCP connection with the server
void ESP_OpenTCPConnection(const char *serverIP, uint16_t serverPort) {
    
    // Open the TCP connection to the server
    sprintf(cmdBuffer, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", serverIP, serverPort);
    ESP_SendCommand(cmdBuffer, "CONNECT\r\n", 1000);  // Wait for "CONNECT" response
    ClearBuffer(cmdBuffer, sizeof(cmdBuffer));
}

// Function to close the TCP connection with the server
void ESP_CloseTCPConnection(void) {
    // Close the TCP connection
    ESP_SendCommand("AT+CIPCLOSE\r\n", "\r\nOK\r\n", 1000);  // Wait for "CLOSE" response
}

// Function to send data to the server via TCP
void ESP_SendDataToServer(const char *serverIP, uint16_t serverPort) {
	
    // temperature = DS1621_Read_Temp();  // Uncomment if temperature reading is needed
    sprintf(DataBuffer, "CH1=%d CH2=%d CH3=%d Temp_DS1621=%.2f C\n", ADC_Value[0], ADC_Value[1], ADC_Value[2], temperature);
    int dataLength = strlen(DataBuffer);  

    // Step 1: Open TCP connection
    ESP_OpenTCPConnection(serverIP, serverPort);

    // Step 2: Send the data
    sprintf(cmdBuffer, "AT+CIPSEND=%d\r\n", dataLength);  // Prepare command with data length
    ESP_SendCommand(cmdBuffer, ">", 1000);  // Wait for ">" prompt to send data
    SendTxt_usart2(DataBuffer);  // Send the actual data string
    waitForResponse("\r\nSEND OK\r\n");  // Wait for confirmation "SEND OK"

    // Step 3: Close the TCP connection
    ESP_CloseTCPConnection();
}*/
void Parse_ESP_Data(const char* buffer) //extract data
{
	 if ((!strstr(buffer, "+IPD"))&&(!strstr(buffer, "CH1")))
        return;
    const char* ch1_str = strstr(buffer, "CH1=");
    const char* ch2_str = strstr(buffer, "CH2=");
    const char* ch3_str = strstr(buffer, "CH3=");
    const char* temp_str = strstr(buffer, "Temp_DS1621=");

    if (ch1_str && ch2_str && ch3_str && temp_str)
    {
        sscanf(ch1_str, "CH1=%hu", &Esp_ADC_Value[0]);
        sscanf(ch2_str, "CH2=%hu", &Esp_ADC_Value[1]);
        sscanf(ch3_str, "CH3=%hu", &Esp_ADC_Value[2]);
        sscanf(temp_str, "Temp_DS1621=%f", &Esp_temperature);
				flag_esp=1;
    }
		//delay_ms(1000);
		
}




