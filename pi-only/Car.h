#pragma once

#include <pigpio.h>

class Car
{
private:
    int wheels[4];

public:
    Car(int, int, int, int);
    virtual ~Car();

    void foward();
    void turn(int dir);
    void stop();
};