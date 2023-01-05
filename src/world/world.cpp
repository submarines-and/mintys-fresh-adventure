#include "world.h"
#include "gfx/shader.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

World::World()
{
    global.renderer->loadShader(Shader::TERRAIN, "shaders/terrain.vert", "shaders/terrain.frag");
    chunks = std::vector<WorldChunk>(numberOfChunks * numberOfChunks);

    for (int y = 0; y < numberOfChunks; y++) {
        for (int x = 0; x < numberOfChunks; x++) {
            chunks[x + y * numberOfChunks] = WorldChunk();
        }
    }
}

void World::render()
{
    glm::mat4 projection = glm::perspective(glm::radians(global.camera->zoom), (float)global.width / (float)global.height, 0.1f, (float)chunkWidth * (renderDistance - 1.2f));
    glm::mat4 view = global.camera->getViewMatrix();

    Shader* objectShader = global.renderer->getShader(Shader::TERRAIN);
    objectShader->start();
    objectShader->setBool("isFlat", true);
    objectShader->setVec3("light.ambient", 0.2, 0.2, 0.2);
    objectShader->setVec3("light.diffuse", 0.3, 0.3, 0.3);
    objectShader->setVec3("light.specular", 1.0, 1.0, 1.0);
    objectShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    objectShader->setMat4("u_projection", projection);
    objectShader->setMat4("u_view", view);
    objectShader->setVec3("u_viewPos", global.camera->position);

    // Measures number of map chunks away from origin map chunk the camera is
    float originX = (chunkWidth * numberOfChunks) / 2 - chunkWidth / 2;
    float originY = (chunkHeight * numberOfChunks) / 2 - chunkHeight / 2;
    int gridPosX = (int)(global.camera->position.x - originX) / chunkWidth + numberOfChunks / 2;
    int gridPosY = (int)(global.camera->position.z - originY) / chunkHeight + numberOfChunks / 2;

    // Render map chunks
    for (int y = 0; y < numberOfChunks; y++) {
        for (int x = 0; x < numberOfChunks; x++) {

            // Only render chunk if it's within render distance
            if (std::abs(gridPosX - x) <= renderDistance && (y - gridPosY) <= renderDistance) {

                auto& chunk = chunks[x + y * numberOfChunks];
                if (!chunk.generated) {
                    generateWorldChunk(chunk.id, x, y);
                    chunk.generated = true;
                }

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * y));
                objectShader->setMat4("u_model", model);

                // Terrain chunk
                glBindVertexArray(chunk.id);
                glDrawElements(GL_TRIANGLES, chunkWidth * chunkHeight * 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

void World::generateWorldChunk(GLuint& VAO, int xOffset, int yOffset)
{
    std::vector<int> indices = generateIndices();
    std::vector<float> noise_map = noise.generateNoiseMap(xOffset, yOffset, chunkHeight, chunkWidth);
    std::vector<float> vertices = generateVertices(noise_map);
    std::vector<float> normals = generateNormals(indices, vertices);
    std::vector<float> colors = generateBiome(vertices);

    GLuint VBO[3], EBO;

    // Create buffers and arrays
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind vertices to VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Create element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Configure vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    // Configure vertex normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Bind vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

    // Configure vertex colors attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
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

std::vector<float> World::generateNormals(const std::vector<int>& indices, const std::vector<float>& vertices)
{
    int pos;
    glm::vec3 normal;
    std::vector<float> normals;
    std::vector<glm::vec3> verts;

    // Get the vertices of each triangle in mesh
    // For each group of indices
    for (auto i = 0.0f; i < indices.size(); i += 3) {

        // Get the vertices (point) for each index
        for (auto j = 0; j < 3; j++) {
            pos = indices[i + j] * 3;
            verts.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }

        // Get vectors of two edges of triangle
        glm::vec3 U = verts[i + 1] - verts[i];
        glm::vec3 V = verts[i + 2] - verts[i];

        // Calculate normal
        normal = glm::normalize(-glm::cross(U, V));
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }

    return normals;
}

std::vector<float> World::generateBiome(const std::vector<float>& vertices)
{
    std::vector<float> colors;
    std::vector<TerrainColor> biomeColors;
    glm::vec3 color = normalizeColor(255, 255, 255);

    // NOTE: Terrain color height is a value between 0 and 1
    biomeColors.push_back(TerrainColor(waterHeight * 0.5f, normalizeColor(60, 95, 190))); // Deep water
    biomeColors.push_back(TerrainColor(waterHeight, normalizeColor(60, 100, 190)));       // Shallow water
    biomeColors.push_back(TerrainColor(0.15f, normalizeColor(210, 215, 130)));            // Sand
    biomeColors.push_back(TerrainColor(0.30f, normalizeColor(95, 165, 30)));              // Grass 1
    biomeColors.push_back(TerrainColor(0.40f, normalizeColor(65, 115, 20)));              // Grass 2
    biomeColors.push_back(TerrainColor(0.50f, normalizeColor(90, 65, 60)));               // Rock 1
    biomeColors.push_back(TerrainColor(0.80f, normalizeColor(75, 60, 55)));               // Rock 2
    biomeColors.push_back(TerrainColor(1.00f, normalizeColor(255, 255, 255)));            // Snow

    // Determine which color to assign each vertex by its y-coord
    // Iterate through vertex y values
    for (auto i = 1.0f; i < vertices.size(); i += 3) {
        for (auto j = 0.0f; j < biomeColors.size(); j++) {

            // NOTE: The max height of a vertex is "meshHeight"
            if (vertices[i] <= biomeColors[j].height * meshHeight) {
                color = biomeColors[j].color;
                break;
            }
        }

        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    return colors;
}

glm::vec3 World::normalizeColor(int r, int g, int b)
{
    return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}