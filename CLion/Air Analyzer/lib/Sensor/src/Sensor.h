/**
 * @file Sensor.h
 * @brief Provides functionality to manage sensors for the Air Analyzer system.
 *
 * This library supports both DHT and HDC sensor types, allowing the system to monitor temperature and humidity.
 * The Sensor class acts as a subject, notifying observers when new data is available via the check method.
 *
 * Copyright (c) 2025 Davide Palladino.
 * All rights reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 2.0.0
 * @date 23rd January 2025
 */

#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ClosedCube_HDC1080.h>
#include <AbstractSubject.h>
#include <AbstractObserver.h>
#include <list>

const uint16_t TIMEOUT_READ_HDC = 1000; /**< Timeout for reading data from the HDC sensor. */

/**
 * @class Sensor
 * @brief Manages temperature and humidity sensors, providing data to observers.
 */
class Sensor : private AbstractSubject {
    friend class Screen;
    friend class ApiManagement;

    public:
        /**
         * @brief Constructs a Sensor object using a DHT sensor type.
         * @param pin The GPIO pin where the sensor is connected.
         * @param type The sensor type (e.g., DHT11, DHT22, AM2301).
         */
        Sensor(uint8_t pin, uint8_t type);

        /**
         * @brief Constructs a Sensor object using an HDC sensor type.
         * @param address The I2C address of the sensor.
         * @param humidityResolution Humidity measurement resolution.
         * @param temperatureResolution Temperature measurement resolution.
         */
        Sensor(uint8_t address, HDC1080_MeasurementResolution humidityResolution, HDC1080_MeasurementResolution temperatureResolution);

        /** @brief Initializes the sensor. */
        void begin();

        /**
         * @brief Checks for changes in temperature or humidity and notifies observers if needed.
         * @return True if a variation is detected, false otherwise.
         */
        bool check();

        /**
         * @brief Retrieves the last recorded temperature value.
         * @return The last measured temperature.
         */
        double getTemperature();

        /**
         * @brief Retrieves the last recorded humidity value.
         * @return The last measured humidity.
         */
        double getHumidity();

    private:
        std::list<AbstractObserver*> observers; /**< List of observer objects. */
        DHT *sensorDHT; /**< Pointer to a DHT sensor instance. */
        ClosedCube_HDC1080 *sensorHDC; /**< Pointer to an HDC sensor instance. */
        uint8_t address; /**< I2C address of the sensor. */
        HDC1080_MeasurementResolution humidityResolution; /**< Humidity resolution setting. */
        HDC1080_MeasurementResolution temperatureResolution; /**< Temperature resolution setting. */
        double temperature; /**< Last recorded temperature. */
        double humidity; /**< Last recorded humidity. */
        unsigned long endTimeoutHDC; /**< Timeout marker for HDC sensor readings. */

        /**
         * @brief Compares current and previous temperature values to detect changes.
         * @param temperature The temperature value to compare.
         * @return True if a variation is detected, false otherwise.
         */
        bool checkTemperature(double temperature);

        /**
         * @brief Compares current and previous humidity values to detect changes.
         * @param humidity The humidity value to compare.
         * @return True if a variation is detected, false otherwise.
         */
        bool checkHumidity(double humidity);

        /**
         * @brief Adds an observer to the notification list.
         * @param observer Pointer to the observer to be added.
         */
        void addObserver(AbstractObserver* observer) override;

        /**
         * @brief Removes an observer from the notification list.
         * @param observer Pointer to the observer to be removed.
         */
        void removeObserver(AbstractObserver* observer) override;

        /**
         * @brief Notifies all registered observers of data updates.
         */
        void notify() override;
};

#endif // SENSOR_H
