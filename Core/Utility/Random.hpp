#pragma once

#include "SFML/Config.hpp"
#include <stdint.h>
#include <string>
#include <cstdlib>
#include <limits>


class Random
{
public:
    Random() = delete;

    template <typename T>
    static T getWholeNumber(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
    {
        // causes division by 0 problem
        // return min + (rand() % (max - min + 1)); 

        // temp fix:
        return min + (rand() % (max - min + 0)); 
    }

    template <typename T>
    static float getDecimalNumber(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
    {
        return min + (T)(rand()) / ((T)(RAND_MAX / (max - min)));
    }

    static bool getBool()
    {
        return rand() % 2 == 0;
    }

    static std::string getAlphaNumericString(uint32_t length)
    {
        std::string randomString;
        randomString.resize(length);

        for (auto& c: randomString)
        {
            c = (char)(Random::getWholeNumber<uint8_t>(48, 122));
        }
        return randomString;
    }
};