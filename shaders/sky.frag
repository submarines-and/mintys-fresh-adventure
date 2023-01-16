#version 330 core
out vec4 outColor;

in vec3 texCoords;
uniform samplerCube skybox;

void main()
{    
    outColor = texture(skybox, texCoords);
}