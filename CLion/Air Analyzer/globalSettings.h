#pragma once

// Versions
const String VERSION_FIRMWARE =                                         "4.0.0";
const uint8_t VERSION_EEPROM =                                          3;

// EEPROM
const uint8_t SIZE_EEPROM =                                             186;
const uint8_t ADDRESS_VERSION_EEPROM =                                  0;          // Size: 1
const uint8_t ADDRESS_WIFI_SSID =                                       1;          // Size: 34
const uint8_t ADDRESS_WIFI_PASSWORD =                                   35;         // Size: 65
const uint8_t ADDRESS_ROOM_ID =                                         100;        // Size: 1
const uint8_t ADDRESS_CREDENTIAl_USERNAME =                             101;        // Size: 21
const uint8_t ADDRESS_CREDENTIAL_PASSWORD =                             122;        // Size: 65
const uint16_t TIME_SAVE_EEPROM =                                       5000;

// Serial Monitor
const uint32_t BAUDRATE =                                               115200;

// WiFi
const uint8_t SIZE_WIFI_SSID =                                          33;
const uint8_t SIZE_WIFI_PASSWORD =                                      64;

// Socket
const uint32_t PORT_SERVER_SOCKET =                                     60000;
const uint8_t SIZE_CREDENTIAL_USERNAME =                                20;
const uint8_t SIZE_CREDENTIAL_PASSWORD =                                64;

// Server (Database and Updater)
const uint8_t API_MINUTES_UPDATE_MEASURES =                             1;
const String BASE_URL =                                                 "http://airanalyzer.servehttp.com";
const uint16_t BASE_PORT =                                              81;
const String FIRMWARE_URI =                                             "api/firmware/getLatest";

// Sensor
const uint8_t ADDRESS_SENSOR =                                          0x40;
const HDC1080_MeasurementResolution HUMIDITY_RESOLUTION =               HDC1080_RESOLUTION_14BIT;
const HDC1080_MeasurementResolution TEMPERATURE_RESOLUTION =            HDC1080_RESOLUTION_14BIT;

// Button
const uint8_t PIN_BUTTON =                                              D5;
const int16_t TIME_LONG_PRESS =                                         3000;

// OLED screen
const uint8_t PIN_SCL =                                                 20;
const uint8_t PIN_SDA =                                                 19;
const uint16_t TIME_LOGO = TIME_LONG_PRESS + 1000;
const uint16_t TIME_LOADING_MESSAGE =                                   3000;
const uint16_t TIME_MESSAGE =                                           5000;