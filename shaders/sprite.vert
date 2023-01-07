#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;

// position and size
uniform vec3 position;
uniform vec2 size;

// camera info
uniform vec3 cameraRight;
uniform vec3 cameraUp;

// sprite atlas
uniform vec2 offset;
uniform vec2 atlasSize;

void main()
{
    // select position in atlas
    float scaleX = 1.0 / atlasSize.x;
    float scaleY = 1.0 / atlasSize.y;
    texCoords = vec2((vertex.z + offset.x) * scaleX, (vertex.w + offset.y) * scaleY);

    // billboarding
    vec3 transform = position + cameraRight * vertex.x * size.x + cameraUp * vertex.y * size.y;
    gl_Position = projection * view  * vec4(transform, 1.0);
}