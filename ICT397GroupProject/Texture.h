#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

	/*
     * @class Texture
     * @brief Stores Texture data and functions to use them
     *
     * Texture stores the data and functions necessary to load in and
     * render textures
     *
     * @author Aidan O'Connor
     * @version 01
     * @date 09/06/2023 Aidan O'Connor, finished
     *
     * @bug No bugs have currently been found
     */

class Texture
{
private:
    /// Stores ID of the current texture
    GLuint id;
    /// Stores the width of an image
    int width;
    /// Stores the height of an image
    int height;
    /// Stores the number of channels of an iamge
    int numChannels;
    /// Stores the type of image to be loaded
    unsigned int type;
public:

	/*
	 * @brief Default Texture constructor
	 *
	 * Defualt constructor for whenever a Texture
	 * is created without paramaters
	 *
	 * @return void
	 */
	Texture();

	/*
	 * @brief Texture constructor when given parameters
	 *
	 * Constructor used to create a Texture object with
	 * the given values
	 *
	 * @param fileName, type
	 * @return void
	 */
	Texture(const char* fileName, GLenum type);

	/*
	 * @brief Returns the current textures id
	 *
	 * This function returns the ID of the current texture
	 *
	 * @return id
	 */
	GLuint getID();

	/*
	 * @brief Calls functions to bind the current texture
	 *
	 * This function calls the necessary functions to the bind
	 * the current texture
	 *
	 * @param texture_unit
	 * @return void
	 */
	void bind(const GLint texture_unit);

	/*
	 * @brief Calls functions to unbind the current texture
	 *
	 * This function calls the functions necessary to
	 * unbind the current texture
	 *
	 * @return void
	 */
	void unbind();

	/*
	 * @brief Calls functions to load a new Texture
	 *
	 * This function calls the necesary functions to load in a new
	 * texture without using the constructor
	 *
	 * @param fileName
	 * @return void
	 */
	void loadTexture(const char* fileName);

	/*
	 * @brief Calls functions to load a new Texture
	 *
	 * This function calls the necesary functions to load in a new
	 * texture in a suitable format for md2Models
	 *
	 * @param path
	 * @return textureId
	 */
    unsigned int TextureFromFile(std::string path);

	/*
	 * @brief Loads texture for ImGui
	 *
	 * This function calls the necesary functions to load in a new
	 * texture compatible with ImGui
	 *
	 * @param fileName, out_texture, out_width, out_height
	 * @return bool
	 */
    bool LoadTextureForGUI(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

	/*
	 * @brief Returns width
	 *
	 * Returns the textures width
	 *
	 * @return width
	 */
    int getWidth();

	/*
	 * @brief Returns height
	 *
	 * Returns the textures width
	 *
	 * @return height
	 */
    int getHeight();
};