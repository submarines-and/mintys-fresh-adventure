#include "model.h"

ModelLoader::~ModelLoader()
{
    for (auto id : vaos) {
        glDeleteVertexArrays(1, &id);
    }

    for (auto id : vbos) {
        glDeleteBuffers(1, &id);
    }

    for (auto id : shaders) {
        glDeleteProgram(id);
    }
}

Model ModelLoader::load(std::vector<float> vertices)
{
    Model model{
        .vertexCount = static_cast<GLsizei>(vertices.size() / 3),
    };

    // vs
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // fs
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // shader program
    model.shaderId = glCreateProgram();
    glAttachShader(model.shaderId, vs);
    glAttachShader(model.shaderId, fs);
    glLinkProgram(model.shaderId);
    shaders.emplace_back(model.shaderId);

    glGetProgramiv(model.shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(model.shaderId, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // vertex array
    glGenVertexArrays(1, &model.id);

    GLuint vboId;
    glGenBuffers(1, &vboId);

    glBindVertexArray(model.id);
    vaos.emplace_back(model.id);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    vbos.emplace_back(vboId);

    // store data in slot 0
    int attributeNumber = 0;
    glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attributeNumber);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return model;
}

void ModelLoader::render(Model model)
{
    glUseProgram(model.shaderId);

    glBindVertexArray(model.id);
    glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
    glBindVertexArray(0);
}