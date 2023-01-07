#pragma once
#include <glm/glm.hpp>

class Sprite {
private:
    unsigned int id;
    unsigned int vao;

private:
    void loadFile(const char* filepath);
    void setupVao();

public:
    enum SpriteType {
        PLAYER = 0
    };

    glm::vec2 atlasSize = glm::vec2(1, 1);
    glm::vec2 atlasOffset = glm::vec2(0, 0);
    glm::vec3 worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 worldSize = glm::vec2(1.0f, 1.0f);

public:
    Sprite(const char* filepath);
    ~Sprite();

    void render();
};