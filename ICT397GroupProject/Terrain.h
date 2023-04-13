#pragma once
#include <iostream>
#include <glm.hpp>
#include <Vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

class Terrain
{
private:
    int width, height, channels;
    unsigned char* heightMap;
    float widthScale;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;
    int rez, numOfStrips, numOfTrisPerStrip;
    unsigned int terrainVAO, terrainVBO, terrainIBO;

    void initVAO();

    float getMaxY(std::vector<glm::vec3> list);

    float getMinY(std::vector<glm::vec3> list);

    void getHeights(float** faultHeights, int fWidth, int fHeight, float iterations, float minHeight, float maxHeight);

    void applyFilter(float** faultHeights, int fWidth, int fHeight, float iterations, float filter);

    void normaliseHeights(float maxHeight, float minHeight);
public:
    Terrain() {}

    Terrain(const char* fileName, float scale);

    Terrain(float iterations, float fWidth, float fHeight, float widthScale, float heightScale, float minHeight, float maxHeight, float filter);

    void loadHeightMap(const char* fileName, float scale);

    void generateFaultFormation(float iterations, float fWidth, float fHeight, float widthScale, float heightScale, float minHeight, float maxHeight, float filter);

    void renderTerrain(bool renderType);

    std::vector<glm::vec3> getVertices();

    int getWidth();

    int getHeight();
};