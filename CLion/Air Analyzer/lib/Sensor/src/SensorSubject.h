#ifndef SENSORBSUBJECT_H
    #define SENSORBSUBJECT_H

    #include <Arduino.h>

    #include "SensorObserver.h"

    class SensorSubject {
        public:
            virtual void addObserver(SensorObserver* observer) = 0;
            virtual void removeObserver(SensorObserver* observer) = 0;
            virtual void notify()  = 0;
            virtual ~SensorSubject() = default;
    };
#endif