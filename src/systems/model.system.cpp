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
    printf("Loading model %s\n", modelData.modelFilePath);

    std::vector<float> vertices;
    std::vector<int> indices;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    float MODEL_SCALE = 3;
    float MODEL_BRIGHTNESS = 6;

    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelData.modelFilePath, "obj");

    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Error: %s\n", err.c_str());
        return;
    }

    for (auto s = 0; s < (int)shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (auto f = 0; f < (int)shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (auto v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);

                if (materials.size() > 0) {
                    vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[0] * MODEL_BRIGHTNESS);
                    vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[1] * MODEL_BRIGHTNESS);
                    vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[2] * MODEL_BRIGHTNESS);
                }
            }
            index_offset += fv;
        }
    }

    GLuint vbo, ibo;

    // Create buffers and arrays
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &modelData.vao);

    // Bind vertices to VBO
    glBindVertexArray(modelData.vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Configure vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configure vertex normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configure vertex color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void ModelSystem::update()
{
    shader.start();

    for (auto entity : entities) {
        auto transformComponent = global.ecs->getComponent<TransformComponent>(entity);
        auto model = global.ecs->getComponent<ModelComponent>(entity);

        shader.setMat4("projection", global.camera->getProjectionMatrix());
        shader.setMat4("view", global.camera->getViewMatrix());

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
