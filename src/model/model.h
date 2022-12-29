#pragma once
#include "gfx/shader.h"
#include "mesh.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

class Model {
public:
    Model(const char* path);
    void draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> cache;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int textureFromFile(const char* path, const std::string& directory);
};