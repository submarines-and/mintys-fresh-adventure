#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aOffset;

out vec3 passColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main() {
    passColor = aColor;
    gl_Position = projection * view * transform * vec4(aPos + aOffset, 1.0);
}