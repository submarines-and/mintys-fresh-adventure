#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

// sprite atlas
uniform vec2 offset;
uniform vec2 atlasSize;

void main()
{
    float scaleX = 1.0 / atlasSize.x;
    float scaleY = 1.0 / atlasSize.y;

    gl_Position = projection * view * transform * vec4(vertex.xy, 0.0, 1.0);
    texCoords = vec2((vertex.z + offset.x) * scaleX, (vertex.w + offset.y) * scaleY);
}