#ifndef ABSTRACTOBSERVER_H
    #define ABSTRACTOBSERVER_H

    #include <Arduino.h>

    class AbstractObserver {
        public:
            virtual void update() = 0;
    };
#endif