#pragma once

#include "Terrain.h"
#include "Texture.h"

class Landscape
{
private:
	Terrain* terrain;
	Texture* texture;
public:
	Landscape() {};

	void loadFromHeightmap(const char* heightMapFile, int mapScale, const char* textureFile, GLenum textureType)
	{
		this->terrain = new Terrain(heightMapFile, mapScale);
		this->texture = new Texture(textureFile, textureType);
	}

	void loadFromFaultFormation(float iterations, float width, float height, float minHeight, float maxHeight, float filter, const char* textureFile, GLenum textureType)
	{
		this->terrain = new Terrain(iterations, width, height, minHeight, maxHeight, filter);
		this->texture = new Texture(textureFile, textureType);
	}

	void renderLandscape(bool renderType)
	{
		this->texture->bind(0);
		this->terrain->renderTerrain(renderType);
	}

	Terrain getTerrain()
	{
		return *this->terrain;
	}
};