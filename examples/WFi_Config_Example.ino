
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266DynConfig.h>
#include <EEPROM.h>

#define SERIALDEBUG

ESP8266WebServer server(80);
ESP8266DynConfig espConfig = ESP8266DynConfig();
int configMode = 0;

void handle_root() {
   String response = "This is the Root page in Normal Mode";
   server.send(200, "text/html",response); 
}

void ESP8266DynConfigPageFn_wrapper()
{
  espConfig.handleConfigPage();
}

void ESP8266DynConfigPostPageFn_wrapper()
{
  espConfig.handleConfigPostPage();
}

void setup() {

  
  Serial.begin(9600);
  pinMode(2, INPUT);
  
  delay(4000);
  if (digitalRead(2) == 0)
    configMode = 1;
  if (espConfig.readConfig() == false)
    configMode = 1;
  
  if(configMode == 1)
  {
      Serial.println("Config Mode Active");
      espConfig.enableConfigMode("esp8266", server);
      server.on("/", ESP8266DynConfigPageFn_wrapper);
      server.on("/savesettings.htm", ESP8266DynConfigPostPageFn_wrapper);
      server.begin();
  }
  else
  {
      WiFi.mode(WIFI_STA);
      #ifdef SERIALDEBUG
      Serial.print("ssid:");
      Serial.println(espConfig.WiFiConfig.eSSID);
      Serial.print("pass:");
      Serial.println(espConfig.WiFiConfig.ePass);
      Serial.print("name:");
      Serial.println(espConfig.WiFiConfig.eName);
      #endif
      WiFi.begin(espConfig.WiFiConfig.eSSID, espConfig.WiFiConfig.ePass);
      
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #ifdef SERIALDEBUG
        Serial.print(".");
        #endif
      }
      #ifdef SERIALDEBUG
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println(WiFi.localIP());
      Serial.println("Setup done");
      #endif
    
      delay(1000);
      
      server.on("/", handle_root);
      server.begin();
      #ifdef SERIALDEBUG
      Serial.println("HTTP server started");
      #endif
  }

  
}

void loop() {
  
  if(configMode == 1)
    espConfig.handleClient();
  else
    server.handleClient();
}

