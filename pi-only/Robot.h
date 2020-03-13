#pragma once

#include <pigpio.h>
#include <unistd.h>
#include <chrono>
#include "Car.h"

#define TURN_TIME 5

class Robot : public Car
{
private:
    int claw[4];
    int n_turns = 0;

    void claw_stop();
    void claw_up();
    void claw_down();

public:
    Robot(int w1, int w2, int w3, int w4, int c1, int c2, int c3, int c4);
    ~Robot();

    void grab();
};