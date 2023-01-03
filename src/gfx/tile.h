#pragma once
#include "shader.h"
#include <glm/glm.hpp>

class TileAtlas {
public:
    const char* spritePath = "assets/terrain.png";
    Shader::ShaderType shaderKey = Shader::TILE;

    glm::vec2 size = glm::vec2(20, 20);
};

class Tile {
public:
    enum TileType {
        GRASS = 0,
        STUMP = 139,
        WATER = 161,
    };

public:
    TileType type = GRASS;

    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(32, 32);
    float rotation = 0.0f;

public:
    glm::vec2 getOffset(TileAtlas atlas);
};