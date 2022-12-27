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

Model ModelLoader::load(std::vector<float> vertices, std::vector<int> indices)
{
    Model model{
        .vertexCount = static_cast<GLsizei>(indices.size()),
    };

    // vertex array
    glGenVertexArrays(1, &model.id);
    glBindVertexArray(model.id);
    vaos.emplace_back(model.id);

    // vertex buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    vbos.emplace_back(vertexBufferId);

    // index buffer
    GLuint indexBufferId;
    glGenBuffers(1, &indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    vbos.emplace_back(indexBufferId);

    // store vertex data in attribute slot 0
    int attributeNumber = 0;
    glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attributeNumber);

    // unbind all
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return model;
}

void ModelLoader::render(Model model)
{
    glUseProgram(model.shaderId);
    glBindVertexArray(model.id);
    glDrawElements(GL_TRIANGLES, model.vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}