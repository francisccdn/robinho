#include "Robot.h"

Robot::Robot(int w1, int w2, int w3, int w4, int c1, int c2, int c3, int c4)
    :   Car(w1, w2, w3, w4)
{
    claw[0] = c1;
    claw[1] = c2;
    claw[2] = c3;
    claw[3] = c4;

    gpioSetMode(claw[0], PI_OUTPUT);
    gpioSetMode(claw[1], PI_OUTPUT);
    gpioSetMode(claw[2], PI_OUTPUT);
    gpioSetMode(claw[3], PI_OUTPUT);

    claw_stop();
}

Robot::~Robot()
{
    gpioTerminate();
}

void Robot::claw_stop()
{
    gpioWrite(claw[0], 1);
    gpioWrite(claw[1], 1);
    gpioWrite(claw[2], 1);
    gpioWrite(claw[3], 1);
}

void Robot::claw_up()
{
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_stop = std::chrono::high_resolution_clock::now();

    const auto t_move = (std::chrono::milliseconds)2750;
    auto t_dur = t_stop - t_start;

    while(t_dur < t_move)
    {
        gpioWrite(claw[0], 0);
        gpioWrite(claw[1], 0);
        gpioWrite(claw[2], 1);
        gpioWrite(claw[3], 1);
        usleep(5000);

        gpioWrite(claw[0], 1);
        gpioWrite(claw[1], 0);
        gpioWrite(claw[2], 0);
        gpioWrite(claw[3], 1);
        usleep(5000);

        gpioWrite(claw[0], 1);
        gpioWrite(claw[1], 1);
        gpioWrite(claw[2], 0);
        gpioWrite(claw[3], 0);
        usleep(5000);

        gpioWrite(claw[0], 0);
        gpioWrite(claw[1], 1);
        gpioWrite(claw[2], 1);
        gpioWrite(claw[3], 0);
        usleep(5000);

        t_stop = std::chrono::high_resolution_clock::now();
        t_dur = t_stop - t_start;
    }
}

void Robot::claw_down()
{
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_stop = std::chrono::high_resolution_clock::now();

    const auto t_move = (std::chrono::milliseconds)2750;
    auto t_dur = t_stop - t_start;

    while(t_dur < t_move)
    {
        gpioWrite(claw[0], 0);
        gpioWrite(claw[1], 1);
        gpioWrite(claw[2], 1);
        gpioWrite(claw[3], 0);
        usleep(5000);

        gpioWrite(claw[0], 1);
        gpioWrite(claw[1], 1);
        gpioWrite(claw[2], 0);
        gpioWrite(claw[3], 0);
        usleep(5000);

        gpioWrite(claw[0], 1);
        gpioWrite(claw[1], 0);
        gpioWrite(claw[2], 0);
        gpioWrite(claw[3], 1);
        usleep(5000);

        gpioWrite(claw[0], 0);
        gpioWrite(claw[1], 0);
        gpioWrite(claw[2], 1);
        gpioWrite(claw[3], 1);
        usleep(5000);

        t_stop = std::chrono::high_resolution_clock::now();
        t_dur = t_stop - t_start;
    }
}

void Robot::grab()
{
    stop();
    claw_down();
    claw_up();
    claw_stop();
}