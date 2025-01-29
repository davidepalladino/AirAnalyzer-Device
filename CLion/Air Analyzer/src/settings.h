#ifndef SETTINGS_H
    #define SETTINGS_H
    #include <ClosedCube_HDC1080.h>

    // EEPROM
    constexpr uint16_t TIME_SAVE_EEPROM =                                       5000;

    // Serial Monitor
    constexpr uint32_t BAUDRATE =                                               115200;



    // ### LIBRARIES ###
    // Button
    constexpr uint8_t BUTTON_PIN =                                              D5;
    constexpr int16_t BUTTON_TIME_LONG_PRESS =                                  3000;

    // Screen
    constexpr uint8_t SCREEN_PIN_SCL =                                          20;
    constexpr uint8_t SCREEN_PIN_SDA =                                          19;
    constexpr uint16_t TIME_TO_STANDBY =                                        5000;
    constexpr uint16_t TIME_LOGO =                                              BUTTON_TIME_LONG_PRESS + 1000;

    // Sensor
    constexpr uint8_t SENSOR_ADDRESS =                                          0x40;
    constexpr HDC1080_MeasurementResolution SENSOR_HUMIDITY_RESOLUTION =        HDC1080_RESOLUTION_14BIT;
    constexpr HDC1080_MeasurementResolution SENSOR_TEMPERATURE_RESOLUTION =     HDC1080_RESOLUTION_14BIT;
#endif // SETTINGS_H
