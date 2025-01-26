#include <ClosedCube_HDC1080.h>

// EEPROM
const uint16_t TIME_SAVE_EEPROM =                                       5000;

// Serial Monitor
const uint32_t BAUDRATE =                                               115200;



// ### LIBRARIES ###
// Button
const uint8_t BUTTON_PIN =                                              D5;
const int16_t BUTTON_TIME_LONG_PRESS =                                  3000;

// Screen
const uint8_t SCREEN_PIN_SCL =                                          20;
const uint8_t SCREEN_PIN_SDA =                                          19;
const uint16_t TIME_TO_STANDBY =                                        5000;
const uint16_t TIME_LOGO =                                              BUTTON_TIME_LONG_PRESS + 1000;

// Sensor
const uint8_t SENSOR_ADDRESS =                                          0x40;
const HDC1080_MeasurementResolution SENSOR_HUMIDITY_RESOLUTION =        HDC1080_RESOLUTION_14BIT;
const HDC1080_MeasurementResolution SENSOR_TEMPERATURE_RESOLUTION =     HDC1080_RESOLUTION_14BIT;
