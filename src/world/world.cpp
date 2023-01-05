#include "world.h"
#include "biome.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

void World::generate(int requestedTileCount, TileAtlas atlas)
{
    printf("Generating world...\n");
    this->atlas = atlas;

    std::vector<glm::mat4> transformations;
    std::vector<glm::vec2> offsets;
    std::vector<glm::vec4> colors;

    for (int x = 0; x < requestedTileCount; x++) {
        for (int y = 0; y < requestedTileCount; y++) {

            float rainfall = rainfallNoise.next(x, y);
            float temperature = temperatureNoise.next(x, y);
        
            auto position = glm::vec2(x * atlas.tileSize.x, y * atlas.tileSize.y);

            // set world variables
            width = position.x;
            height = position.y;

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(position, 0.0f));
            transform = glm::translate(transform, glm::vec3(0.5f * atlas.tileSize.x, 0.5f * atlas.tileSize.y, 0.0f));
            transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform, glm::vec3(-0.5f * atlas.tileSize.x, -0.5f * atlas.tileSize.y, 0.0f));
            transform = glm::scale(transform, glm::vec3(atlas.tileSize, 1.0f));

            transformations.emplace_back(transform);

            int tileType = 0;
            offsets.emplace_back(glm::vec2(((int)tileType % (int)atlas.atlasSize.x), (int)tileType / (int)atlas.atlasSize.y));

            Biome biome(rainfall, temperature);

            colors.emplace_back(biome.getColor());
        }
    }

    printf("Sending to GPU...\n");

    // set world variables
    tileCount = transformations.size();

    // send to renderer
    global.renderer->prepareTiles(transformations, offsets, colors);

    printf("Done (Size: %i)!\n", tileCount);
}
