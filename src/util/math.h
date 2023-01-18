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

    static float random(float min, float max)
    {
        return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
    }
};