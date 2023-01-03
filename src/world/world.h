#pragma once
#include <FastNoiseLite.h>
#include <glm/glm.hpp>
#include <vector>

class World {
private:
    FastNoiseLite noiseGenerator;

    enum TileType {
        GRASS,
        WATER,
        SAND,
        SNOW,
    };

    enum Obstacles {
        TREE,
        ROCK,
    };

    enum Teasures {
        SEASHELL,
        FLOWER,
        BREAD,
    };

public:
    World();
    void generate(int tileCount);

    std::vector<glm::vec3> tiles;
};