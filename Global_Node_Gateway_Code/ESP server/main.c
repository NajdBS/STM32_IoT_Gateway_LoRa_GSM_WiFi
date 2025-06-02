#include "stm32f4xx.h"
#include "SPI.h"
#include "SX1278.h"
#include "ESP.h"
#include "GSM.h"


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

// Buffers
char buffer[64];//Lora data reception here
char cmdBuffer[CMD_BUFFER_SIZE];
char RX2_Buffer[TX_RX_BUFFER_SIZE];//esp reception here
char CpyBuffer[DATA_BUFFER_SIZE];//buffer for test esp reception 
char RX6_BUFFER[GSM_BUFFER_SIZE];//gsm reception here
char CpyBuffer2[CPY_BUFFER_SIZE];//buffer for test gsm reception
//char DataBuffer[DATA_BUFFER_SIZE];

//Var
int i,j = 0;
uint8_t ret;
uint8_t ESP_Configured = 0; 
uint8_t linkID;
uint8_t pagenumb=0;

bool flag_esp= false;
bool flag_lora= false;
bool browrser_request = false;
bool lora_active = false;
bool esp_active = false;

float Esp_temperature;
uint16_t Esp_ADC_Value[3];
float Lora_temperature;
uint16_t Lora_ADC_Value[3];


int main(void)
{
		USART1_Init();//lora terminal
    USART2_Init();//ESP
    USART3_Init();//esp terminal + GSM  terminal
		USART6_Init();//GSM 
		NVIC_SetPriority(USART2_IRQn, 0);  
		NVIC_SetPriority(USART6_IRQn, 1); 
		IWDG_Init();
		ESP_Init_Station_Server();
		GPIO_Init();
		SPI1_Init();//LORA
		LoRa_Receive_Mode();
		//apn/ooredoo:internet.ooredoo.tn
		//apn/telecom:internet.tn
		GSM_Init("internet.ooredoo.tn");
    while (1) 
    {
		if(lora_active == true){
		ret = SX1278_LoRaRxPacket1();  // Attempt to receive a LoRa packet, store the number of bytes received in 'ret'
		}
		if (ret > 0)
			{
			// Read the received data from the LoRa module into the buffer
			SX1278_read1((uint8_t *)buffer, ret);
			buffer[ret] = '\0';  // Add null-terminator 
			Parse_LoRa_Data(buffer);  // Parse the received LoRa data
			SendTxt_usart1(buffer);   // Send the received message through USART6 (monitoring purpose)
			//SendTxt_usart1(msg10);    // Send "Package received ...\r\n"
			//delay_ms(4000);
			}
		if ((flag_esp == true)&&(esp_active == true))
			{
			GSM_SendData(Esp_ADC_Value,Esp_temperature);//SEND GSM
			flag_esp = false;
			}
		if ((flag_lora == true)&&(lora_active == true))
			{
			GSM_SendData(Lora_ADC_Value,Lora_temperature);//SEND GSM
			flag_lora = false;
			delay_ms(15000);
			}	
		if (browrser_request == true )
			{
			processRequest(CpyBuffer);
			browrser_request = false;	
			}
		// Refresh watchdog periodically
    IWDG_Refresh();
    }
}




