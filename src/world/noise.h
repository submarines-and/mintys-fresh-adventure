#pragma once
#include "util/math.h"
#include <FastNoiseLite.h>

class Noise {
private:
    FastNoiseLite generator;

public:
    Noise()
    {
        generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        auto seed = rand() % RAND_MAX + 1;
        generator.SetSeed(seed);
    }

    float next(int x, int y)
    {
        float noise = generator.GetNoise((float)x, (float)y);

        // clamp to 0-1
        return Math::normalize(noise, 1, -1, 1);
    }
};