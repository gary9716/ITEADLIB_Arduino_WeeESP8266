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
#include "KTHttpRequest.h"

#define SSID        "Lab430 2.4GHz"
#define PASSWORD    "androidiphone"
#define HOST_NAME   "www.google.com.tw"
#define HOST_PORT   (80)
#define BUFFER_SIZE (256)

SoftwareSerial wifiSerial(10,11);
ESP8266 wifi(wifiSerial);
const int RST = 13;
uint8_t myBuf[BUFFER_SIZE];

void setup(void)
{
    pinMode(RST, OUTPUT);
    digitalWrite(RST, LOW);
    delay(3000);
    digitalWrite(RST, HIGH);

    wifiSerial.begin(9600);
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

void loop(void)
{

    uint32_t len = httpRequest(wifi, myBuf, BUFFER_SIZE, GET_METHOD, HOST_NAME, HOST_PORT, "/");
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)myBuf[i]);
        }
        Serial.print("]\r\n");
    }
    delay(5000);
    //    while(1);




}

