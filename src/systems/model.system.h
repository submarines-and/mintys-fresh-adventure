#pragma once
#include "ecs/system.h"
#include "gfx/shader.h"
#include <map>
#include <memory>
#include <vector>

class ModelSystem : public System {
private:
    Shader shader;

    /** Shared model data */
    struct ModelFile {
        unsigned int vao;
        unsigned int transformationVbo;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> colors;
    };

    std::map<const char*, std::shared_ptr<ModelFile>> modelCache;

private:
    std::shared_ptr<ModelFile> loadModel(const char* filePath);

public:
    ModelSystem();
    ~ModelSystem();

    void entityAdded(Entity entity);
    void update();
};