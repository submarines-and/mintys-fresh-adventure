#include "biome.h"

Biome::Biome(float meshHeight, float waterHeight) : meshHeight(meshHeight), waterHeight(waterHeight){};

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

Biome::TerrainType Biome::getTerrainType(float vertexHeight)
{
    if (vertexHeight <= meshHeight * waterHeight * 0.5f) {
        return DEEP_WATER;
    }
    else if (vertexHeight <= meshHeight * waterHeight) {
        return SHALLOW_WATER;
    }
    else if (vertexHeight <= meshHeight * 0.15f) {
        return SAND;
    }
    else if (vertexHeight <= meshHeight * 0.3f) {
        return GRASS;
    }
    else if (vertexHeight <= meshHeight * 0.4f) {
        return SLOPE;
    }
    else if (vertexHeight <= meshHeight * 0.5f) {
        return MOUNTAIN;
    }
    else if (vertexHeight <= meshHeight * 0.8f) {
        return ROCK;
    }
    else {
        return SNOW;
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