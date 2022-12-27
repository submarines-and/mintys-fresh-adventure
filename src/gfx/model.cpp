#include "model.h"

Model::Model(std::vector<float> vertices, std::vector<int> indices, const char* vertexPath, const char* fragmentPath) : shader(Shader(vertexPath, fragmentPath))
{
    vertexCount = static_cast<GLsizei>(indices.size());

    // vertex array
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    vaos.emplace_back(id);

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
}

Model::~Model()
{
    printf("Deleting data for model %i\n", id);

    for (auto id : vaos) {
        glDeleteVertexArrays(1, &id);
    }

    for (auto id : vbos) {
        glDeleteBuffers(1, &id);
    }
}

void Model::render()
{
    shader.start();

    glBindVertexArray(id);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.stop();
}
