/**************************************************************************
    This library is for the ESP8266
    
    It helps with the dynamic configuration of the ESP8266 for accessing
    various WiFi access points by providing an internal web page for configuration
    by computer or mobile phone and stores the config in the EEPROM
    
    Written by: Richard Wardlow for Circuits for Fun, LLC
    GNU GPL, include above text in redistribution
***************************************************************************/

typedef struct{
  char eSSIDLen;
  char eSSID[32];
  char ePassLen;
  char ePass[64];
  char eNameLen;
  char eName[32];
}WIFICONFIGHEADER;

////////////////////////////////////////
// ESP8266 WiFi Dynamic Config Class
////////////////////////////////////////
class ESP8266DynConfig
{
    public:
        void clearEEPROMHeader();
        void writeEEPROMHeader();
        void readEEPROMHeader();
        void enableConfigMode(char *apName, ESP8266WebServer &webServer);
        boolean readConfig();
        void handleConfigPage();
        void handleConfigPostPage();
        void handleClient();
        WIFICONFIGHEADER WiFiConfig;
    private:
        boolean _configMode;
        ESP8266WebServer *_server;
};