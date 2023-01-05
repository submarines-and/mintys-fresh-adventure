#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <random>
#include <string>

#include "gfx/opengl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "gfx/camera.h"
#include "gfx/renderer.h"
#include "gfx/shader.h"
#include "gfx/window.h"
#include "global.h"
#include "tiny_obj_loader.h"
#include "world/noise.h"

// Structs
struct plant {
    std::string type;
    float xpos;
    float ypos;
    float zpos;
    int xOffset;
    int yOffset;

    plant(std::string _type, float _xpos, float _ypos, float _zpos, int _xOffset, int _yOffset)
    {
        type = _type;
        xpos = _xpos;
        ypos = _ypos;
        zpos = _zpos;
        xOffset = _xOffset;
        yOffset = _yOffset;
    }
};

/**
 *
 *
 *
 */
std::vector<int> generate_indices();
std::vector<float> generate_vertices(const std::vector<float>& noise_map);
std::vector<float> generate_normals(const std::vector<int>& indices, const std::vector<float>& vertices);
std::vector<float> generate_biome(const std::vector<float>& vertices, std::vector<plant>& plants, int xOffset, int yOffset);
void generate_map_chunk(GLuint& VAO, int xOffset, int yOffset, std::vector<plant>& plants);

void load_model(GLuint& VAO, std::string filename);
void setup_instancing(GLuint& VAO, std::vector<GLuint>& plant_chunk, std::string plant_type, std::vector<plant>& plants, std::string filename);
/**
 *
 *
 *
 */

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

// Map params
float WATER_HEIGHT = 0.1;
int chunk_render_distance = 3;
int xMapChunks = 10;
int yMapChunks = 10;
int chunkWidth = 127;
int chunkHeight = 127;
int gridPosX = 0;
int gridPosY = 0;
float originX = (chunkWidth * xMapChunks) / 2 - chunkWidth / 2;
float originY = (chunkHeight * yMapChunks) / 2 - chunkHeight / 2;
float meshHeight = 32; // Vertical scaling

// Model params
float MODEL_SCALE = 3;
float MODEL_BRIGHTNESS = 6;

std::vector<GLuint> map_chunks(xMapChunks* yMapChunks);
int nIndices = chunkWidth * chunkHeight * 6;

