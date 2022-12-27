#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>

ShaderLoader::~ShaderLoader()
{
    for (auto id : shaders) {
        glDeleteProgram(id);
    }
}

Shader ShaderLoader::load(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        printf("=== Failed to load shader files ===\n");
        printf("Vertext: %s\n", vertexPath);
        printf("Fragment: %s\n", fragmentPath);
    }

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

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

    return Shader{
        .id = shaderId,
    };
}