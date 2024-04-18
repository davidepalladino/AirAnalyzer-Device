# AirAnalyzer-Device
## Description
This repository concerns the software of device for the Air Analyzer project, where the user can:
 - see the actual temperature and humidity;
 - change the room ID;
 - change the WiFi connection;
 - reset the device.

## Components used
For this project I'm using these components:
 - NodeMCU ESP8266 ESP-12F;
 - OLED screen by 0.98" in I2C;
 - HDC1080 sensor;
 - RTC DS3231;
 - Push button.

### Pinout
| Pin | What |
| --- | ---- |
| D1 | SCL (for OLED, HDC1080 and DS3231) |
| D2 | SDA (for OLED, HDC1080 and DS3231) |
| D5 | Push button |

## Related posts
* [Air Analyzer v2.0.3](https://davidepalladino.github.io/2021/07/12/Air-Analyzer-v2-0-3/)
* [Air Analyzer v3.0.0](https://davidepalladino.github.io/2022/02/13/Air-Analyzer-v3-0-0/)
