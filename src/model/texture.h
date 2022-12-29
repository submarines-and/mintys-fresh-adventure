#pragma once
#include "gfx/opengl.h"
#include <string>

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};