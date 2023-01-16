#include "noise.h"

Noise::Noise()
{
    int seed = rand() % RAND_MAX;
    generator.SetSeed(seed);
    generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

float Noise::normalize(float input, float min, float max)
{
    return (input - min) / (max - min);
}

std::vector<float> Noise::generateNoiseMap(int offsetX, int offsetY, int height, int width)
{
    std::vector<float> returnList;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // add offsets
            float noiseX = x + offsetX * width - offsetX;
            float noiseY = y + offsetY * height - offsetY;

            float noiseValue = normalize(generator.GetNoise(noiseX, noiseY), 0, 1);
            returnList.emplace_back(noiseValue);
        }
    }

    return returnList;
}