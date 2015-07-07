/**************************************************************************
    This library is for the ESP8266
    
    It helps with the dynamic configuration of the ESP8266 for accessing
    various WiFi access points by providing an internal web page for configuration
    by computer or mobile phone and stores the config in the EEPROM
    
    Written by: Richard Wardlow for Circuits for Fun, LLC
    GNU GPL, include above text in redistribution
***************************************************************************/
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include "ESP8266DynConfig.h"


////////////////////////////////////////////////
// ESP8266 WiFi Dynamic Config Class Mehtods
////////////////////////////////////////////////
void ESP8266DynConfig::enableConfigMode(char *apName, ESP8266WebServer &webServer)
{
    _server = &webServer;
    _configMode = true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(apName);
    WiFi.disconnect();
}

void ESP8266DynConfig::handleClient()
{
    _server->handleClient();
}

void ESP8266DynConfig::clearEEPROMHeader()
{
  for(int x=0;x<132;x++)
  {
    EEPROM.write(x, '\0');
  }
}

void ESP8266DynConfig::writeEEPROMHeader()
{
    EEPROM.begin(132);
    clearEEPROMHeader();
    WiFiConfig.eSSIDLen = sizeof(WiFiConfig.eSSID)-1;
    WiFiConfig.ePassLen = sizeof(WiFiConfig.ePass)-1;
    WiFiConfig.eNameLen = sizeof(WiFiConfig.eName)-1;
    EEPROM.write(0, WiFiConfig.eSSIDLen);
    for(int x=0;x<WiFiConfig.eSSIDLen;x++)
    {
        EEPROM.write(x+1, WiFiConfig.eSSID[x]);
    }
    EEPROM.write(33, WiFiConfig.ePassLen);
    for(int x=0;x<WiFiConfig.ePassLen;x++)
    {
        EEPROM.write(x+34, WiFiConfig.ePass[x]);
    }
    EEPROM.write(98, WiFiConfig.eNameLen);
    for(int x=0;x<WiFiConfig.eNameLen;x++)
    {
        EEPROM.write(x+99, WiFiConfig.eName[x]);
    }
    EEPROM.commit();
}

void ESP8266DynConfig::readEEPROMHeader()
{
    EEPROM.begin(132);
    WiFiConfig.eSSIDLen = EEPROM.read(0);
    if(WiFiConfig.eSSIDLen != 0)
    {
        for(int x=0;x<WiFiConfig.eSSIDLen;x++)
        {
            WiFiConfig.eSSID[x] = EEPROM.read(x+1);
        }
        WiFiConfig.eSSID[WiFiConfig.eSSIDLen] = '\0'; 
    }
    WiFiConfig.ePassLen = EEPROM.read(33);
    if(WiFiConfig.ePassLen != 0)
    {
        for(int x=0;x<WiFiConfig.ePassLen;x++)
        {
            WiFiConfig.ePass[x] = EEPROM.read(x+34);
        } 
        WiFiConfig.ePass[WiFiConfig.ePassLen+34] = '\0';
    }
    WiFiConfig.eNameLen = EEPROM.read(98);
    if(WiFiConfig.eNameLen != 0)
    {
        for(int x=0;x<WiFiConfig.eNameLen;x++)
        {
            WiFiConfig.eName[x] = EEPROM.read(x+99);
        } 
        WiFiConfig.eName[WiFiConfig.eNameLen+99] = '\0';
    }
}

boolean ESP8266DynConfig::readConfig()
{
    readEEPROMHeader();
    if (sizeof(WiFiConfig.eSSID) - 1 <= 0)
        return false;
    else
        return true;
}

void ESP8266DynConfig::handleConfigPage()
{
    String response = "<html><head><title>Wifi Config</title></head><body><div id=\"container\">";
    response += "<div id=\"wb_Form1\" style=\"position:absolute;left:0px;top:0px;width:302px;height:201px;z-index:7;\">";
    response += "<form name=\"ConfigWifi\" method=\"post\" action=\"savesettings.htm\" enctype=\"text/plain\" id=\"Form1\">";
    response += "<div id=\"wb_Text1\" style=\"position:absolute;left:10px;top:15px;width:62px;height:16px;z-index:0;text-align:left;\">";
    response += "<span style=\"color:#000000;font-family:Arial;font-size:13px;\">SSID</span></div>";
    response += "<select name=\"SSID\" size=\"1\" id=\"Combobox1\" style=\"position:absolute;left:82px;top:15px;width:200px;height:25px;z-index:1;\">";
    int n = WiFi.scanNetworks();
    if (n != 0)
    {
        for (int i = 0; i < n; i++)
        {
            response += "<option>";
            response += WiFi.SSID(i);
            response += "</option>";
        }
    }
    response += "</select>";
    response += "<div id=\"wb_Text2\" style=\"position:absolute;left:10px;top:45px;width:62px;height:16px;z-index:2;text-align:left;\">";
    response += "<span style=\"color:#000000;font-family:Arial;font-size:13px;\">Password</span></div>";
    response += "<input type=\"password\" id=\"Editbox1\" style=\"position:absolute;left:82px;top:45px;width:198px;height:23px;line-height:23px;z-index:3;\" name=\"Password\" value=\"\" maxlength=\"64\">";
    response += "<div id=\"wb_Text3\" style=\"position:absolute;left:10px;top:75px;width:62px;height:16px;z-index:4;text-align:left;\">";
    response += "<span style=\"color:#000000;font-family:Arial;font-size:13px;\">Name</span></div>";
    response += "<input type=\"text\" id=\"Editbox2\" style=\"position:absolute;left:82px;top:75px;width:198px;height:23px;line-height:23px;z-index:5;\" name=\"devName\" value=\"esp8266-01\" maxlength=\"32\">";
    response += "<input type=\"submit\" id=\"Button1\" name=\"submit\" value=\"Save\" style=\"position:absolute;left:102px;top:144px;width:96px;height:25px;z-index:6;\">";
    response += "</form></div></div></body></html>";
    _server->send(200,"text/html",response);

}

void ESP8266DynConfig::handleConfigPostPage()
{
    String tmpVal = _server->arg("SSID");
    char tmpData[128];

    tmpVal.toCharArray(tmpData, 128);
    char * spt;
    spt = strtok (tmpData,"\r=");
    strcpy(WiFiConfig.eSSID, spt);

    spt = strtok (NULL, "\r=");
    spt = strtok (NULL, "\r=");
    strcpy(WiFiConfig.ePass, spt);

    spt = strtok (NULL, "\r=");
    spt = strtok (NULL, "\r=");
    strcpy(WiFiConfig.eName, spt);
    writeEEPROMHeader();
    String response = "Setup Complete. Reset module to continue! \r\n";
    _server->send(200, "text/plain", response);
}

