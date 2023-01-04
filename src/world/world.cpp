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
    std::vector<glm::vec4> colors;

    for (int x = 0; x < requestedTileCount; x++) {
        for (int y = 0; y < requestedTileCount; y++) {

            // generate noise
            // normalize to range 0 - 1
            float noise = noiseGenerator.GetNoise((float)x, (float)y);
            noise = (noise + 1) / 2;

            // pick biome
            auto tileType = Tile::GRASS;

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

            colors.emplace_back(glm::vec4(noise * 0.5, noise, noise * 0.1, 0.9f));
        }
    }

    // set world variables
    tileCount = transformations.size();

    // send to renderer
    global.renderer->prepareTiles(transformations, offsets, colors);

    printf("Done (Size: %i)!\n", tileCount);
}
