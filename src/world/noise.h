#pragma once
#include <FastNoiseLite.h>
#include <vector>

class Noise {
private:
    FastNoiseLite generator;
    float normalize(float input, float min, float max);

public:
    Noise();
    std::vector<float> generateNoiseMap(int offsetX, int offsetY, int height, int width);
};