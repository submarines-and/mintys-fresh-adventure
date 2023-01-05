#include "biome.h"
#include "util/math.h"
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
        color = glm::vec3(255, 255, 255);
        break;

    case BOREAL_FOREST:
        color = glm::vec3(90, 156, 113);
        break;

    case FOREST:
    case RAIN_FOREST:
        color = glm::vec3(49, 181, 25);
        break;

    case GHOSTLANDS:
        color = glm::vec3(140, 156, 90);
        break;

    case SWAMP:
        color = glm::vec3(113, 138, 30);
        break;

    case PLAINS:
    case SAVANNA:
        color = glm::vec3(169, 191, 25);
        break;

    case DESERT:
        color = glm::vec3(194, 164, 33);
        break;

    case WATER:
    default:
        color = glm::vec3(25, 78, 191);
        break;
    }

    // make gradient based on noise level
    color *= (rainfall + temperature);

    // normalize colors to 0-1 range
    return glm::vec4(Math::normalize(color.x, 255), Math::normalize(color.y, 255), Math::normalize(color.z, 255), 0.9f);
}
