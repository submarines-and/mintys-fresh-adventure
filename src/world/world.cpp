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
    tiles = std::vector<glm::vec3>(tileCount ^ 2);

    for (int x = 0; x < tileCount; x++) {
        for (int y = 0; y < tileCount; y++) {

            // generate noise
            auto noise = noiseGenerator.GetNoise((float)x, (float)y);

            auto tileId = WATER;
            if (noise < -0.9) {
                tileId = SNOW;
            }
            else if (noise < -0.5) {
                tileId = GRASS;
            }
            else if (noise < 0.) {
                tileId = SAND;
            }

            // map to tiles
            auto tile = glm::vec3(x, y, tileId);
            tiles.push_back(tile);
        }
    }
}
