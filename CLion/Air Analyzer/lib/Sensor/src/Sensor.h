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
 * @version 3.0.0
 * @date 28th January 2025
 */

#ifndef SENSOR_H
    #define SENSOR_H

    #include <Arduino.h>
    #include <DHT.h>
    #include <DHT_U.h>
    #include <ClosedCube_HDC1080.h>
    #include <SensorObserver.h>
    #include <list>

    #include "SensorSubject.h"
    #include "SensorConsts.h"

    /**
     * @class Sensor
     * @brief Manages temperature and humidity sensors, providing data to observers.
     *
     * The Sensor class supports DHT and HDC sensor types and acts as a subject in an observer pattern.
     */
    class Sensor : public SensorSubject {
        friend class Screen;
        friend class ApiManagement;

        public:
        /**
             * @brief Constructs a Sensor object using a DHT sensor type.
             *
             * @param pin The GPIO pin where the DHT sensor is connected.
             * @param type The sensor type (e.g., DHT11, DHT22, AM2301).
             */
            Sensor(uint8_t pin, uint8_t type);

            /**
             * @brief Constructs a Sensor object using an HDC sensor type.
             *
             * @param address The I2C address of the HDC sensor.
             * @param humidityResolution Humidity measurement resolution setting.
             * @param temperatureResolution Temperature measurement resolution setting.
             */
            Sensor(uint8_t address, HDC1080_MeasurementResolution humidityResolution, HDC1080_MeasurementResolution temperatureResolution);

            /**
             * @brief Initializes the sensor hardware.
             *
             * This method should be called before collecting sensor data.
             */
            void begin();

            /**
             * @brief Checks for changes in temperature or humidity and notifies observers if a significant variation is detected.
             *
             * @return True if a variation is detected, false otherwise.
             */
            bool check();

            /**
             * @brief Retrieves the last recorded temperature value.
             *
             * @return The most recent temperature reading.
             */
            double getTemperature();

            /**
             * @brief Retrieves the last recorded humidity value.
             *
             * @return The most recent humidity reading.
             */
            double getHumidity();

            /**
             * @brief Adds an observer to the list of observers.
             *
             * @param observer Pointer to the observer object to be added.
             */
            void addObserver(SensorObserver* observer) override;

            /**
             * @brief Removes an observer from the list of observers.
             *
             * @param observer Pointer to the observer object to be removed.
             */
            void removeObserver(SensorObserver* observer) override;

        private:
            std::list<SensorObserver*> observers;                       /**< List of observer objects that get notified on data changes. */
            DHT *sensorDHT;                                             /**< Pointer to an instance of a DHT sensor object. */
            ClosedCube_HDC1080 *sensorHDC;                              /**< Pointer to an instance of an HDC sensor object. */
            uint8_t address;                                            /**< I2C address of the HDC sensor. */
            HDC1080_MeasurementResolution humidityResolution;           /**< Humidity resolution setting. */
            HDC1080_MeasurementResolution temperatureResolution;        /**< Temperature resolution setting. */
            double temperature;                                         /**< Last recorded temperature value. */
            double humidity;                                            /**< Last recorded humidity value. */
            unsigned long endTimeoutHDC;                                /**< Timeout marker for HDC sensor readings to prevent frequent updates. */

            /**
             * @brief Compares current and previous temperature values to detect changes.
             *
             * @param temperature The new temperature reading to compare.
             * @return True if a significant change is detected, false otherwise.
             * // FIXME: Is wrong. update receives values.
             */
            bool checkTemperature(double temperature);

            /**
             * @brief Compares current and previous humidity values to detect changes.
             *
             * @param humidity The new humidity reading to compare.
             * @return True if a significant change is detected, false otherwise.
             * // FIXME: Is wrong. update receives values.
             */
            bool checkHumidity(double humidity);

            /**
             * @brief Notifies all registered observers of data updates.
             */
            void notify() override;
    };

#endif // SENSOR_H
