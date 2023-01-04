#pragma once
#include "tile.h"
#include "noise.h"

class World {
private:
    Noise rainfallNoise;
    Noise temperatureNoise;

public:
    int tileCount = 0;
    int width, height = 0;
    TileAtlas atlas;

public:
    void generate(int requestedTileCount, TileAtlas atlas);
};