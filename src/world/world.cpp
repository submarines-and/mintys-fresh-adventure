#include "world.h"
#include "components/ai.component.h"
#include "components/model.component.h"
#include "components/transform.component.h"
#include "global.h"
#include "util/math.h"
#include <glm/gtc/matrix_transform.hpp>

World::World(int numberOfChunks) : shader(Shader("shaders/terrain.vert", "shaders/terrain.frag")), biomeGen(meshHeight)
{
    this->numberOfChunks = numberOfChunks;
    this->worldWidth = numberOfChunks * chunkWidth - numberOfChunks;
    this->worldHeight = numberOfChunks * chunkHeight - numberOfChunks;

    chunks = std::vector<WorldChunk>(numberOfChunks * numberOfChunks);
    indices = generateIndices();

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
    for (auto chunk : chunks) {
        glDeleteVertexArrays(1, &chunk.id);
    }

    chunks.clear();
    indices.clear();
}

void World::render()
{
    shader.start();
    shader.setMat4("projection", global.camera->getProjectionMatrix());
    shader.setMat4("view", global.camera->getViewMatrix());

    shader.setVec3("lightDirection", glm::vec3(0.3f, -1.0f, 0.5f));
    shader.setVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.8f));
    shader.setVec2("lightBias", glm::vec2(0.3f, 0.8f));

    // render visible chunks
    for (auto& chunk : chunks) {

        if (glm::abs(chunk.x * chunkWidth - global.camera->position.x) <= renderDistance && glm::abs(chunk.y * chunkHeight - global.camera->position.z) <= renderDistance) {

            // generate if missing
            if (!chunk.generated) {
                generateWorldChunk(chunk);
            }

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3((chunkWidth - 1) * chunk.x, 0.0, (chunkHeight - 1) * chunk.y));
            shader.setMat4("transform", transform);

            // Terrain chunk
            glBindVertexArray(chunk.id);
            glDrawElements(GL_TRIANGLES, chunkWidth * chunkHeight * 6, GL_UNSIGNED_INT, 0);
        }
    }

    shader.stop();
}

WorldChunk World::getWorldChunk(glm::vec3 position)
{
    // get active chunk
    int gridX = position.x / chunkWidth;
    int gridZ = position.z / chunkHeight;

    auto chunkIndex = gridX + gridZ * numberOfChunks;
    if (chunkIndex > (int)chunks.size() - 1) {
        printf("Out of bounds: x:%i, z:%i, chunk index: %i, pX: %f, pY: %f, pZ: %f\n", gridX, gridZ, chunkIndex, position.x, position.y, position.z);
        return WorldChunk();
    }

    return chunks[chunkIndex];
}

float World::getTerrainHeight(glm::vec3 position)
{
    auto chunk = getWorldChunk(position);
    if (!chunk.generated) {
        return 1.0f;
    }

    float terrainX = position.x - chunk.x * chunkWidth;
    float terrainZ = position.z - chunk.y * chunkHeight;
    float gridSquareSize = glm::abs(chunkWidth / (glm::sqrt(chunk.heights.size()) - 1));
    int gridX = terrainX / gridSquareSize;
    int gridZ = terrainZ / gridSquareSize;
    float xCoord = fmod(terrainX, gridSquareSize) / gridSquareSize;
    float zCoord = fmod(terrainZ, gridSquareSize) / gridSquareSize;

    if (xCoord <= (1 - zCoord)) {
        return Math::baryCentric(glm::vec3(0, chunk.heights[gridX + gridZ * chunkWidth], 0), glm::vec3(1, chunk.heights[gridX + 1 + gridZ * chunkWidth], 0), glm::vec3(0, chunk.heights[gridX + (gridZ + 1) * chunkWidth], 1), glm::vec2(xCoord, zCoord));
    }
    else {
        return Math::baryCentric(glm::vec3(1, chunk.heights[gridX + 1 + gridZ * chunkWidth], 0), glm::vec3(1, chunk.heights[gridX + 1 + (gridZ + 1) * chunkWidth], 1), glm::vec3(0, chunk.heights[gridX + (gridZ + 1) * chunkWidth], 1), glm::vec2(xCoord, zCoord));
    }
}

Biome::TerrainType World::getTerrainType(glm::vec3 position)
{
    auto chunk = getWorldChunk(position);
    if (!chunk.generated) {
        return Biome::GRASS;
    }

    auto heightAtPosition = getTerrainHeight(position);
    return biomeGen.getTerrainType(heightAtPosition);
}

