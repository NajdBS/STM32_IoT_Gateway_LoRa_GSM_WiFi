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
}

// Function to send a command to the ESP and wait for the expected response
void ESP_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout) {
    SendTxt_usart2(cmd);   // Send the command to the ESP
    waitForResponse(expectedResponse);  // Wait for the expected response
    delay_ms(timeout);  // Wait for the specified timeout
		IWDG_Refresh();  // Refresh watchdog timer to prevent reset
}

// Initialize the ESP module and perform basic setup
void ESP_Init_Station_Client(void) {
    // Send commands and wait for expected responses
    ESP_SendCommand("AT\r\n", "\r\nOK\r\n", 1000);  // Check communication with ESP
    ESP_SendCommand("AT+RST\r\n", "\r\nOK\r\n", 5000);  // Reset ESP
    ESP_SendCommand("AT+CWMODE=1\r\n", "\r\nOK\r\n", 1000);  // Set ESP to Station (Client) mode
    
    sprintf(cmdBuffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", Network, Password);
    ESP_SendCommand(cmdBuffer, "\r\nOK\r\n", 10000);  // Connect to the Wi-Fi network
    
    ESP_SendCommand("AT+CIFSR\r\n", "\r\nOK\r\n", 2000);  // Get the IP address
}

// Function to open a TCP connection with the server
void ESP_OpenTCPConnection(const char *serverIP, uint16_t serverPort) {
    
    // Open the TCP connection to the server
    sprintf(cmdBuffer, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", serverIP, serverPort);
    ESP_SendCommand(cmdBuffer, "\r\nOK\r\n", 1000);  // Wait for "CONNECT" response
    ClearBuffer(cmdBuffer, sizeof(cmdBuffer));
}

// Function to close the TCP connection with the server
void ESP_CloseTCPConnection(void) {
    // Close the TCP connection
    ESP_SendCommand("AT+CIPCLOSE\r\n", "\r\nOK\r\n", 1000);  // Wait for "CLOSE" response
}

// Function to send data to the server via TCP
void ESP_SendDataToServer(const char *serverIP, uint16_t serverPort) {
	
    temperature = DS1621_Read_Temp();  // Uncomment if temperature reading is needed
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
}

