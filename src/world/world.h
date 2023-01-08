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
    bool generated = false;

    Biome::BiomeType biomeType;
    std::vector<float> heights;
    std::vector<Biome::TerrainType> terrain;
};

class World {
private:
    Shader shader;

    int numberOfChunks = 100;
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
    std::vector<glm::vec3> generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices);

    WorldChunk getWorldChunk(glm::vec3 position, glm::vec2 size);

public:
    World(int numberOfChunks);
    ~World();
    void render();
    float getTerrainHeight(glm::vec3 position, glm::vec2 size);
    Biome::TerrainType getTerrainType(glm::vec3 position, glm::vec2 size);
};