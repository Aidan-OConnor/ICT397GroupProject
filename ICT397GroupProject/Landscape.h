#pragma once

#include "Terrain.h"
#include "Texture.h"

class Landscape
{
private:
	Terrain* terrain;
	Texture* texture;
	Texture* texture2;
	Texture* texture3;
public:
	Landscape() {};

	void loadFromHeightmap(const char* heightMapFile, int mapScale, const char* textureFile, GLenum textureType)
	{
		this->terrain = new Terrain(heightMapFile, mapScale);
		this->texture = new Texture(textureFile, textureType);
	}

	void loadFromFaultFormation(float iterations, float width, float height, float widthScale, float heightScale, float minHeight, float maxHeight, float filter, const char* textureFile, GLenum textureType)
	{
		this->terrain = new Terrain(iterations, width, height, widthScale, heightScale, minHeight, maxHeight, filter);
		this->texture = new Texture(textureFile, textureType);
	}

	void addTextures(const char* textureFile, GLenum textureType, const char* textureFile2, GLenum textureType2)
	{
		this->texture2 = new Texture(textureFile, textureType);
		this->texture3 = new Texture(textureFile2, textureType2);
	}

	void renderLandscape(bool renderType)
	{
		this->texture->bind(0);
		this->texture2->bind(1);
		this->texture3->bind(2);
		this->terrain->renderTerrain(renderType);
	}

	void renderWater(bool renderType)
	{
		this->texture->bind(0);
		this->terrain->renderTerrain(renderType);
	}

	Terrain getTerrain()
	{
		return *this->terrain;
	}
};