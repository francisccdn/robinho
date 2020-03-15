#pragma once

#include <pigpio.h>
#include "gpioInitException.h"

class Car
{
private:
    int wheels[4];
    bool on;

public:
    Car(int, int, int, int);
    virtual ~Car();

    void isOn();
    void foward();
    void turn(int dir);
    void stop();
};