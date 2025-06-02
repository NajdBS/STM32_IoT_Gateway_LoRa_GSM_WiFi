#include "stm32f4xx.h"
#include "ESP.h"



// Buffers
char cmdBuffer[CMD_BUFFER_SIZE];
char RX2_Buffer[TX_RX_BUFFER_SIZE];
char DataBuffer[DATA_BUFFER_SIZE];
char CpyBuffer[DATA_BUFFER_SIZE];

int j = 0;
//Var
bool flag = false;
float temperature;
uint16_t ADC_Value[BUFFER_SIZE];



int main(void)
{
    USART2_Init();
    USART3_Init();
    IWDG_Init();
    ESP_Init_Station_Client();
		config_GPIO_ADC_Channnel();
		config_ADC1();  
		config_TIMER3();
		config_DMA2();
		I2C1_Init();
	  DS1621_Init();
    while (1) 
    {
    if (flag == true){
		ESP_SendDataToServer("192.168.4.1", 80);  // Send sensor data to the specified server IP and port
		flag = false;
		}
		// Refresh watchdog periodically
    IWDG_Refresh();
		
		SendTxt_usart3("Entering Sleep Mode...\r\n");
	 
		// Enter mode Sleep
    enter_sleep_mode();
	
	  SendTxt_usart3("Woke up from Sleep Mode.\r\n");
    }
}




