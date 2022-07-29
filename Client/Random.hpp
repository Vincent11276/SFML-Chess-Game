#pragma once

#include "SFML/Config.hpp"
#include <string>
#include <cstdlib>
#include <limits>


class Random
{
public:
    Random() = delete;

    static int getInt(int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max())
    {
        return min + (rand() % (max - min + 1)); 
    }

    static float getFloat(float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max())
    {
        return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
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
            c = (char)(Random::getInt(48, 122));
        }
        return randomString;
    }
};