#pragma once

#include "Terrain.h"
#include "Texture.h"

/*
 * @class Landscape
 * @brief Contains a terrain and some textures for rendering
 *
 * Landscape stores a terrain object as well as up to 3 textures
 * to be rendered to that object through it render functions
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 21/4/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

class Landscape
{
private:
	/// Stores a terrain object
	Terrain* terrain;
	/// Stores a texture object
	Texture* texture;
	/// Stores the second texture object
	Texture* texture2;
	/// Stores the third texture object
	Texture* texture3;
public:
	/*
	 * @brief Default Landscape constructor
	 *
	 * Landscape constructor for instantiating a landscape
	 * with no parameters
	 *
	 * @return void
	 */
	Landscape() {};

	/*
	 * @brief Loads a terrain through a height map
	 *
	 * This function generates a terrain object through
	 * a given height map file path as well as a given
	 * texture
	 *
	 * @param heightMapFile, mapScale, textureFile, textureType
	 * @return void
	 */
	void loadFromHeightmap(const char* heightMapFile, int mapScale, const char* textureFile, GLenum textureType);

	/*
	 * @brief Loads a terrain through a fault formation
	 *
	 * This function generates a terrain object through
	 * given fault formation variables as well as a given
	 * texture
	 *
	 * @param iterations, width, height, widthScale, heightScale, minHeight, maxHeight, filter textureFile, textureType
	 * @return void
	 */
	void loadFromFaultFormation(float iterations, float width, float height, float widthScale, float heightScale, float minHeight, float maxHeight, float filter, const char* textureFile, GLenum textureType);

	/*
	 * @brief Adds two more textures
	 *
	 * This function adds two more textures to generate a
	 * terrain with three textures
	 *
	 * @param textureFile, textureType, textureFile2, textureType2
	 * @return void
	 */
	void addTextures(const char* textureFile, GLenum textureType, const char* textureFile2, GLenum textureType2);

	/*
	 * @brief Renders the Landscape
	 *
	 * This function renders the terrain and its
	 * textures to render a complete landscape
	 *
	 * @param renderType
	 * @return void
	 */
	void renderLandscape(bool renderType);

	/*
	 * @brief Renders a water landscape
	 *
	 * This function renders a water landscape with
	 * its given singular texture.
	 *
	 * @param renderType
	 * @return void
	 */
	void renderWater(bool renderType);

	/*
	 * @brief Returns the current terrain
	 *
	 * This function returns the currently loaded terrain
	 *
	 * @return terrain
	 */
	Terrain getTerrain();
};