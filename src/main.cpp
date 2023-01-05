#include "gfx/camera.h"
#include "gfx/opengl.h"
#include "gfx/renderer.h"
#include "gfx/shader.h"
#include "gfx/window.h"
#include "global.h"
#include "world/world.h"
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <random>
#include <string>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

// Map params
int renderDistance = 3;
int xMapChunks = 10;
int yMapChunks = 10;
int chunkWidth = 127;
int chunkHeight = 127;
int gridPosX = 0;
int gridPosY = 0;
float originX = (chunkWidth * xMapChunks) / 2 - chunkWidth / 2;
float originY = (chunkHeight * yMapChunks) / 2 - chunkHeight / 2;

std::vector<GLuint> map_chunks(xMapChunks* yMapChunks);
int nIndices = chunkWidth * chunkHeight * 6;

void init(int ticks)
{
#pragma unused(ticks)
    global.renderer->loadShader(Shader::TERRAIN, "shaders/terrain.vert", "shaders/terrain.frag");

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
    gridPosX = (int)(global.camera->position.x - originX) / chunkWidth + xMapChunks / 2;
    gridPosY = (int)(global.camera->position.z - originY) / chunkHeight + yMapChunks / 2;

    // Render map chunks
    for (int y = 0; y < yMapChunks; y++)
        for (int x = 0; x < xMapChunks; x++) {

            // Only render chunk if it's within render distance
            if (std::abs(gridPosX - x) <= renderDistance && (y - gridPosY) <= renderDistance) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * y));
                objectShader->setMat4("u_model", model);

                // Terrain chunk
                glBindVertexArray(map_chunks[x + y * xMapChunks]);
                glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
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
