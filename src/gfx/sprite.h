#pragma once
#include "opengl.h"

class Sprite {
private:
    unsigned int id;

public:
    Sprite(const char* filepath);
    ~Sprite();

    void bind();
};