#version 330 core

in  vec2 texCoords;
out vec4 color;

uniform sampler2D image;

void main()
{
    vec4 texColor = texture(image, texCoords);

    // transparent parts of image
    if(texColor.a < 0.1)
        discard;

    color = texColor;

}