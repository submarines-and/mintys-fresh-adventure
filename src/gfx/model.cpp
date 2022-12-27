#include "model.h"

Model::Model(std::vector<float> vertices, std::vector<int> indices, const char* vertexPath, const char* fragmentPath, const char* texturePath)
    : shader(Shader(vertexPath, fragmentPath)),
      texture(Texture(texturePath))
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

    // vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind all
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::~Model()
{
    printf("Deleting model %i\n", id);

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
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(id);

    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shader.stop();
}
