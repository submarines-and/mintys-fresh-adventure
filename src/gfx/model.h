#pragma once
#include <vector>
#include "opengl.h"

struct Model {
    GLuint id;
    GLuint shaderId;

    GLsizei vertexCount;
};

class ModelLoader {
private:
    std::vector<GLuint> vaos;
    std::vector<GLuint> vbos;

public:
    ~ModelLoader();

    Model load(std::vector<float> vertices, std::vector<int> indices);
    void render(Model model);
};