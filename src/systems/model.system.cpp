#define TINYOBJLOADER_IMPLEMENTATION

#include "model.system.h"
#include "components/model.component.h"
#include "components/transform.component.h"
#include "glm/gtc/matrix_transform.hpp"
#include "global.h"
#include <tiny_obj_loader.h>

ModelSystem::ModelSystem() : shader(Shader("shaders/model.vert", "shaders/model.frag")) {}

ModelSystem::~ModelSystem()
{
    for (auto data : modelCache) {
        glDeleteVertexArrays(1, &data.second->vao);
        glDeleteBuffers(1, &data.second->transformationVbo);
    }

    modelCache.clear();
}

void ModelSystem::entityAdded(Entity entity)
{
    // get or create model
    auto& modelComponent = global.ecs->getComponent<ModelComponent>(entity);
    loadModel(modelComponent.modelFilePath);
}

std::shared_ptr<ModelSystem::ModelFile> ModelSystem::loadModel(const char* filePath)
{
    if (modelCache.count(filePath)) {
        return modelCache[filePath];
    }

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, filePath, "obj");

    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Error: %s\n", err.c_str());
        return nullptr;
    }

    auto data = std::make_shared<ModelFile>(ModelFile());

    for (int s = 0; s < (int)shapes.size(); s++) {
        size_t indexOffset = 0;

        for (int f = 0; f < (int)shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
                data->vertices.emplace_back(glm::vec3(
                    attributes.vertices[3 * idx.vertex_index + 0],
                    attributes.vertices[3 * idx.vertex_index + 1],
                    attributes.vertices[3 * idx.vertex_index + 2]));

                data->normals.emplace_back(
                    attributes.normals[3 * idx.normal_index + 0],
                    attributes.normals[3 * idx.normal_index + 1],
                    attributes.normals[3 * idx.normal_index + 2]);

                if (materials.size() > 0) {
                    data->colors.emplace_back(
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
    glGenVertexArrays(1, &data->vao);
    glBindVertexArray(data->vao);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(glm::vec3), &data->vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, data->normals.size() * sizeof(glm::vec3), &data->normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, data->colors.size() * sizeof(glm::vec3), &data->colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    // transformations (4 slots)
    glGenBuffers(1, &data->transformationVbo);
    glBindBuffer(GL_ARRAY_BUFFER, data->transformationVbo);
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(3 + i, 1);
    }

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    modelCache.emplace(filePath, data);
    return modelCache[filePath];
}

void ModelSystem::update()
{
    shader.start();
    shader.setMat4("projection", global.camera->getProjectionMatrix());
    shader.setMat4("view", global.camera->getViewMatrix());
    shader.setVec3("lightDirection", glm::vec3(0.3f, -1.0f, 0.5f));
    shader.setVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.8f));
    shader.setVec2("lightBias", glm::vec2(0.3f, 0.8f));

    // create transformation vectors
    std::map<const char*, std::vector<glm::mat4>> transformations;

    for (auto entity : entities) {
        auto transformComponent = global.ecs->getComponent<TransformComponent>(entity);
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, transformComponent.position);
        transform = glm::scale(transform, transformComponent.scale);

        // rotate
        transform = glm::rotate(transform, glm::radians(transformComponent.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(transformComponent.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(transformComponent.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        auto modelComponent = global.ecs->getComponent<ModelComponent>(entity);
        transformations[modelComponent.modelFilePath].emplace_back(transform);
    }

    // render each models using the transformations
    for (auto object : modelCache) {

        if (!transformations.count(object.first) || !transformations[object.first].size()) {
            printf("No transformations found for model: %s\n", object.first);
            continue;
        }

        // buffer positions
        glBindBuffer(GL_ARRAY_BUFFER, object.second->transformationVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformations[object.first].size(), &transformations[object.first][0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // draw
        glBindVertexArray(object.second->vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, object.second->vertices.size(), transformations[object.first].size());
    }

    glBindVertexArray(0);
    shader.stop();
}
