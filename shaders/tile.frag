#version 330 core

in  vec2 texCoords;
in  vec4 noiseColor;
out vec4 color;

uniform sampler2D image;

void main()
{
    vec4 texColor = texture(image, texCoords);

    // transparent
    if(texColor.a < 0.1)
        discard;

    color = texColor * noiseColor;
}