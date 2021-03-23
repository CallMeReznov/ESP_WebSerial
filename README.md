
<h1 align="center">ESP_WebSerial</h1>


<div align="center">使用ESP32开发板制作的串口调试工具,无需客户端,支持移动端操作.</div>

---
![开发板1](./img/1.jpg)
![开发板2](./img/2.jpg)

## 更新日志  
* 20210318 构思框架，完成前端demo，WS调试通过。
* 20210319 编写固件，完成WSSERVER WEBSERVER SPIFFS AP 相关基础。
* 20210322 完成串口与网页的通讯调试。  
* 20210323 解决了一些交换机交互的ASCII问题，完成V1.0。

## 开发环境 
开发语言 : Arduino  
开发工具 : Visual Studio 2019  Community  & Arduino IDE
### 项目组件
组件名|简介
:-|:-
Bootstrap | 网页前端框架
ESPAsyncWebServer | Esp32WebServer
ArduinoWebSockets | ESP32WebSocketServer
SPIFFS | ESP32文件系统操作用来保存页面

## 使用说明
编译固件,使用ESP32 data upload上传/data内的文件，正确连接RS-232 TO TTL模块到开发板指定针脚开机即可(代码内D5 D6定义针脚与VCC,GND)

## TODO 
* [ ] 增加外置显示
* [x] 随机WIFI密码(WPS)
* [ ] 一些字符缺失以及乱码问题  
* [ ] 移植到8266之类的开发板上
* [ ] 集成RS-232到PCB上,直接使用网络线线路




## 相关扩展阅读

ESP32­-WROVER­-B DataSheet:  
https://www.espressif.com/sites/default/files/documentation/esp32-wrover-b_datasheet_cn.pdf

Development board :  
https://github.com/LilyGO/LILYGO-T-Energy

ASCII Character Set and Hexadecimal Values :  
https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/fundamentals/command/cf_command_ref/ASCII_Character_Set_and_Hexadecimal_Values.html

TTL/RS-232 
https://blog.csdn.net/zzwdkxx/article/details/7936578?_t_t_t=0.8076314434874803
