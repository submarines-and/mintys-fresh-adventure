#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
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
    auto vertextId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertextId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertextId);

    int success;
    glGetShaderiv(vertextId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertextId, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // fs
    auto fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentId);

    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertextId, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    // shader program
    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertextId);
    glAttachShader(shaderId, fragmentId);
    glLinkProgram(shaderId);
    glValidateProgram(shaderId);

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
        printf("Error: %s\n", infoLog);
    }

    glDeleteShader(vertextId);
    glDeleteShader(fragmentId);
}

Shader::~Shader()
{
    printf("Deleting shader %i\n", shaderId);
    glDeleteProgram(shaderId);
}

void Shader::start()
{
    glUseProgram(shaderId);
}

void Shader::stop()
{
    glUseProgram(0);
}