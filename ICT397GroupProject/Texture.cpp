#include "Texture.h"

Texture::Texture()
{

}

Texture::Texture(const char* fileName, GLenum type)
{
    this->type = type;

    glGenTextures(1, &this->id);
    glBindTexture(type, this->id);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(fileName, &this->width, &this->height, &this->numChannels, 0);

    if (data)
    {
        glTexImage2D(type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glActiveTexture(0);
    glBindTexture(type, 0);
    stbi_image_free(data);
}

inline GLuint Texture::getID() const
{
    return this->id;
}

void Texture::bind(const GLint texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(this->type, this->id);
}

void Texture::unbind()
{
    glActiveTexture(0);
    glBindTexture(this->type, 0);
}

void Texture::loadTexture(const char* fileName)
{
    if (this->id)
    {
        glDeleteTextures(1, &this->id);
    }

    this->type = type;

    glGenTextures(1, &this->id);
    glBindTexture(type, this->id);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(fileName, &this->width, &this->height, &this->numChannels, 0);

    if (data)
    {
        glTexImage2D(type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glActiveTexture(0);
    glBindTexture(type, 0);
    stbi_image_free(data);
}