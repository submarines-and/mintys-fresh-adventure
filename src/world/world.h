#pragma once
#include "gfx/opengl.h"
#include "noise.h"
#include <glm/glm.hpp>
#include <vector>

class World {
private:
    int numberOfChunks = 10;
    int chunkWidth = 100;
    int chunkHeight = 100;

    float meshHeight = 5.0f;
    float waterHeight = 0.1f;
    int renderDistance = 3;



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