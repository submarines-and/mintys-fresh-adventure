#include "world.h"
#include "biome.h"
#include "gfx/shader.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>

World::World(int numberOfChunks)
{
    srand((unsigned)time(NULL));

    global.renderer->loadShader(Shader::TERRAIN, "shaders/terrain.vert", "shaders/terrain.frag");
    chunks = std::vector<WorldChunk>(numberOfChunks * numberOfChunks);
    sharedIndices = generateIndices();

    for (int y = 0; y < numberOfChunks; y++) {
        for (int x = 0; x < numberOfChunks; x++) {
            chunks[x + y * numberOfChunks] = WorldChunk{
                .x = x,
                .y = y,
            };
        }
    }
}

World::~World()
{
    printf("Deleting world \n");

    for (auto chunk : chunks) {
        glDeleteVertexArrays(1, &chunk.id);
    }

    chunks.clear();
    sharedIndices.clear();
}

void World::render()
{
    glm::mat4 projection = glm::perspective(glm::radians(global.camera->zoom), (float)global.width / (float)global.height, 0.1f, (float)chunkWidth * (renderDistance - 1.2f));
    glm::mat4 view = global.camera->getViewMatrix();

    Shader* objectShader = global.renderer->getShader(Shader::TERRAIN);
    objectShader->start();

    objectShader->setMat4("projection", projection);
    objectShader->setMat4("view", view);

    objectShader->setVec3("lightDirection", glm::vec3(0.3f, -1.0f, 0.5f));
    objectShader->setVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.8f));
    objectShader->setVec2("lightBias", glm::vec2(0.3f, 0.8f));

    // Render map chunks
    for (auto& chunk : chunks) {

        // Only render chunk if it's within render distance
        if (std::abs(chunk.x * chunkWidth - global.camera->position.x) <= renderDistance && std::abs(chunk.y * chunkHeight - global.camera->position.z) <= renderDistance) {

            // generate if missing
            if (!chunk.generated) {
                generateWorldChunk(chunk.id, chunk.x, chunk.y);
                chunk.generated = true;
            }

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * chunk.x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * chunk.y));
            objectShader->setMat4("transform", transform);

            // Terrain chunk
            glBindVertexArray(chunk.id);
            glDrawElements(GL_TRIANGLES, chunkWidth * chunkHeight * 6, GL_UNSIGNED_INT, 0);
        }
    }
}

void World::generateWorldChunk(GLuint& VAO, int xOffset, int yOffset)
{
    auto noiseMap = noise.generateNoiseMap(xOffset, yOffset, chunkHeight, chunkWidth);
    auto vertices = generateVertices(noiseMap);
    auto normals = generateNormals(sharedIndices, vertices);

    // biomes
    auto rainfall = (float)rand() / (float)RAND_MAX;
    auto temperature = (float)rand() / (float)RAND_MAX;

    Biome biome(rainfall, temperature, meshHeight, waterHeight);
    auto colors = biome.getColorAtPoint(vertices);

    GLuint VBO[3], EBO;

    // Create buffers and arrays
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sharedIndices.size() * sizeof(int), &sharedIndices[0], GL_STATIC_DRAW);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // Color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::vector<int> World::generateIndices()
{
    std::vector<int> indices;

    for (int y = 0; y < chunkHeight; y++)
        for (int x = 0; x < chunkWidth; x++) {
            int pos = x + y * chunkWidth;

            if (x == chunkWidth - 1 || y == chunkHeight - 1) {
                // Don't create indices for right or top edge
                continue;
            }
            else {
                // Top left triangle of square
                indices.push_back(pos + chunkWidth);
                indices.push_back(pos);
                indices.push_back(pos + chunkWidth + 1);

                // Bottom right triangle of square
                indices.push_back(pos + 1);
                indices.push_back(pos + 1 + chunkWidth);
                indices.push_back(pos);
            }
        }

    return indices;
}

std::vector<float> World::generateVertices(const std::vector<float>& noiseMap)
{
    std::vector<float> v;

    for (int y = 0; y < chunkHeight + 1; y++)
        for (int x = 0; x < chunkWidth; x++) {
            v.push_back(x);

            // Apply cubic easing to the noise
            float easedNoise = std::pow(noiseMap[x + y * chunkWidth] * 1.1f, 3);

            // Scale noise to match meshHeight
            // Pervent vertex height from being below WATER_HEIGHT
            v.push_back(std::fmax(easedNoise * meshHeight, waterHeight * 0.5f * meshHeight));
            v.push_back(y);
        }

    return v;
}

std::vector<glm::vec3> World::generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices)
{
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> verts;

    // Get the vertices of each triangle in mesh
    // For each group of indices
    for (auto i = 0.0f; i < indices.size(); i += 3) {

        // Get the vertices (point) for each index
        for (auto j = 0; j < 3; j++) {
            int pos = indices[i + j] * 3;
            verts.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }

        // Get vectors of two edges of triangle
        glm::vec3 U = verts[i + 1] - verts[i];
        glm::vec3 V = verts[i + 2] - verts[i];

        // Calculate normal
        glm::vec3 normal = glm::normalize(-glm::cross(U, V));
        normals.push_back(normal);
    }

    return normals;
}