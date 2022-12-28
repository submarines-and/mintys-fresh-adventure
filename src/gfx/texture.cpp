#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* filepath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if (data) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load image %s\n", filepath);
    }

    stbi_image_free(data);
}

Texture::~Texture()
{
    printf("Deleting texture %i\n", id);
    glDeleteTextures(1, &id);
}
