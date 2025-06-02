#include "WEBSERVER.h"


const char Login_Page[] =
"<!doctype html>\n"
"<html>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<title>Login</title>\n"
"<style>body{font-family:Arial,sans-serif;background:linear-gradient(135deg,#667eea,#764ba2);margin:0;padding:0}.login-container{width:320px;padding:40px;background:#fff;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,.3);position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)}.login-container h2{text-align:center;margin-bottom:20px;color:#333}.login-container input[type=password],.login-container input[type=text]{width:100%;padding:12px 20px;margin:8px 0;border:1px solid #ccc;border-radius:4px;box-sizing:border-box}.login-container input[type=submit]{width:100%;background-color:#4caf50;color:#fff;padding:14px 20px;margin-top:12px;border:none;border-radius:4px;cursor:pointer;font-size:16px}.login-container input[type=submit]:hover{background-color:#45a049}.btn:active{background-color:#3e8e41}</style>\n"
"</head>\n"
"<body>\n"
"<div class=login-container>\n"
"<h2>Login</h2>\n"
"<form action=/login method=GET>\n"
"User: <input type=text name=user placeholder=\"Enter Username\" required><br>\n"
"Pass: <input type=password name=pass placeholder=\"Enter Password\" required><br>\n"
"<input type=submit value=Login>\n"
"</form>\n"
"</div>\n"
"</body>\n"
"</html>";

const char main_page[] ="<!doctype html>\n"
"<html>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<title>Home Page</title>\n"
"<style>body{font-family:Arial,sans-serif;background:linear-gradient(135deg,#667eea,#764ba2);margin:0;padding:0;height:100vh;display:flex;justify-content:center;align-items:center}.container{width:100%;max-width:800px;padding:40px;text-align:center;background-color:#fff;border-radius:10px;box-shadow:0 0 15px rgba(0,0,0,.1)}.welcome-message{font-size:36px;margin-bottom:30px;color:#333}.button-container{display:flex;justify-content:center;gap:20px;margin-top:30px}.btn{padding:15px 30px;font-size:18px;color:#fff;background-color:#4caf50;border:none;border-radius:5px;cursor:pointer;transition:background-color .3s ease}.btn:hover{background-color:#45a049}.btn:active{background-color:#3e8e41}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<div class=welcome-message>Welcome to your IoT Server</div>\n"
"<p>Choose an option below to either view the data or configure your server.</p>\n"
"<div class=button-container>\n"
"<a href=/dataCenter><button class=btn>Data Center</button></a>\n"
"<a href=/configServer><button class=btn>Configure Server</button></a>\n"
"</div>\n"
"</div>\n"
"</body>\n"
"</html>";

const char Data_Center_page[]="<!doctype html>\n"
"<html lang=en>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<meta name=viewport content=\"width=device-width,initial-scale=1\">\n"
"<title>Data Center</title>\n"
"<link rel=icon href=\"data:;base64,iVBORw0KGgo=\">\n"
"<style>body{background-color:#f4f7f9;font-family:Arial,sans-serif;margin:0;padding:0}.container{display:flex;flex-wrap:wrap;justify-content:space-around;align-items:flex-start;margin:50px}.card{background-color:#fff;border-radius:10px;box-shadow:0 4px 8px rgba(0,0,0,.1);padding:30px;width:300px;margin:10px}h2{color:#333;margin-bottom:20px;text-align:center}.data-item{font-size:18px;margin:10px 0;color:#555}.button-container{text-align:center;margin:30px 0}.back-button{padding:12px 24px;font-size:16px;border:none;border-radius:8px;cursor:pointer;color:#fff;background-color:#007bff;text-decoration:none}@media (max-width:600px){.container{flex-direction:column;align-items:center}}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<div class=card>\n"
"<h2>ESP Data</h2>\n"
"<div class=data-item>Temperature: <span id=esp_temp>0</span> °C</div>\n"
"<div class=data-item>CH1: <span id=esp_ch1>0</span></div>\n"
"<div class=data-item>CH2: <span id=esp_ch2>0</span></div>\n"
"<div class=data-item>CH3: <span id=esp_ch3>0</span></div>\n"
"</div>\n"
"<div class=card>\n"
"<h2>LoRa Data</h2>\n"
"<div class=data-item>Temperature: <span id=lora_temp>0</span> °C</div>\n"
"<div class=data-item>CH1: <span id=lora_ch1>0</span></div>\n"
"<div class=data-item>CH2: <span id=lora_ch2>0</span></div>\n"
"<div class=data-item>CH3: <span id=lora_ch3>0</span></div>\n"
"</div>\n"
"</div>\n"
"<div class=button-container>\n"
"<button class=back-button onclick=goBack()>Back</button>\n"
"</div>\n"
"<script>function updateAll(){fetch(\"/all_data\").then((e=>e.text())).then((e=>{const t=e.split(\"\\n\"),n={};t.forEach((e=>{const[t,l]=e.split(\":\");t&&l&&(n[t.trim()]=l.trim().replace(\"Z\",\".\"))})),document.getElementById(\"esp_temp\").innerText=n.ESP_TEMP||\"0\",document.getElementById(\"esp_ch1\").innerText=n.ESP_CH1||\"0\",document.getElementById(\"esp_ch2\").innerText=n.ESP_CH2||\"0\",document.getElementById(\"esp_ch3\").innerText=n.ESP_CH3||\"0\",document.getElementById(\"lora_temp\").innerText=n.LORA_TEMP||\"0\",document.getElementById(\"lora_ch1\").innerText=n.LORA_CH1||\"0\",document.getElementById(\"lora_ch2\").innerText=n.LORA_CH2||\"0\",document.getElementById(\"lora_ch3\").innerText=n.LORA_CH3||\"0\"})).catch((e=>console.error(\"Error fetching all data:\",e)))}function goBack(){window.location.href=\"/login?user=admin&pass=admin\"}setInterval(updateAll,1e4),window.onload=updateAll</script>\n"
"</body>\n"
"</html>"
"";


