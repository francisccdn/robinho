#pragma once

#include <exception>

class gpioInitException : public std::exception
{
public:
    const char *what() const throw()
    {
        return "Failed to initialize Pi GPIO";
    }
};