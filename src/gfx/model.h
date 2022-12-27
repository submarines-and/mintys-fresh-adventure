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
    std::vector<GLuint> shaders;

    const char* vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

public:
    ~ModelLoader();

    Model load(std::vector<float> vertices);
    void render(Model model);
};