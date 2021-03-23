//ESP32 Only   Core : ESP32­-WROVER­-B
//Development board : https://github.com/LilyGO/LILYGO-T-Energy
// 
//IF 8266
//#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer.h>
//#include <SoftwareSerial.h>
#include <HardwareSerial.h>

//设置WIFI密码
String comdata = "";
const char* ssid = "test12345";
const char* password = "test12345";

#define D5 (18)
#define D6 (22)

//SoftwareSerial swSer2;
HardwareSerial swSer2(2);
//Web服务
AsyncWebServer server(80);
//WebSocket服务
WebSocketsServer webSocket = WebSocketsServer(8088);


//404页
void errorpage(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}


void hexdump(const void* mem, uint32_t len, uint8_t cols = 16) {
    const uint8_t* src = (const uint8_t*)mem;
    Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for (uint32_t i = 0; i < len; i++) {
        if (i % cols == 0) {
            Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        Serial.printf("%02X ", *src);
        src++;
    }
    Serial.printf("\n");
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

    switch (type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);


        // send message to client
        webSocket.sendTXT(num, "Connected");
    }
        break;
    //串口交互ababababababababababababa
    case WStype_TEXT:
        for (int i = 0; i < length; i++) {
            swSer2.write(payload[i]);
        }

        //Serial.printf("%s\n", payload);
        //swSer2.printf("%s\n", payload);
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);
        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }

}



void setup() {
  Serial.begin(115200);
  //设备通讯口默认是交换机
  //SerialServer.begin(9600, SERIAL_8N1, RX1, TX1);
  swSer2.begin(9600, SERIAL_8N1, D5, D6);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS error");
    return;
  }
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(ssid, password);

  
  //首页
  //返回data文件夹内的文件内容,需使用SPIFFS先上传到开发板的ROM里.
  //好像Arduino现有版本对我在使用开发板的psram的支持并不是那么友好.
  //选择4M（1.2 APP 1.5 SPIFFS ，WebPage占用791KB）
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/jquery.min.js", "  application/x-javascript");
      });
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/bootstrap.min.js", "  application/x-javascript");
      });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/bootstrap.min.css", "text/css");
      });
  server.on("/bootstrap.min.css.map", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(SPIFFS, "/bootstrap.min.css.map", "text/html");
      });
  //404页
  server.onNotFound(errorpage);
  server.begin();
  //WebSocket服启动.
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.printf("ok");
}

void loop() {
    webSocket.loop();
    if (swSer2.available() > 0)//判读是否串口有数据
    {
        String comdata = "";//缓存清零
        while (swSer2.available() > 0)//循环串口是否有数据
        {
            comdata += char(swSer2.read());//叠加数据到comdata
            delay(5);//延时等待响应
        }
        if (comdata.length() > 0)//如果comdata有数据
        {
            webSocket.broadcastTXT(comdata);
            Serial.println(comdata);//打印comdata数据
        }
    }
}