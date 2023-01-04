#pragma once
#include <FastNoiseLite.h>

class World {
private:
    FastNoiseLite noiseGenerator;

public:
    int tileCount = 0;
    int width, height = 0;

public:
    World();
    void generate(int requestedTileCount);
};