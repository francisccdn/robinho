#pragma once

#include <pigpio.h>
#include "gpioInitException.h"

enum Directions
{
    LEFT = 0,
    RIGHT = 1
};

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