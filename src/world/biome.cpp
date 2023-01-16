#include "biome.h"

Biome::Biome(float meshHeight) : meshHeight(meshHeight){};

Biome::BiomeType Biome::getBiomeType(float rainfall, float temperature)
{
    if (rainfall < 1.1f && temperature < 0.2f) {
        return TUNDRA;
    }
    else if (rainfall < 0.4f && temperature < 0.4f) {
        return PLAINS;
    }
    else if (rainfall < 1.1f && temperature < 0.4f) {
        return SWAMP;
    }
    else if (rainfall < 0.6f && temperature < 0.8) {
        return FOREST;
    }
    else if (rainfall < 1.1f && temperature < 0.8) {
        return RAIN_FOREST;
    }
    else {
        return DESERT;
    }
}

glm::vec3 Biome::getTerrainColor(BiomeType biomeType, TerrainType terrainType)
{
    glm::vec3 color;

    // missing biome type
    if (!colors.count(biomeType)) {
        color = glm::vec3(255, 0, 255);
        printf("Missing biome %i, %i\n", biomeType, terrainType);
    }
    else {
        color = colors[biomeType][terrainType];
    }

    // normalize and add transparency
    return glm::vec3(color.r / 255.0, color.g / 255.0, color.b / 255.0);
}

Biome::TerrainType Biome::getTerrainType(float vertexHeight)
{
    for (auto h : heights) {
        if (vertexHeight <= h.second * meshHeight) {
            return h.first;
        }
    }

    // in case of bad config
    return SNOW;
}

float Biome::getTerrainHeight(Biome::TerrainType type)
{
    for (auto h : heights) {
        if (h.first == type) {
            return h.second;
        }
    }

    return 0.0f;
}
