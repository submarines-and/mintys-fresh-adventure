#include "world.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

World::World()
{
    srand((unsigned int)time(NULL));

    int seed = rand() % RAND_MAX + 1;

    noiseGenerator.SetSeed(seed);
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

void World::generate(int requestedTileCount, TileAtlas atlas)
{
    printf("Generating world...\n");
    this->atlas = atlas;

    std::vector<glm::mat4> transformations;
    std::vector<glm::vec2> offsets;

    for (int x = 0; x < requestedTileCount; x++) {
        for (int y = 0; y < requestedTileCount; y++) {

            // generate noise
            auto noise = noiseGenerator.GetNoise((float)x, (float)y);

            auto tileType = Tile::GRASS;
            if (noise > -0.5) {
            }
            if (noise > 0.8) {
            }

            // map to tiles
            Tile tile{
                .type = tileType,
                .position = glm::vec2(x * atlas.tileSize.x, y * atlas.tileSize.y),
                .size = atlas.tileSize,
            };

            // set world variables
            width = tile.position.x;
            height = tile.position.y;

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(tile.position, 0.0f));
            transform = glm::translate(transform, glm::vec3(0.5f * tile.size.x, 0.5f * tile.size.y, 0.0f));
            transform = glm::rotate(transform, glm::radians(tile.rotation + 180), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform, glm::vec3(-0.5f * tile.size.x, -0.5f * tile.size.y, 0.0f));
            transform = glm::scale(transform, glm::vec3(tile.size, 1.0f));

            transformations.emplace_back(transform);
            offsets.emplace_back(tile.getOffset(atlas));
        }
    }

    // set world variables
    tileCount = transformations.size();

    // send to renderer
    global.renderer->prepareTiles(transformations, offsets);

    printf("Done (Size: %i)!\n", tileCount);
}
