#pragma once
#include <random>

class Math {
public:
    static float normalize(float input, float min, float max)
    {
        return (input - min) / (max - min);
    }

    static int random(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }
};