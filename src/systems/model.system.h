#pragma once
#include "ecs/system.h"
#include "gfx/shader.h"
#include <map>
#include <vector>

class ModelSystem : public System {
private:
    Shader shader;

    struct ModelFile {
        unsigned int vao;
        unsigned int transformationVbo;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> colors;
        std::vector<glm::mat4> transformations;
    };

    std::map<const char*, ModelFile*> modelCache;

private:
    ModelFile* loadModel(const char* filePath);

public:
    ModelSystem();
    void entityAdded(Entity entity);
    void update();
};