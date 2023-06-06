#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

class Texture
{
private:
    GLuint id;
    int width;
    int height;
    int numChannels;
    unsigned int type;
public:
    Texture();

    Texture(const char* fileName, GLenum type);

    unsigned int TextureFromFile(std::string path);

    bool LoadTextureForGUI(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

    GLuint getID();

    void bind(const GLint texture_unit);

    void unbind();

    void loadTexture(const char* fileName);

    int getWidth();

    int getHeight();
};