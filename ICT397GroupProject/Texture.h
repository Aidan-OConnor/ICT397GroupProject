#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

class Texture
{
private:
    unsigned int id;
    int width;
    int height;
    int numChannels;
    unsigned int type;
public:
    Texture();

    Texture(const char* fileName, GLenum type);

    unsigned int TextureFromFile(std::string path);

    inline GLuint getID() const;

    void bind(const GLint texture_unit);

    void unbind();

    void loadTexture(const char* fileName);
};