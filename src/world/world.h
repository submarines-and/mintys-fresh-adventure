#pragma once
#include "gfx/opengl.h"
#include "noise.h"
#include <glm/glm.hpp>
#include <vector>
#include "gfx/shader.h"

struct WorldChunk {
    GLuint id;
    int x, y;
    bool generated = false;
};

class World {
private:
    Shader shader;

    int chunkWidth = 100;
    int chunkHeight = 100;

    float meshHeight = 10.0f;
    float waterHeight = 0.1f;
    int renderDistance = chunkHeight * 2;

    std::vector<WorldChunk> chunks;
    Noise noise;

    /** shared*/
    std::vector<int> sharedIndices;

private:
    void generateWorldChunk(GLuint& VAO, int xOffset, int yOffset);

    std::vector<int> generateIndices();
    std::vector<float> generateVertices(const std::vector<float>& noiseMap);
    std::vector<glm::vec3> generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices);

public:
    World(int numberOfChunks);
    ~World();
    void render();
};