#ifndef SENSOROBSERVER_H
    #define SENSOROBSERVER_H

    #include <Arduino.h>

    class SensorObserver {
        public:
            virtual void update(double temperature, double humidity) = 0;
            virtual ~SensorObserver() = default;
    };
#endif