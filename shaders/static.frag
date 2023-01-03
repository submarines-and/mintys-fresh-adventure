#version 330 core

in  vec2 texCoords;
out vec4 outColor;

uniform sampler2D image;
uniform vec2 tilePosition;

const float tilesWidth  = 4.0;
const float tilesHeight = 1.0;

const float scaleX = 1.0 / tilesWidth;
const float scaleY = 1.0 / tilesHeight;

void main()
{
    outColor = texture(image, vec2((texCoords.x + tilePosition.x) * scaleX, (texCoords.y + tilePosition.y) * scaleY));
}