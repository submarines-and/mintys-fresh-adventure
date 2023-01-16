#define TINYOBJLOADER_IMPLEMENTATION

#include "model.system.h"
#include "components/model.component.h"
#include "components/transform.component.h"
#include "glm/gtc/matrix_transform.hpp"
#include "global.h"
#include <tiny_obj_loader.h>

ModelSystem::ModelSystem() : shader(Shader("shaders/model.vert", "shaders/model.frag")) {}

void ModelSystem::entityAdded(Entity entity)
{
    auto& modelData = global.ecs->getComponent<ModelComponent>(entity);

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, modelData.modelFilePath, "obj");

    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Error: %s\n", err.c_str());
        return;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;

    for (int s = 0; s < (int)shapes.size(); s++) {
        size_t indexOffset = 0;

        for (int f = 0; f < (int)shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
                vertices.emplace_back(glm::vec3(
                    attributes.vertices[3 * idx.vertex_index + 0],
                    attributes.vertices[3 * idx.vertex_index + 1],
                    attributes.vertices[3 * idx.vertex_index + 2]));

                normals.emplace_back(
                    attributes.normals[3 * idx.normal_index + 0],
                    attributes.normals[3 * idx.normal_index + 1],
                    attributes.normals[3 * idx.normal_index + 2]);

                if (materials.size() > 0) {
                    colors.emplace_back(
                        materials[shapes[s].mesh.material_ids[f]].diffuse[0],
                        materials[shapes[s].mesh.material_ids[f]].diffuse[1],
                        materials[shapes[s].mesh.material_ids[f]].diffuse[2]);
                }
            }

            indexOffset += fv;
        }
    }

    // Create buffers and arrays
    GLuint vbo[3];
    glGenBuffers(3, vbo);
    glGenVertexArrays(1, &modelData.vao);
    glBindVertexArray(modelData.vao);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ModelSystem::update()
{
    shader.start();

    for (auto entity : entities) {
        auto transformComponent = global.ecs->getComponent<TransformComponent>(entity);
        auto model = global.ecs->getComponent<ModelComponent>(entity);

        shader.setMat4("projection", global.camera->getProjectionMatrix());
        shader.setMat4("view", global.camera->getViewMatrix());

        shader.setVec3("lightDirection", glm::vec3(0.3f, -1.0f, 0.5f));
        shader.setVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.8f));
        shader.setVec2("lightBias", glm::vec2(0.3f, 0.8f));

        // render the loaded model
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, transformComponent.position);
        transform = glm::scale(transform, glm::vec3(transformComponent.size, 1.0f));
        shader.setMat4("transform", transform);

        glBindVertexArray(model.vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 10192, 8);
        glDisable(GL_CULL_FACE);
    }

    shader.stop();
}
