#pragma once
#include <vector>

#include "gfx/opengl.h"
#include "gfx/shader.h"
#include "texture.h"
#include "vertex.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader& shader);

private:
    GLuint VAO, VBO, EBO;
};