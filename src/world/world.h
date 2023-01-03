#pragma once
#include "gfx/tile.h"
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <vector>

class World {
private:
    FastNoiseLite noiseGenerator;

public:
    World();
    void generate(int tileCount);

    std::vector<Tile> tiles;
};