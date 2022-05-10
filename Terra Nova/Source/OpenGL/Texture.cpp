#include "Texture.h"

#include <GL/glew.h>
#include <stb_image/std_image.h>
#include <iostream>

/*
aici definesc functiile declarate in "Texture.h"
*/
/*
functie chemata cand creez o noua textura, are nevoie de locatia in care se afla textura pentru a o putea incarca
*/
texture::texture(const std::string& filepath) : texture_filepath(filepath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* texture_data = nullptr;
    texture_data = stbi_load(texture_filepath.c_str(), &width, &height, &bits_per_pixel, 0);

    if (!texture_data)
    {
        std::cout << "Texture not found: " << texture_filepath << std::endl;
        return;
    }

    switch (bits_per_pixel)
    {
    case 3:
        internal_format = GL_RGB8;
        format = GL_RGB;
        break;

    case 4:
        internal_format = GL_RGBA8;
        format = GL_RGBA;
        break;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(texture_data);
}

// cand distrug textura ca obiect in c++ o sterg si din OpenGL prin destructor
texture::~texture()
{
    glDeleteTextures(1, &texture_id);
}

void texture::bind(int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
