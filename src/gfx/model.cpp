#include "model.h"

ModelLoader::~ModelLoader()
{
    for (auto id : vaos) {
        glDeleteVertexArrays(1, &id);
    }

    for (auto id : vbos) {
        glDeleteBuffers(1, &id);
    }
}

Model ModelLoader::load(std::vector<float> vertices)
{
    Model model{
        .vertexCount = static_cast<GLsizei>(vertices.size() / 3),
    };

    // vertex array
    glGenVertexArrays(1, &model.id);

    glBindVertexArray(model.id);
    vaos.emplace_back(model.id);

    // vertex buffer
    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    vbos.emplace_back(vboId);

    // store vertex data in attribute slot 0
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