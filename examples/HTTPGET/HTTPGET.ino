/**
 * @example HTTPGET.ino
 * @brief The HTTPGET demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SoftwareSerial.h>
#include "ESP8266.h"

#define SSID        "Lab430PrinterWifi"
#define PASSWORD    "androidiphone"
#define HOST_NAME   "10.4.28.9"
#define HOST_PORT   (8080)
#define BUFFER_SIZE (128)

enum{
  GET_METHOD = 0,
  POST_METHOD = 1,
  PUT_METHOD = 2,
  DEL_METHOD = 3
};

const char* methodNames[] = { 
  "GET",
  "POST",
  "PUT",
  "DEL"
};

SoftwareSerial wifiSerial(10,11);
ESP8266 wifi(wifiSerial);

void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");

    Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());

    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }
    while(true) {
      if (wifi.joinAP(SSID, PASSWORD)) {
          Serial.print("Join AP success\r\n");
  
          Serial.print("IP:");
          Serial.println( wifi.getLocalIP().c_str());   
          break;    
      } else {
          Serial.print("Join AP failure\r\n");
          delay(1000);
      }
    }

    while(true) {
      if (wifi.disableMUX()) {
          Serial.print("single ok\r\n");
          break;
      } else {
          Serial.print("single err\r\n");
          delay(2000);
      }
    }
    
    Serial.print("setup end\r\n");
}


void httpRequest(int methodType, const char* hostName, int port, const char* reqPath) {
    
    while(true) {
      //we need to establish tcp link before send out http request
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) { 
          Serial.print("create tcp ok\r\n");
          break;
      } else {
          Serial.print("create tcp err\r\n");
          delay(500);
      }
    }
    
    char buffer[BUFFER_SIZE] = {0}; //buffer too large would cause tcp failing
    sprintf(buffer,"%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",methodNames[methodType],reqPath,hostName);
    wifi.send((const uint8_t*)buffer, strlen(buffer));

//    Serial.print("buffer:");
//    Serial.println(buffer);
    
    uint32_t len = wifi.recv((uint8_t *)buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }
    
    if (wifi.releaseTCP()) {
      Serial.print("release tcp ok\r\n");
    } else {
      Serial.print("release tcp err\r\n");
    }
    
}


void loop(void)
{
    
    httpRequest(GET_METHOD, HOST_NAME, HOST_PORT, "/");
    delay(2000);
//    while(1);

    
    
    
}
     