/*
const char server_config_page[]="<!DOCTYPE html>\n"
"<html lang=\"en\">\n"
"<head>\n"
" <meta charset=\"UTF-8\">\n"
" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
" <title>Select Gateway</title>\n"
" <style>\n"
" body {\n"
" font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
" background: #f5f5f5;\n"
" display: flex;\n"
" justify-content: center;\n"
" align-items: center;\n"
" height: 100vh;\n"
" margin: 0;\n"
" }\n"
" .container {\n"
" background: #fff;\n"
" padding: 30px;\n"
" border-radius: 15px;\n"
" box-shadow: 0 4px 8px rgba(0,0,0,0.1);\n"
" max-width: 400px;\n"
" width: 100%;\n"
" text-align: center;\n"
" }\n"
" h1 {\n"
" font-size: 1.8rem;\n"
" margin-bottom: 20px;\n"
" color: #333;\n"
" }\n"
" .checkbox-group {\n"
" text-align: left;\n"
" margin: 20px 0;\n"
" }\n"
" label {\n"
" display: flex;\n"
" align-items: center;\n"
" margin-bottom: 15px;\n"
" cursor: pointer;\n"
" font-size: 1.1rem;\n"
" color: #555;\n"
" }\n"
" input[type=\"checkbox\"] {\n"
" margin-right: 10px;\n"
" transform: scale(1.2);\n"
" accent-color: #007BFF;\n"
" }\n"
" .btn {\n"
" display: inline-block;\n"
" padding: 10px 20px;\n"
" font-size: 1rem;\n"
" color: #fff;\n"
" background: #007BFF;\n"
" border: none;\n"
" border-radius: 25px;\n"
" cursor: pointer;\n"
" transition: background 0.3s ease;\n"
" text-decoration: none;\n"
" margin: 10px;\n"
" }\n"
" .btn:hover {\n"
" background: #0056b3;\n"
" }\n"
" .btn-secondary {\n"
" background: #6c757d;\n"
" }\n"
" .btn-secondary:hover {\n"
" background: #5a6268;\n"
" }\n"
" </style>\n"
"</head>\n"
"<body>\n"
"\n"
" <div class=\"container\">\n"
" <h1>Select the Gateway</h1>\n"
" <form id=\"gatewayForm\" action=\"/configServer\" method=\"get\">\n"
" <div class=\"checkbox-group\">\n"
" <label>\n"
" <input type=\"checkbox\" name=\"gateway_lora\" value=\"on\" id=\"gateway_lora\">\n"
" Use LoRa Gateway\n"
" </label>\n"
" <label>\n"
" <input type=\"checkbox\" name=\"gateway_espwifi\" value=\"on\" id=\"gateway_espwifi\">\n"
" Use ESP WiFi Gateway\n"
" </label>\n"
" </div>\n"
" <button type=\"submit\" class=\"btn\">Confirm</button>\n"
" <a href=\"http://192.168.4.1/login?user=admin&pass=admin\" class=\"btn btn-secondary\">Back</a>\n"
" </form>\n"
" </div>\n"
"\n"
" <script>\n"
" // Lors de la soumission du formulaire : ajouter les champs cachés si non cochés\n"
" document.getElementById('gatewayForm').addEventListener('submit', function(e) {\n"
"   const loraCheckbox = document.querySelector('input[name=\"gateway_lora\"]');\n"
"   if (!loraCheckbox.checked) {\n"
"     const hiddenLora = document.createElement('input');\n"
"     hiddenLora.type = 'hidden';\n"
"     hiddenLora.name = 'gateway_lora';\n"
"     hiddenLora.value = 'off';\n"
"     this.appendChild(hiddenLora);\n"
"   }\n"
"\n"
"   const espCheckbox = document.querySelector('input[name=\"gateway_espwifi\"]');\n"
"   if (!espCheckbox.checked) {\n"
"     const hiddenEsp = document.createElement('input');\n"
"     hiddenEsp.type = 'hidden';\n"
"     hiddenEsp.name = 'gateway_espwifi';\n"
"     hiddenEsp.value = 'off';\n"
"     this.appendChild(hiddenEsp);\n"
"   }\n"
" });\n"
"\n"
" // Charger l’état actuel au démarrage\n"
" function loadCurrentConfig() {\n"
"   fetch('/getConfig')\n"
"     .then(response => response.json())\n"
"     .then(data => {\n"
"       if (data.gateway_lora === 'on') {\n"
"         document.getElementById('gateway_lora').checked = true;\n"
"       }\n"
"       if (data.gateway_espwifi === 'on') {\n"
"         document.getElementById('gateway_espwifi').checked = true;\n"
"       }\n"
"     })\n"
"     .catch(() => {\n"
"       console.log('Failed to load config.');\n"
"     });\n"
" }\n"
"\n"
" window.onload = loadCurrentConfig;\n"
" </script>\n"
"\n"
"</body>\n"
"</html>\n"
"";*/