void setup_instancing(GLuint& VAO, std::vector<GLuint>& plant_chunk, std::string plant_type, std::vector<plant>& plants, std::string filename)
{
    std::vector<std::vector<float>> chunkInstances;
    chunkInstances.resize(xMapChunks * yMapChunks);

    // Instancing prep
    for (int i = 0; i < plants.size(); i++) {
        float xPos = plants[i].xpos / MODEL_SCALE;
        float yPos = plants[i].ypos / MODEL_SCALE;
        float zPos = plants[i].zpos / MODEL_SCALE;
        int pos = plants[i].xOffset + plants[i].yOffset * xMapChunks;

        if (plants[i].type == plant_type) {
            chunkInstances[pos].push_back(xPos);
            chunkInstances[pos].push_back(yPos);
            chunkInstances[pos].push_back(zPos);
        }
    }

    GLuint instancesVBO[xMapChunks * yMapChunks];
    glGenBuffers(xMapChunks * yMapChunks, instancesVBO);

    for (int y = 0; y < yMapChunks; y++) {
        for (int x = 0; x < xMapChunks; x++) {
            int pos = x + y * xMapChunks;
            load_model(plant_chunk[pos], filename);

            glBindVertexArray(plant_chunk[pos]);
            glBindBuffer(GL_ARRAY_BUFFER, instancesVBO[pos]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * chunkInstances[pos].size(), &chunkInstances[pos][0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            // Instanced array
            // Move to next vertex attrib on next instance of object
            glVertexAttribDivisor(3, 1);
        }
    }
}

void load_model(GLuint& VAO, std::string filename)
{
    std::vector<float> vertices;
    std::vector<int> indices;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }
    else if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[0] * MODEL_BRIGHTNESS);
                vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[1] * MODEL_BRIGHTNESS);
                vertices.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[2] * MODEL_BRIGHTNESS);
            }
            index_offset += fv;
        }
    }

    GLuint VBO, EBO;

    // Create buffers and arrays
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind vertices to VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Configure vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configure vertex normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configure vertex color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void generate_map_chunk(GLuint& VAO, int xOffset, int yOffset, std::vector<plant>& plants)
{
    std::vector<int> indices;
    std::vector<float> noise_map;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> colors;

    Noise noise;

    // Generate map
    indices = generate_indices();
    noise_map = noise.generateNoiseMap(xOffset, yOffset, chunkHeight, chunkWidth);
    vertices = generate_vertices(noise_map);
    normals = generate_normals(indices, vertices);
    colors = generate_biome(vertices, plants, xOffset, yOffset);

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

glm::vec3 get_color(int r, int g, int b)
{
    return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}

struct terrainColor {
    terrainColor(float _height, glm::vec3 _color)
    {
        height = _height;
        color = _color;
    };
    float height;
    glm::vec3 color;
};

std::vector<float> generate_biome(const std::vector<float>& vertices, std::vector<plant>& plants, int xOffset, int yOffset)
{
    std::vector<float> colors;
    std::vector<terrainColor> biomeColors;
    glm::vec3 color = get_color(255, 255, 255);

    // NOTE: Terrain color height is a value between 0 and 1
    biomeColors.push_back(terrainColor(WATER_HEIGHT * 0.5f, get_color(60, 95, 190))); // Deep water
    biomeColors.push_back(terrainColor(WATER_HEIGHT, get_color(60, 100, 190)));       // Shallow water
    biomeColors.push_back(terrainColor(0.15f, get_color(210, 215, 130)));             // Sand
    biomeColors.push_back(terrainColor(0.30f, get_color(95, 165, 30)));               // Grass 1
    biomeColors.push_back(terrainColor(0.40f, get_color(65, 115, 20)));               // Grass 2
    biomeColors.push_back(terrainColor(0.50f, get_color(90, 65, 60)));                // Rock 1
    biomeColors.push_back(terrainColor(0.80f, get_color(75, 60, 55)));                // Rock 2
    biomeColors.push_back(terrainColor(1.00f, get_color(255, 255, 255)));             // Snow

    std::string plantType;

    // Determine which color to assign each vertex by its y-coord
    // Iterate through vertex y values
    for (int i = 1; i < vertices.size(); i += 3) {
        for (int j = 0; j < biomeColors.size(); j++) {
            // NOTE: The max height of a vertex is "meshHeight"
            if (vertices[i] <= biomeColors[j].height * meshHeight) {
                color = biomeColors[j].color;
                if (j == 3) {
                    if (rand() % 1000 < 5) {
                        if (rand() % 100 < 70) {
                            plantType = "flower";
                        }
                        else {
                            plantType = "tree";
                        }
                        plants.push_back(plant{plantType, vertices[i - 1], vertices[i], vertices[i + 1], xOffset, yOffset});
                    }
                }
                break;
            }
        }
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    return colors;
}

std::vector<float> generate_normals(const std::vector<int>& indices, const std::vector<float>& vertices)
{
    int pos;
    glm::vec3 normal;
    std::vector<float> normals;
    std::vector<glm::vec3> verts;

    // Get the vertices of each triangle in mesh
    // For each group of indices
    for (int i = 0; i < indices.size(); i += 3) {

        // Get the vertices (point) for each index
        for (int j = 0; j < 3; j++) {
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

std::vector<float> generate_vertices(const std::vector<float>& noise_map)
{
    std::vector<float> v;

    for (int y = 0; y < chunkHeight + 1; y++)
        for (int x = 0; x < chunkWidth; x++) {
            v.push_back(x);
            // Apply cubic easing to the noise
            float easedNoise = std::pow(noise_map[x + y * chunkWidth] * 1.1f, 3);
            // Scale noise to match meshHeight
            // Pervent vertex height from being below WATER_HEIGHT
            v.push_back(std::fmax(easedNoise * meshHeight, WATER_HEIGHT * 0.5f * meshHeight));
            v.push_back(y);
        }

    return v;
}

std::vector<int> generate_indices()
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

void init(int ticks)
{
#pragma unused(ticks)

    global.renderer->loadShader(Shader::TERRAIN, "shaders/object.vert", "shaders/object.frag");
    std::vector<plant> plants;

    for (int y = 0; y < yMapChunks; y++)
        for (int x = 0; x < xMapChunks; x++) {
            generate_map_chunk(map_chunks[x + y * xMapChunks], x, y, plants);
        }

    //  std::vector<GLuint> tree_chunks(xMapChunks * yMapChunks);
    // std::vector<GLuint> flower_chunks(xMapChunks * yMapChunks);
    // GLuint treeVAO, flowerVAO;
    // setup_instancing(treeVAO, tree_chunks, "tree", plants, "obj/CommonTree_1.obj");
    //  setup_instancing(flowerVAO, flower_chunks, "flower", plants, "obj/Flowers.obj");
}

void update(int ticks)
{
#pragma unused(ticks)
}

void render(int ticks)
{
#pragma unused(ticks)

    glm::mat4 projection = glm::perspective(glm::radians(global.camera->zoom), (float)global.width / (float)global.height, 0.1f, (float)chunkWidth * (chunk_render_distance - 1.2f));
    glm::mat4 view = global.camera->getViewMatrix();

    Shader* objectShader = global.renderer->getShader(Shader::TERRAIN);
    objectShader->start();
    objectShader->setBool("isFlat", true);

    // Lighting intensities and direction
    objectShader->setVec3("light.ambient", 0.2, 0.2, 0.2);
    objectShader->setVec3("light.diffuse", 0.3, 0.3, 0.3);
    objectShader->setVec3("light.specular", 1.0, 1.0, 1.0);
    objectShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    objectShader->setMat4("u_projection", projection);
    objectShader->setMat4("u_view", view);
    objectShader->setVec3("u_viewPos", global.camera->position);

    glm::mat4 model;

    // Measures number of map chunks away from origin map chunk the camera is
    gridPosX = (int)(global.camera->position.x - originX) / chunkWidth + xMapChunks / 2;
    gridPosY = (int)(global.camera->position.z - originY) / chunkHeight + yMapChunks / 2;

    // Render map chunks
    for (int y = 0; y < yMapChunks; y++)
        for (int x = 0; x < xMapChunks; x++) {
            // Only render chunk if it's within render distance
            if (std::abs(gridPosX - x) <= chunk_render_distance && (y - gridPosY) <= chunk_render_distance) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * y));
                objectShader->setMat4("u_model", model);

                // Terrain chunk
                glBindVertexArray(map_chunks[x + y * xMapChunks]);
                glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

                // Plant chunks
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * y));
                model = glm::scale(model, glm::vec3(MODEL_SCALE));
                objectShader->setMat4("u_model", model);

                /*
                glEnable(GL_CULL_FACE);
                glBindVertexArray(flower_chunks[x + y * xMapChunks]);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 1300, 16);

                glBindVertexArray(tree_chunks[x + y * xMapChunks]);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 10192, 8);
                glDisable(GL_CULL_FACE);
                */
            }
        }
}

void destroy(int ticks)
{
#pragma unused(ticks)

    /*
        for (int i = 0; i < map_chunks.size(); i++) {
            glDeleteVertexArrays(1, &map_chunks[i]);
            glDeleteVertexArrays(1, &tree_chunks[i]);
            glDeleteVertexArrays(1, &flower_chunks[i]);
        }
    */
}

int main()
{
    global.width = 1920;
    global.height = 1080;
    srand((unsigned int)time(NULL));

    Window window(global.width, global.height, init, update, render, destroy);
    global.renderer = new Renderer();
    global.camera = new Camera(glm::vec3(originX, 20.0f, originY));

    window.loop();

    return 0;
}
