#include "biome.h"
#include <iostream>

Biome::Biome(float rainfall, float temperature) : rainfall(rainfall), temperature(temperature)
{
    if (temperature < 0.2) {
        type = TUNDRA;
    }
    else if (rainfall < 0.2 && temperature < 0.4) {
        type = PLAINS;
    }
    else if (rainfall < 0.2) {
        type = DESERT;
    }
    else if (temperature < 0.5 && rainfall < 0.4) {
        type = GHOSTLANDS;
    }
    else if (temperature < 0.5 && rainfall < 0.6) {
        type = FOREST;
    }
    else if (temperature < 0.5) {
        type = SWAMP;
    }
    else if (rainfall < 0.4) {
        type = SAVANNA;
    }
    else if (rainfall < 0.8) {
        type = RAIN_FOREST;
    }
    else {
        type = WATER;
    }
};

glm::vec4 Biome::getColor()
{
    glm::vec3 color(0.0f);

    switch (type) {
    case TUNDRA:
        color = glm::vec3(1, 1, 1);
        break;

    case BOREAL_FOREST:
    case FOREST:
    case RAIN_FOREST:
        color = glm::vec3(0.5f, 1, 0.1f);
        break;

    case SWAMP:
    case GHOSTLANDS:
        color = glm::vec3(0.1f, 0.8, 0.3f);
        break;

    case PLAINS:
    case SAVANNA:
    case DESERT:
        color = glm::vec3(1, 1, 0);

    case WATER:
    default:
        color = glm::vec3(0, 0, 1.0f);
        break;
    }

    float modifier = rainfall * temperature;
    return glm::vec4(color, 0.9f);
}