const char server_config_page[]="<!doctype html>\n"
"<html lang=en>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<meta name=viewport content=\"width=device-width,initial-scale=1\">\n"
"<title>Select Gateway</title>\n"
"<style>body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:#f5f5f5;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}.container{background:#fff;padding:30px;border-radius:15px;box-shadow:0 4px 8px rgba(0,0,0,.1);max-width:400px;width:100%;text-align:center}h1{font-size:1.8rem;margin-bottom:20px;color:#333}.checkbox-group{text-align:left;margin:20px 0}label{display:flex;align-items:center;margin-bottom:15px;cursor:pointer;font-size:1.1rem;color:#555}input[type=checkbox]{margin-right:10px;transform:scale(1.2);accent-color:#007BFF}.btn{display:inline-block;padding:10px 20px;font-size:1rem;color:#fff;background:#007bff;border:none;border-radius:25px;cursor:pointer;transition:background .3s ease;text-decoration:none;margin:10px}.btn:hover{background:#0056b3}.btn-secondary{background:#6c757d}.btn-secondary:hover{background:#5a6268}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<h1>Select the Gateway</h1>\n"
"<form id=gatewayForm action=/configServer>\n"
"<div class=checkbox-group>\n"
"<label>\n"
"<input type=checkbox name=gateway_lora value=on id=gateway_lora>\n"
"Activate LoRa Gateway\n"
"</label>\n"
"<label>\n"
"<input type=checkbox name=gateway_espwifi value=on id=gateway_espwifi>\n"
"Activate ESP WiFi Gateway\n"
"</label>\n"
"</div>\n"
"<button type=submit class=btn>Confirm</button>\n"
"<a href=\"http://192.168.4.1/login?user=admin&pass=admin\" class=\"btn btn-secondary\">Back</a>\n"
"</form>\n"
"</div>\n"
"<script>function loadCurrentConfig(){fetch(\"/getConfig\").then((e=>e.json())).then((e=>{\"on\"===e.gateway_lora&&(document.getElementById(\"gateway_lora\").checked=!0),\"on\"===e.gateway_espwifi&&(document.getElementById(\"gateway_espwifi\").checked=!0)})).catch((()=>{console.log(\"Failed to load config.\")}))}document.getElementById(\"gatewayForm\").addEventListener(\"submit\",(function(e){if(!document.querySelector('input[name=\"gateway_lora\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_lora\",e.value=\"off\",this.appendChild(e)}if(!document.querySelector('input[name=\"gateway_espwifi\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_espwifi\",e.value=\"off\",this.appendChild(e)}})),window.onload=loadCurrentConfig</script>\n"
"</body>\n"
"</html>"
"";

