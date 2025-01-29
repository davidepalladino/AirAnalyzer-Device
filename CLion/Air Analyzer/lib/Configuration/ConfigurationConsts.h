#pragma once

// ### MAIN ###
// Versions
const String VERSION_FIRMWARE =                                         "6.0.0";
constexpr uint8_t VERSION_EEPROM =                                      3;

// EEPROM
constexpr uint8_t SIZE_EEPROM =                                         186;
constexpr uint8_t ADDRESS_VERSION_EEPROM =                              0;          // Size: 1
constexpr uint8_t ADDRESS_WIFI_SSID =                                   1;          // Size: 34
constexpr uint8_t ADDRESS_WIFI_PASSWORD =                               35;         // Size: 65
constexpr uint8_t ADDRESS_ROOM_ID =                                     100;        // Size: 1
constexpr uint8_t ADDRESS_CREDENTIAl_USERNAME =                         101;        // Size: 21
constexpr uint8_t ADDRESS_CREDENTIAL_PASSWORD =                         122;        // Size: 65

// Wi-Fi
constexpr uint8_t SIZE_WIFI_SSID =                                      33;
constexpr uint8_t SIZE_WIFI_PASSWORD =                                  64;

// Rooms
constexpr uint8_t MIN_ROOM_NUMBER =                                     1;
constexpr uint8_t MAX_ROOM_NUMBER =                                     9;



// ### LIBRARIES ###
// Api Management
const String API_MANAGEMENT_BASE_ADDRESS =                              "http://airanalyzer.shadowmoses.ovh";
constexpr uint8_t API_MANAGEMENT_BASE_PORT =                            80;
constexpr uint8_t API_MANAGEMENT_MAX_ATTEMPTS =                         3;
constexpr uint8_t API_MANAGEMENT_MINUTES_UPDATE_MEASURES =              10;

// Firmware Update OTA
const String FIRMWARE_UPDATE_OTA_BASE_ADDRESS =                         "http://airanalyzer.shadowmoses.ovh";
constexpr uint16_t FIRMWARE_UPDATE_OTA_BASE_PORT =                      80;

// Screen
constexpr uint16_t TIME_LOADING_MESSAGE =                               3000;
constexpr uint16_t TIME_MESSAGE =                                       5000;

// Server Socket JSON
constexpr uint32_t SERVER_SOCKET_PORT =                                 60000;