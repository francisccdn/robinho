#include "Car.h"

Car::Car(int w1, int w2, int w3, int w4)
{
    gpioInitialise();
    
    wheels[0] = w1;
    wheels[1] = w2;
    wheels[2] = w3;
    wheels[3] = w4;

    gpioSetMode(wheels[0], PI_OUTPUT);
    gpioSetMode(wheels[1], PI_OUTPUT);
    gpioSetMode(wheels[2], PI_OUTPUT);
    gpioSetMode(wheels[3], PI_OUTPUT);

    stop();
}

Car::~Car()
{
    gpioTerminate();
}

void Car::foward()
{
    gpioWrite(wheels[0], 1);
    gpioWrite(wheels[1], 0);
    gpioWrite(wheels[2], 1);
    gpioWrite(wheels[3], 0);
}

void Car::turn(int dir)
{
    if(dir)
    {
        gpioWrite(wheels[0], 1);
        gpioWrite(wheels[1], 0);
        gpioWrite(wheels[2], 0);
        gpioWrite(wheels[3], 1);
    }
    else
    {
        gpioWrite(wheels[0], 0);
        gpioWrite(wheels[1], 1);
        gpioWrite(wheels[2], 1);
        gpioWrite(wheels[3], 0);
    }
}

void Car::stop()
{
    gpioWrite(wheels[0], 1);
    gpioWrite(wheels[1], 1);
    gpioWrite(wheels[2], 1);
    gpioWrite(wheels[3], 1);
}