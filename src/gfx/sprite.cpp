#include "sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sprite::Sprite(const char* filepath)
{
   // stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &id);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // and finally free image data
    stbi_image_free(data);
}

Sprite::~Sprite()
{
    printf("Deleting sprite %i\n", id);
    glDeleteTextures(1, &id);
}

void Sprite::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}
