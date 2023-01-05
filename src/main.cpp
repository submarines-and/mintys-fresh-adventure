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
#include "world/world.h"

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
int chunk_render_distance = 3;
int xMapChunks = 10;
int yMapChunks = 10;
int chunkWidth = 127;
int chunkHeight = 127;
int gridPosX = 0;
int gridPosY = 0;
float originX = (chunkWidth * xMapChunks) / 2 - chunkWidth / 2;
float originY = (chunkHeight * yMapChunks) / 2 - chunkHeight / 2;

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

void init(int ticks)
{
#pragma unused(ticks)
    global.renderer->loadShader(Shader::TERRAIN, "shaders/object.vert", "shaders/object.frag");

    World world(chunkHeight, chunkWidth);

    for (int y = 0; y < yMapChunks; y++) {
        for (int x = 0; x < xMapChunks; x++) {
            world.generateWorldChunk(map_chunks[x + y * xMapChunks], x, y);
        }
    }
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
            }
        }
}

void destroy(int ticks)
{
#pragma unused(ticks)
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
