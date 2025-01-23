 /**
 * @brief This library allows to manage the sensor for the Air Analyzer purposes. You can use DHT or HDC sensor.
 * The class is an subject that could have some observers, that will be notified if
 *  there will be an update, with the "check" method.
 * Copyright (c) 2022 Davide Palladino.
 * All right reserved.
 *
 * @author Davide Palladino
 * @contact davidepalladino@hotmail.com
 * @website https://davidepalladino.github.io/
 * @version 1.1.2
 * @date 23rd January 2025
 *
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

    const uint16_t TIMEOUT_READ_HDC = 1000;                                   // Timeout to read the new value from HDC sensor.

    class Sensor : private AbstractSubject {
        public:
            friend class Screen;
            friend class ApiManagement;

            /** 
             * @brief This constructor creates the object setting only the type of sensor in DHT family and the pin.
             * @param pin Pin where is connected the sensor.
             * @param type Type between "DHT11", "DHT12", "DHT21", "DHT22" and "AM2301".
             */
            Sensor(uint8_t pin, uint8_t type);

            /** 
             * @brief This constructor creates the object setting only the type of sensor in HDC family, the I2C address and the resolution for humidity and temperature.
             * @param address I2C address of the sensor.
             * @param humidityResolution Value between "HDC1080_RESOLUTION_8BIT", "HDC1080_RESOLUTION_11BIT" and "HDC1080_RESOLUTION_14BIT".
             * @param temperatureResolution Value between "HDC1080_RESOLUTION_8BIT", "HDC1080_RESOLUTION_11BIT" and "HDC1080_RESOLUTION_14BIT".
             */
            Sensor(uint8_t address, HDC1080_MeasurementResolution humidityResolution, HDC1080_MeasurementResolution temperatureResolution);

            /** 
             * @brief This method initializes the sensor object.
             */
            void begin();

            /** 
             * @brief This method checks if there is some variation of temperature or humidity and if there will be, the observers will be notified.
             * @return Boolean value "true" to indicate if there is a variation; else value "false".
             */
            bool check();

            /** 
             * @brief This method gets the last value read about the temperature.
             * @return Last value of temperature read.
             */
            double getTemperature();

            /** 
             * @brief This method gets the last value read about the humidity.
             * @return Last value of humidity read.
             */
            double getHumidity();

        private:
            std::list<AbstractObserver* > observers;
            DHT *sensorDHT;
            ClosedCube_HDC1080 *sensorHDC;
            uint8_t address;
            HDC1080_MeasurementResolution humidityResolution;
            HDC1080_MeasurementResolution temperatureResolution;
            double temperature;
            double humidity;
            unsigned long endTimeoutHDC;

            /** 
             * @brief This method checks if the is some variation of temperature between the object and the parameter.
             * @param temperature Value to compare.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool checkTemperature(double temperature);

            /** 
             * @brief This method checks if the is some variation of humidity between the object and the parameter.
             * @param humidity Value to compare.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool checkHumidity(double humidity);

            /** 
             * @brief This method adds an observer.
             * @param observer Object to add.
             */
            void addObserver(AbstractObserver* observer) override;

            /** 
             * @brief This method remove an observer.
             * @param observer Object to remove.
             */
            void removeObserver(AbstractObserver* observer) override;

            /** 
             * @brief This method notifies all observers.
             */
            void notify() override;
    };
#endif