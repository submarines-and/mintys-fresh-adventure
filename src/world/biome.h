#pragma once
#include <glm/glm.hpp>

enum BiomeType {
    TUNDRA,
    BOREAL_FOREST,
    PLAINS,
    SWAMP,
    FOREST,
    GHOSTLANDS,
    RAIN_FOREST,
    SAVANNA,
    DESERT,
    WATER,
};

class Biome {
private:
    BiomeType type;
    float rainfall;
    float temperature;

public:
    Biome(float rainfall, float temperature);
    glm::vec4 getColor();
};