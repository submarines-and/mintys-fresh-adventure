#include "shader.h"

ShaderLoader::~ShaderLoader()
{
    for (auto id : shaders) {
        glDeleteProgram(id);
    }
}

GLuint ShaderLoader::load()
{
    // vs
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // fs
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // shader program
    auto shaderId = glCreateProgram();
    glAttachShader(shaderId, vs);
    glAttachShader(shaderId, fs);
    glLinkProgram(shaderId);
    shaders.emplace_back(shaderId);

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderId;
}