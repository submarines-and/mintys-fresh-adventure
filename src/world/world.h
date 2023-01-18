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

    Biome::BiomeType biomeType = Biome::FOREST;
    std::vector<float> heights;
    std::vector<Biome::TerrainType> terrain;
};

class World {
private:
    int numberOfChunks = 0;
    int chunkWidth = 100;
    int chunkHeight = 100;
    int renderDistance = chunkHeight * 2;

    float meshHeight = 12.0f;

    Shader shader;
    Biome biomeGen;
    Noise noise;

    std::vector<int> indices;
    std::vector<WorldChunk> chunks;

private:
    void generateWorldChunk(WorldChunk& chunk);
    std::vector<int> generateIndices();
    std::vector<glm::vec3> generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices);

    WorldChunk getWorldChunk(glm::vec3 position, glm::vec2 size);

public:
    int worldWidth = 0;
    int worldHeight = 0;

public:
    World(int numberOfChunks);
    ~World();
    void render();
    float getTerrainHeight(glm::vec3 position, glm::vec2 size);
    Biome::TerrainType getTerrainType(glm::vec3 position, glm::vec2 size);
};