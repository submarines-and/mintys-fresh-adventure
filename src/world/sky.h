#pragma once
#include "gfx/shader.h"

class Sky {
private:
    unsigned int vao;
    unsigned int textureId;
    Shader shader;

public:
    Sky();
    ~Sky();

    void render();
};