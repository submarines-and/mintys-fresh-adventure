#include "noise.h"

std::vector<float> Noise::generateNoiseMap(int offsetX, int offsetY, int height, int width)
{
    std::vector<float> noiseValues;
    std::vector<float> normalizedNoiseValues;
    std::vector<int> p = getPermutationVector();

    float amp = 1;
    float freq = 1;
    float maxPossibleHeight = 0;
    int octaves = 5;
    float noiseScale = 64;
    float persistence = 0.5;
    float lacunarity = 2;

    for (int i = 0; i < octaves; i++) {
        maxPossibleHeight += amp;
        amp *= persistence;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            amp = 1;
            freq = 1;
            float noiseHeight = 0;
            for (int i = 0; i < octaves; i++) {
                float xSample = (x + offsetX * (width - 1)) / noiseScale * freq;
                float ySample = (y + offsetY * (height - 1)) / noiseScale * freq;

                float perlinValue = getNoise(xSample, ySample, p);
                noiseHeight += perlinValue * amp;

                // Lacunarity  --> Increase in frequency of octaves
                // Persistence --> Decrease in amplitude of octaves
                amp *= persistence;
                freq *= lacunarity;
            }

            noiseValues.push_back(noiseHeight);
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Inverse lerp and scale values to range from 0 to 1
            normalizedNoiseValues.push_back((noiseValues[x + y * width] + 1) / maxPossibleHeight);
        }
    }

    return normalizedNoiseValues;
}