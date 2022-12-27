#pragma once
#include <vector>
#include "opengl.h"
#include "shader.h"

class Model {
private:
    std::vector<GLuint> vaos;
    std::vector<GLuint> vbos;
    Shader shader;

public:
    GLuint id;
    GLsizei vertexCount;

    Model(std::vector<float> vertices, std::vector<int> indices, const char* vertexPath, const char* fragmentPath);
    ~Model();

    void render();
};