#include "world.h"

World::World()
{
    srand((unsigned int)time(NULL));

    int seed = rand() % RAND_MAX + 1;

    noiseGenerator.SetSeed(seed);
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

void World::generate(int tileCount)
{
    tiles = std::vector<Tile>(tileCount ^ 2);

    for (int x = 0; x < tileCount; x++) {
        for (int y = 0; y < tileCount; y++) {

            // generate noise
            auto noise = noiseGenerator.GetNoise((float)x, (float)y);

            auto tileType = Tile::WATER;
            if (noise > -0.5) {
                tileType = Tile::GRASS;
            }
            if (noise > 0.8) {
                tileType = Tile::STUMP;
            }

            // map to tiles
            tiles.push_back(Tile{
                .type = tileType,
                .position = glm::vec2(x * 32, y * 32),
            });
        }
    }
}
