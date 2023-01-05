#pragma once
#include "gfx/opengl.h"
#include "noise.h"
#include <glm/glm.hpp>
#include <vector>

class World {
private:
    float meshHeight = 32.0f;
    float waterHeight = 0.1f;
    int renderDistance = 3;
    int xMapChunks = 10;
    int yMapChunks = 10;
    int chunkWidth = 127;
    int chunkHeight = 127;
    int gridPosX = 0;
    int gridPosY = 0;
    float originX = (chunkWidth * xMapChunks) / 2 - chunkWidth / 2;
    float originY = (chunkHeight * yMapChunks) / 2 - chunkHeight / 2;
    int nIndices = chunkWidth * chunkHeight * 6;

    std::vector<GLuint> chunks;
    Noise noise;

    struct TerrainColor {
        TerrainColor(float height, glm::vec3 color) : height(height), color(color) {}
        float height;
        glm::vec3 color;
    };

private:
    void generateWorldChunk(GLuint& VAO, int xOffset, int yOffset);

    std::vector<int> generateIndices();
    std::vector<float> generateVertices(const std::vector<float>& noiseMap);
    std::vector<float> generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices);
    std::vector<float> generateBiome(const std::vector<float>& vertices);
    glm::vec3 normalizeColor(int r, int g, int b);

public:
    World();
    void render();
};