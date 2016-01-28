#ifndef KT_HTTP_REQ_H
#define KT_HTTP_REQ_H

#include "ESP8266.h"
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

char cmdBuffer[128] = {0}; //buffer too large would cause tcp failing
uint32_t httpRequest(ESP8266& wifi, uint8_t* outBuffer, uint16_t buffSize, int methodType, const char* hostName, int port, const char* reqPath) {

    while(true) {
      //we need to establish tcp link before send out http request
      if (wifi.createTCP(hostName, port)) { 
          Serial.print("create tcp ok\r\n");
          break;
      } else {
          Serial.print("create tcp err\r\n");
          delay(500);
      }
    }
    
    sprintf(cmdBuffer,"%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",methodNames[methodType],reqPath,hostName);
    wifi.send((const uint8_t*)cmdBuffer, strlen(cmdBuffer));

    uint32_t len = wifi.recv(outBuffer, buffSize, 5000);
    
    if (wifi.releaseTCP()) {
      Serial.print("release tcp ok\r\n");
    } else {
      Serial.print("release tcp err\r\n");
    }

    return len;
    
}

#endif