void World::generateWorldChunk(WorldChunk& chunk)
{
    // reserve memory for heights vector
    chunk.heights = std::vector<float>(chunkHeight * chunkWidth);

    // create noise
    auto noiseMap = noise.generateNoiseMap(chunk.x, chunk.y, chunkHeight, chunkWidth);

    // generate vertices
    std::vector<float> vertices;

    for (int y = 0; y < chunkHeight; y++) {
        for (int x = 0; x < chunkWidth; x++) {

            // pervent vertex from being below water
            float noiseAtPoint = noiseMap[x + y * chunkWidth];
            float waterHeight = biomeGen.getTerrainHeight(Biome::WATER) * meshHeight;
            auto height = std::fmax(noiseAtPoint * meshHeight, waterHeight);

            // persist height to chunk map for later lookup
            chunk.heights[x + y * chunkWidth] = height;

            vertices.emplace_back(x);
            vertices.emplace_back(height);
            vertices.emplace_back(y);

            // don't generate objects in water
            if (height <= waterHeight) {
                continue;
            }

            // small chance to generate a tree
            if (Math::random(0, 1000) < 2) {
                auto tree = global.ecs->createEntity();
                global.ecs->addComponent(tree, TransformComponent{.position = glm::vec3(x + chunk.x * chunkWidth, height, y + chunk.y * chunkHeight)});
                global.ecs->addComponent(tree, ModelComponent{.modelFilePath = "obj/tree.obj"});
            }
            else if (Math::random(0, 10000) < 5) {
                // sheep
                auto sheep = global.ecs->createEntity();
                global.ecs->addComponent(sheep, TransformComponent{
                                                    .position = glm::vec3(x + chunk.x * chunkWidth, height, y + chunk.y * chunkHeight),
                                                    .speed = 3.0f,
                                                });
                global.ecs->addComponent(sheep, ModelComponent{.modelFilePath = "obj/sheep.obj"});
                global.ecs->addComponent(sheep, AIComponent{});
            }
        }
    }

    // normals
    auto normals = generateNormals(indices, vertices);

    // randomize biometype
    /*
    auto rainfall = Math::random(0.0f, 1.0f);
    auto temperature = Math::random(0.0f, 1.0f);
    chunk.biomemaiType = biomeGen.getBiomeType(rainfall, temperature);
    */

    // reserve memory for terrain
    chunk.terrain = std::vector<Biome::TerrainType>(chunkHeight * chunkWidth);

    // get colors per point and save biome type to chunk
    std::vector<glm::vec3> colors;
    for (auto i = 1; i < (int)vertices.size(); i += 3) {
        auto terrainType = biomeGen.getTerrainType(vertices[i]);
        auto color = biomeGen.getTerrainColor(chunk.biomeType, terrainType);
        colors.emplace_back(color);

        // persist terrain type
        auto x = vertices[i - 1];
        auto z = vertices[i + 1];
        chunk.terrain[x + z * chunkWidth] = terrainType;
    }

    GLuint vbo[3], ibo;

    // Create buffers and arrays
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &chunk.id);

    glBindVertexArray(chunk.id);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // Color
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // flag as done
    chunk.generated = true;
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
                indices.emplace_back(pos + chunkWidth);
                indices.emplace_back(pos);
                indices.emplace_back(pos + chunkWidth + 1);

                // Bottom right triangle of square
                indices.emplace_back(pos + 1);
                indices.emplace_back(pos + 1 + chunkWidth);
                indices.emplace_back(pos);
            }
        }

    return indices;
}

std::vector<glm::vec3> World::generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices)
{
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> verts;

    // Get the vertices of each triangle in mesh
    // For each group of indices
    for (auto i = 0; i < (int)indices.size(); i += 3) {

        // Get the vertices (point) for each index
        for (auto j = 0; j < 3; j++) {
            int pos = indices[i + j] * 3;
            verts.emplace_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }

        // Get vectors of two edges of triangle
        glm::vec3 U = verts[i + 1] - verts[i];
        glm::vec3 V = verts[i + 2] - verts[i];

        // Calculate normal
        glm::vec3 normal = glm::normalize(-glm::cross(U, V));
        normals.emplace_back(normal);
    }

    return normals;
}
