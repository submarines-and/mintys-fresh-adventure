#pragma once
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <vector>

class World {
private:
    FastNoiseLite noiseGenerator;

    enum TileType {
        GRASS = 1,
        STUMP = 140,
        WATER = 162,
    };

public:
    World();
    void generate(int tileCount);

    std::vector<glm::vec3> tiles;
};