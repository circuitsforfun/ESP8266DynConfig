ESP8266 Dynamic Configuration
====================

A simple configuration web page served up in AP mode

Use the library to help put the ESP8266 into AP mode, serve up a config page and save settings in EEPROM

Requirements
------------
- ESP8266WiFi library

Usage
-----
1. Download this repository as a zip (button on the right)
2. Unzip contents into: [Arduino Root Path]\hardware\esp8266com\esp8266\libraries\ESP8266DynConfig
3. Create an instance of the ESP8266DynConfig class.
4. Create wrappers for handleConfigPage, and handleConfigPostPage
5. Use whatever method you want to check for config mode, for example if a button is pressed during power on
6. enable config mode using class method
7. point server handler for root and savesettings to the wrapper functions
8. begin server 

See the included Dynamic Config sketch for a full example.

License
-------
Copyright (c) 2015 Richard Wardlow (richard@circuitsforfun.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
