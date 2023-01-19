#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;

// position and size
uniform vec3 position;
uniform vec3 scale;
uniform vec2 rotation;

// camera info
uniform vec3 cameraRight;
uniform vec3 cameraUp;

// sprite atlas
uniform vec2 atlasOffset;
uniform vec2 atlasSize;

void main()
{
    // select position in atlas
    float scaleX = 1.0 / atlasSize.x;
    float scaleY = 1.0 / atlasSize.y;

    texCoords = vec2((vertex.z * rotation.x + atlasOffset.x) * scaleX, (vertex.w * rotation.y + atlasOffset.y) * scaleY);

    // billboarding
    vec3 transform = position + cameraRight * vertex.x * scale.x + cameraUp * vertex.y * scale.y;
    gl_Position = projection * view  * vec4(transform, 1.0);
}