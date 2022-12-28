#include "model.h"
#include "global.h"

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture attribute data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    glm::mat4 view = global.camera.getViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(global.camera.Zoom), (float)1280 / (float)720, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(id);

    // glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    shader.stop();
}