const char server_config_page_LORAon_ESPoff[]="<!doctype html>\n"
"<html lang=en>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<meta name=viewport content=\"width=device-width,initial-scale=1\">\n"
"<title>Select Gateway</title>\n"
"<style>body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:#f5f5f5;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}.container{background:#fff;padding:30px;border-radius:15px;box-shadow:0 4px 8px rgba(0,0,0,.1);max-width:400px;width:100%;text-align:center}h1{font-size:1.8rem;margin-bottom:20px;color:#333}.checkbox-group{text-align:left;margin:20px 0}label{display:flex;align-items:center;margin-bottom:15px;cursor:pointer;font-size:1.1rem;color:#555}input[type=checkbox]{margin-right:10px;transform:scale(1.2);accent-color:#007BFF}.btn{display:inline-block;padding:10px 20px;font-size:1rem;color:#fff;background:#007bff;border:none;border-radius:25px;cursor:pointer;transition:background .3s ease;text-decoration:none;margin:10px}.btn:hover{background:#0056b3}.btn-secondary{background:#6c757d}.btn-secondary:hover{background:#5a6268}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<h1>Select the Gateway</h1>\n"
"<form id=gatewayForm action=/configServer>\n"
"<div class=checkbox-group>\n"
"<label>\n"
"<input type=checkbox name=gateway_lora value=off id=gateway_lora>\n"
"Desactivate LoRa Gateway\n"
"</label>\n"
"<label>\n"
"<input type=checkbox name=gateway_espwifi value=on id=gateway_espwifi>\n"
"Activate ESP WiFi Gateway\n"
"</label>\n"
"</div>\n"
"<button type=submit class=btn>Confirm</button>\n"
"<a href=\"http://192.168.4.1/login?user=admin&pass=admin\" class=\"btn btn-secondary\">Back</a>\n"
"</form>\n"
"</div>\n"
"<script>function loadCurrentConfig(){fetch(\"/getConfig\").then((e=>e.json())).then((e=>{\"on\"===e.gateway_lora&&(document.getElementById(\"gateway_lora\").checked=!0),\"on\"===e.gateway_espwifi&&(document.getElementById(\"gateway_espwifi\").checked=!0)})).catch((()=>{console.log(\"Failed to load config.\")}))}document.getElementById(\"gatewayForm\").addEventListener(\"submit\",(function(e){if(!document.querySelector('input[name=\"gateway_lora\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_lora\",e.value=\"off\",this.appendChild(e)}if(!document.querySelector('input[name=\"gateway_espwifi\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_espwifi\",e.value=\"off\",this.appendChild(e)}})),window.onload=loadCurrentConfig</script>\n"
"</body>\n"
"</html>"
"";

