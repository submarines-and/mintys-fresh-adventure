#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 offset;
layout (location = 2) in mat4 transform;
layout (location = 6) in vec4 noiseColorIn;

out vec2 texCoords;
out vec4 noiseColor;

uniform mat4 projection;
uniform mat4 view;

// sprite atlas
uniform vec2 atlasSize;


void main()
{
    float scaleX = 1.0 / atlasSize.x;
    float scaleY = 1.0 / atlasSize.y;

    gl_Position = projection * view * transform * vec4(vertex.xy, 0.0, 1.0);
    texCoords = vec2((vertex.z + offset.x) * scaleX, (vertex.w + offset.y) * scaleY);
    noiseColor = noiseColorIn;
}