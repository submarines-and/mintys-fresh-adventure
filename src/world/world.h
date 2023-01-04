#pragma once
#include <FastNoiseLite.h>
#include "tile.h"

class World {
private:
    FastNoiseLite noiseGenerator;

public:
    int tileCount = 0;
    int width, height = 0;
    TileAtlas atlas;

public:
    World();
    void generate(int requestedTileCount, TileAtlas atlas);
};