const char server_config_page_LORAon_ESPon[]="<!doctype html>\n"
"<html lang=en>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<meta name=viewport content=\"width=device-width,initial-scale=1\">\n"
"<title>Select Gateway</title>\n"
"<style>body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:#f5f5f5;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}.container{background:#fff;padding:30px;border-radius:15px;box-shadow:0 4px 8px rgba(0,0,0,.1);max-width:400px;width:100%;text-align:center}h1{font-size:1.8rem;margin-bottom:20px;color:#333}.checkbox-group{text-align:left;margin:20px 0}label{display:flex;align-items:center;margin-bottom:15px;cursor:pointer;font-size:1.1rem;color:#555}input[type=checkbox]{margin-right:10px;transform:scale(1.2);accent-color:#007BFF}.btn{display:inline-block;padding:10px 20px;font-size:1rem;color:#fff;background:#007bff;border:none;border-radius:25px;cursor:pointer;transition:background .3s ease;text-decoration:none;margin:10px}.btn:hover{background:#0056b3}.btn-secondary{background:#6c757d}.btn-secondary:hover{background:#5a6268}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<h1>Select the Gateway</h1>\n"
"<form id=gatewayForm action=/configServer>\n"
"<div class=checkbox-group>\n"
"<label>\n"
"<input type=checkbox name=gateway_lora value=off id=gateway_lora>\n"
"Desactivate LoRa Gateway\n"
"</label>\n"
"<label>\n"
"<input type=checkbox name=gateway_espwifi value=off id=gateway_espwifi>\n"
"Desactivate ESP WiFi Gateway\n"
"</label>\n"
"</div>\n"
"<button type=submit class=btn>Confirm</button>\n"
"<a href=\"http://192.168.4.1/login?user=admin&pass=admin\" class=\"btn btn-secondary\">Back</a>\n"
"</form>\n"
"</div>\n"
"<script>function loadCurrentConfig(){fetch(\"/getConfig\").then((e=>e.json())).then((e=>{\"on\"===e.gateway_lora&&(document.getElementById(\"gateway_lora\").checked=!0),\"on\"===e.gateway_espwifi&&(document.getElementById(\"gateway_espwifi\").checked=!0)})).catch((()=>{console.log(\"Failed to load config.\")}))}document.getElementById(\"gatewayForm\").addEventListener(\"submit\",(function(e){if(!document.querySelector('input[name=\"gateway_lora\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_lora\",e.value=\"off\",this.appendChild(e)}if(!document.querySelector('input[name=\"gateway_espwifi\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_espwifi\",e.value=\"off\",this.appendChild(e)}})),window.onload=loadCurrentConfig</script>\n"
"</body>\n"
"</html>"
"";
const char server_config_page_LORAoff_ESPon[]="<!doctype html>\n"
"<html lang=en>\n"
"<head>\n"
"<meta charset=UTF-8>\n"
"<meta name=viewport content=\"width=device-width,initial-scale=1\">\n"
"<title>Select Gateway</title>\n"
"<style>body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:#f5f5f5;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}.container{background:#fff;padding:30px;border-radius:15px;box-shadow:0 4px 8px rgba(0,0,0,.1);max-width:400px;width:100%;text-align:center}h1{font-size:1.8rem;margin-bottom:20px;color:#333}.checkbox-group{text-align:left;margin:20px 0}label{display:flex;align-items:center;margin-bottom:15px;cursor:pointer;font-size:1.1rem;color:#555}input[type=checkbox]{margin-right:10px;transform:scale(1.2);accent-color:#007BFF}.btn{display:inline-block;padding:10px 20px;font-size:1rem;color:#fff;background:#007bff;border:none;border-radius:25px;cursor:pointer;transition:background .3s ease;text-decoration:none;margin:10px}.btn:hover{background:#0056b3}.btn-secondary{background:#6c757d}.btn-secondary:hover{background:#5a6268}</style>\n"
"</head>\n"
"<body>\n"
"<div class=container>\n"
"<h1>Select the Gateway</h1>\n"
"<form id=gatewayForm action=/configServer>\n"
"<div class=checkbox-group>\n"
"<label>\n"
"<input type=checkbox name=gateway_lora value=on id=gateway_lora>\n"
"Activate LoRa Gateway\n"
"</label>\n"
"<label>\n"
"<input type=checkbox name=gateway_espwifi value=off id=gateway_espwifi>\n"
"Desactivate ESP WiFi Gateway\n"
"</label>\n"
"</div>\n"
"<button type=submit class=btn>Confirm</button>\n"
"<a href=\"http://192.168.4.1/login?user=admin&pass=admin\" class=\"btn btn-secondary\">Back</a>\n"
"</form>\n"
"</div>\n"
"<script>function loadCurrentConfig(){fetch(\"/getConfig\").then((e=>e.json())).then((e=>{\"on\"===e.gateway_lora&&(document.getElementById(\"gateway_lora\").checked=!0),\"on\"===e.gateway_espwifi&&(document.getElementById(\"gateway_espwifi\").checked=!0)})).catch((()=>{console.log(\"Failed to load config.\")}))}document.getElementById(\"gatewayForm\").addEventListener(\"submit\",(function(e){if(!document.querySelector('input[name=\"gateway_lora\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_lora\",e.value=\"off\",this.appendChild(e)}if(!document.querySelector('input[name=\"gateway_espwifi\"]').checked){const e=document.createElement(\"input\");e.type=\"hidden\",e.name=\"gateway_espwifi\",e.value=\"off\",this.appendChild(e)}})),window.onload=loadCurrentConfig</script>\n"
"</body>\n"
"</html>"
"";
void sendHTML(const char* htmlContent) {
    linkID = extractLinkID(CpyBuffer);
    if (linkID < 0 || linkID > 4) return;

    int totalLen = strlen(htmlContent);
    int sent = 0;

    while (sent < totalLen) {
        int chunkLen = (totalLen - sent > MAX_SEND_SIZE) ? MAX_SEND_SIZE : (totalLen - sent);
        
        snprintf(cmdBuffer, sizeof(cmdBuffer), "AT+CIPSEND=%d,%d\r\n", linkID, chunkLen);
        SendTxt_usart2(cmdBuffer);
        delay_ms(100);
        
        for (int i = 0; i < chunkLen; i++) {
            SendChar_usart2(htmlContent[sent + i]);
        }
        sent += chunkLen;
				delay_ms(100);
    }
    /*sprintf(cmdBuffer,"AT+CIPCLOSE=%d\r\n", linkID);
    SendTxt_usart2(cmdBuffer);
    delay_ms(100);
		ClearBuffer(cmdBuffer, sizeof(cmdBuffer));*/
}



