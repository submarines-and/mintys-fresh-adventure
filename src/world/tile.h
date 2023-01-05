#pragma once
#include "gfx/shader.h"
#include <glm/glm.hpp>

class TileAtlas {
public:
    const char* spritePath = "assets/terrain.png";
    Shader::ShaderType shaderKey = Shader::TILE;
    glm::vec2 atlasSize = glm::vec2(20, 20);
    glm::vec2 tileSize = glm::vec2(32, 32);
};