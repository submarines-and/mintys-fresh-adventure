#pragma once

class Sprite {
private:
    unsigned int id;

public:
    Sprite(const char* filepath);
    ~Sprite();

    void bind();
};