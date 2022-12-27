#pragma once
#include "opengl.h"

class Texture {
public:
    GLuint id;

    Texture(const char* filepath);
    ~Texture();
};