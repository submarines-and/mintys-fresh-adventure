#include "biome.h"

Biome::Biome(float rainfall, float temperature, float meshHeight, float waterHeight) : meshHeight(meshHeight), waterHeight(waterHeight)
{
    this->type = getBiomeType(rainfall, temperature);
};

BiomeType Biome::getBiomeType(float rainfall, float temperature)
{
    return TUNDRA;
}

glm::vec3 Biome::getColorAtPoint(float noise)
{
    glm::vec3 color;

    if (!colors.count(type)) {
        // missing biome type
        color = glm::vec3(255, 0, 255);
    }
    else {
        if (noise <= meshHeight * waterHeight * 0.5f) {
            color = colors[type][DEEP_WATER];
        }
        else if (noise <= meshHeight * waterHeight) {
            color = colors[type][SHALLOW_WATER];
        }
        else if (noise <= meshHeight * 0.15f) {
            color = colors[type][SAND];
        }
        else if (noise <= meshHeight * 0.3f) {
            color = colors[type][GRASS];
        }
        else if (noise <= meshHeight * 0.4f) {
            color = colors[type][SLOPE];
        }
        else if (noise <= meshHeight * 0.5f) {
            color = colors[type][MOUNTAIN];
        }
        else if (noise <= meshHeight * 0.8f) {
            color = colors[type][ROCK];
        }
        else {
            color = colors[type][SNOW];
        }
    }

    // normalize and add transparency
    return glm::vec3(color.r / 255.0, color.g / 255.0, color.b / 255.0);
}

std::vector<glm::vec3> Biome::getColorAtPoint(std::vector<float> noise)
{
    std::vector<glm::vec3> colors;

    for (auto i = 1.0f; i < noise.size(); i += 3) {
        colors.emplace_back(getColorAtPoint(noise[i]));
    }

    return colors;
}