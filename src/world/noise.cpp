#include "noise.h"
#include "util/math.h"

Noise::Noise()
{
    int seed = Math::random(0, RAND_MAX);
    generator.SetSeed(seed);
    generator.SetFractalOctaves(5);
    generator.SetFrequency(0.01f);
    generator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

std::vector<float> Noise::generateNoiseMap(int offsetX, int offsetY, int height, int width)
{
    std::vector<float> returnList;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // add offsets
            float noiseX = x + offsetX * width - offsetX;
            float noiseY = y + offsetY * height - offsetY;

            float noiseValue = Math::normalize(generator.GetNoise(noiseX, noiseY), -1.0f, 1.0f);
            returnList.emplace_back(noiseValue);
        }
    }

    return returnList;
}