int extractLinkID(const char* buffer) {
    const char* start = strstr(buffer, "+IPD,");
    if (start) {
        return atoi(start + 5); // "+IPD," = 5 caractères
    }
    return -1; // Erreur, pas trouvé
}

void all_data_res() {
    char content[500];

    if (esp_active) {
        sprintf(content,
            "ESP_TEMP: %.2f\nESP_CH1: %hu\nESP_CH2: %hu\nESP_CH3: %hu\n",
            Esp_temperature,
            Esp_ADC_Value[0],
            Esp_ADC_Value[1],
            Esp_ADC_Value[2]
        );
    } else {
        sprintf(content,
            "ESP_TEMP: N/A\nESP_CH1: N/A\nESP_CH2: N/A\nESP_CH3: N/A\n"
        );
    }

    if (lora_active) {
        char lora_content[250];
        sprintf(lora_content,
            "LORA_TEMP: %.2f\nLORA_CH1: %hu\nLORA_CH2: %hu\nLORA_CH3: %hu",
            Lora_temperature,
            Lora_ADC_Value[0],
            Lora_ADC_Value[1],
            Lora_ADC_Value[2]
        );
        strcat(content, lora_content);
    } else {
        strcat(content,
            "LORA_TEMP: N/A\nLORA_CH1: N/A\nLORA_CH2: N/A\nLORA_CH3: N/A"
        );
    }

    char resp[600];
    int content_length = strlen(content);

    sprintf(resp,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "%s",
        content_length,
        content
    );
    sendHTML(resp);
}


void processRequest(char* request) {
    if ((strstr(request,"GET / HTTP"))) {
        sendHTML(Login_Page);
    }
		else  if (strstr(request, "GET /login?user=admin&pass=admin") != NULL) {
        sendHTML(main_page);
    }
		else if (strstr(request, "GET /dataCenter") != NULL) {
        sendHTML(Data_Center_page);
    }
		if (strstr(request, "gateway_lora=on&gateway_espwifi=on") != NULL){
            lora_active = true;
						esp_active = true;
						pagenumb=3;
						sendHTML(server_config_page_LORAon_ESPon);
    }
		else if (strstr(request, "gateway_espwifi=on&gateway_lora=off") != NULL) {
            lora_active = false;
						esp_active = true;
						pagenumb=2;
						sendHTML(server_config_page_LORAoff_ESPon);
    }
		else if (strstr(request, "gateway_lora=on&gateway_espwifi=off") != NULL) {
            esp_active = false;
						lora_active = true;
						pagenumb=1;
						sendHTML(server_config_page_LORAon_ESPoff);
						//sendHTML(server_config_page_desactiver_ESP);
    }
		else if ((strstr(request, "gateway_lora=off&gateway_espwifi=off") != NULL)||(strstr(request, "gateway_espwifi=off&gateway_lora=off") != NULL)) {
            esp_active = false;
						lora_active = false;
						pagenumb=0;
						sendHTML(server_config_page);
						//sendHTML(server_config_page_desactiver_ESP);
    }	
		else if (strstr(request, "GET /configServer") != NULL) {

						switch (pagenumb) {
								case 1:
										sendHTML(server_config_page_LORAon_ESPoff);
										break;
								case 2:
										sendHTML(server_config_page_LORAoff_ESPon);
										break;
								case 3:
										sendHTML(server_config_page_LORAon_ESPon);
										break;
								default:
										sendHTML(server_config_page);  // fallback
										break;
						}
    } 
		else if (strstr(request, "GET /all_data")) {
				all_data_res();
		}
}

