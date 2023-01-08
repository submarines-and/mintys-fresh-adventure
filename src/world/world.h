#pragma once
#include "biome.h"
#include "gfx/opengl.h"
#include "gfx/shader.h"
#include "noise.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>

struct WorldChunk {
    GLuint id;
    int x, y;
    Biome::BiomeType biomeType;
    bool generated = false;
};

class World {
private:
    Shader shader;

    int numberOfChunks = 1;
    int chunkWidth = 100;
    int chunkHeight = 100;

    float meshHeight = 10.0f;
    float waterHeight = 0.1f;
    Biome biomeGen;

    int renderDistance = chunkHeight * 2;

    std::vector<WorldChunk> chunks;
    Noise noise;

    /** shared*/
    std::vector<int> sharedIndices;

private:
    void generateWorldChunk(WorldChunk& chunk);

    std::vector<int> generateIndices();
    std::vector<float> generateVertices(const std::vector<float>& noiseMap);
    std::vector<glm::vec3> generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices);

public:
    World(int numberOfChunks);
    ~World();
    void render();
    float getTerrainHeight(glm::vec2 position);
};