#ifndef ABSTRACTOBSUBJECT_H
#define ABSTRACTOBSUBJECT_H

#include <Arduino.h>
#include <AbstractObserver.h>

class AbstractSubject {
    public:
        virtual void addObserver(AbstractObserver* observer) = 0;
        virtual void removeObserver(AbstractObserver* observer) = 0;
        virtual void notify() = 0;
};
#endif