# AirAnalyzer-Device
## Description
This repository concerns the software of device for the Air Analyzer project, where the user can:
 - see the actual temperature and humidity;
 - change the room ID;
 - change the WiFi connection;
 - reset the device.

## Components used
For this project I'm using this components:
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

## Image
<img src="https://www.davidepalladino.com/wp-content/uploads/2022/02/Air-Analyzer-Device-v3.0.0-scaled.jpg" alt="Home image" width="400px" height="auto">
