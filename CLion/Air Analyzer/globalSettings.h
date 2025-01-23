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

// Sensor
const uint8_t SENSOR_ADDRESS =                                          0x40;
const HDC1080_MeasurementResolution SENSOR_HUMIDITY_RESOLUTION =        HDC1080_RESOLUTION_14BIT;
const HDC1080_MeasurementResolution SENSOR_TEMPERATURE_RESOLUTION =     HDC1080_RESOLUTION_14BIT;

// Button
const uint8_t BUTTON_PIN =                                              D5;
const int16_t BUTTON_TIME_LONG_PRESS =                                  3000;

// OLED screen
const uint8_t SCREEN_PIN_SCL =                                          20;
const uint8_t SCREEN_PIN_SDA =                                          19;
const uint16_t TIME_LOGO =                                              BUTTON_TIME_LONG_PRESS + 1000;
const uint16_t TIME_LOADING_MESSAGE =                                   3000;
const uint16_t TIME_MESSAGE =                                           5000;
const uint16_t TIME_TO_STANDBY =                                        5000;

// Rooms
const uint8_t MIN_ROOM_NUMBER =                                         1;
const uint8_t MAX_ROOM_NUMBER =                                         9;

// FirmwareUpdateOta
const String FIRMWARE_UPDATE_OTA_BASE_URL =                             "http://airanalyzer.shadowmoses.ovh";
constexpr uint16_t FIRMWARE_UPDATE_OTA_BASE_PORT =                      80;
const String FIRMWARE_UPDATE_OTA_URI_GET_LATEST =                       "api/firmware/getLatest";

// ApiManagement
const String API_MANAGEMENT_BASE_URL =                                  "http://airanalyzer.shadowmoses.ovh";
constexpr uint16_t API_MANAGEMENT_BASE_PORT =                           80;
const String API_MANAGEMENT_URI_USER_LOGIN =                            "api/user/login";
const String API_MANAGEMENT_URI_ROOM_CHANGE_STATUS_ACTIVATION =         "api/room/changeStatusActivation";
const String API_MANAGEMENT_URI_ROOM_API_CHANGE_LOCAL_IP =              "api/room/changeLocalIP";
const String API_MANAGEMENT_URI_MEASURE_SET =                           "api/measure/set";
constexpr uint8_t API_MANAGEMENT_MAX_ATTEMPTS =                         10;
constexpr uint8_t API_MANAGEMENT_MINUTES_UPDATE_MEASURES =              10;