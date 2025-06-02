#include "GSM.h"

/*//Notes
// Usart 2 baatht bih lil GSM ken inty msitaamlou juste badel usart6 w ken andekch 9oli andi hadher il 6 
//hajet lezim tit1aked minha : type de data ili bech idakhalha int wala uint8_t wala ghirou fil GSM_SendData(int data[4])
// fil fonction GSM_SendData(int data[4]) ena dakhaltlou buffer fih les 4 valeur ili ebch youslou min and il esp donc inty suppose kif ti9blhom 
//mil esp thothom fi buffer w baad idakhlou lil fonction wala ken inty fara9thom kol wehed fi variable simple badel il parametres mtaa fct wkhw 
// w kif tji tistaamlou t3ayet lil 2 fonction hedhom : 
//GSM_Init("internet.tn");
//GSM_SendData(sensorData); il sensordata heka il buffer mtaa les valeur ili hkitlik alih (mouch lezim naamlou temperature bech nkhaliwh kolou int wkhw
//nahi il delay wil wait for resp maw deja nadek fi fichier akher ena lina bech nikhdim bihom wkhw 

//extern void SendTxt_usart6(char *ADR);*/




void waitForResponse_GSM(char* expectedResponse) {
    while (1) {
        if (strstr(CpyBuffer2, expectedResponse) != NULL) {
            break;
        }
        delay_ms(100);
    }		
}

void GSM_SendCommand(char *cmd, char *expectedResponse, uint32_t timeout) {
		SendTxt_usart6(cmd);
    //waitForResponse_GSM(expectedResponse);
    delay_ms(timeout);
		IWDG_Refresh();  // Refresh watchdog timer to prevent reset
}

void GSM_Init(const char* apn) {
    char apnCommand[100];
    
    GSM_SendCommand("AT\r\n", "OK", 500);
    GSM_SendCommand("AT+CIPSHUT\r\n", "SHUT OK", 500);
    GSM_SendCommand("AT+CGATT=1\r\n", "OK", 500);

    // Insert the APN into the AT+CSTT command
    sprintf(apnCommand, "AT+CSTT=\"%s\",\"/\",\"/\"\r\n", apn);
    GSM_SendCommand(apnCommand, "OK", 500);

    GSM_SendCommand("AT+CIICR\r\n", "OK", 2000);
    GSM_SendCommand("AT+CIFSR\r\n", ".", 2000);
}

void GSM_SendData(uint16_t data[3],float temp) {
    char buffer1[100];
    char buffer2[200];
		if (data == Esp_ADC_Value && temp == Esp_temperature) {
        //ESP : field1 -> field4
        sprintf(buffer2,"GET /update?api_key=%s&field1=%hu&field2=%hu&field3=%d&field4=%.2f\r\n",KEY, data[0], data[1], data[2], temp);
		}
    if (data == Lora_ADC_Value && temp == Lora_temperature) {
        //LoRa : field5 -> field8
        sprintf(buffer2,"GET /update?api_key=%s&field5=%hu&field6=%hu&field7=%d&field8=%.2f\r\n",KEY, data[0], data[1], data[2], temp);
    } 
 
    GSM_SendCommand("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n", "\r\nOK\r\n", 2000);
		sprintf(buffer1, "AT+CIPSEND=%d\r\n", strlen(buffer2));
		GSM_SendCommand(buffer1, ">", 1000);
    GSM_SendCommand(buffer2, "CLOSED\r\n", 3000);
    //delay_ms(1000);
}
