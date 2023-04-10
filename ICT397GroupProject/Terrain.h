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
    int widthScale;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;
    int rez, numOfStrips, numOfTrisPerStrip;

    void initVAO();

    float getMaxY(std::vector<glm::vec3> list);

    float getMinY(std::vector<glm::vec3> list);

    void getHeights(float faultHeights[][400], int faultWidth, int faultHeight, float iterations, float minHeight, float maxHeight);

    void applyFilter(float faultHeights[][400], int faultWidth, int faultHeight, float iterations, float filter);

    void normaliseHeights(float maxHeight, float minHeight);
public:
    Terrain() {}

    Terrain(const char* fileName, float scale);

    Terrain(float iterations, float minHeight, float maxHeight, float filter);

    void loadHeightMap(const char* fileName, float scale);

    void generateFaultFormation(float iterations, float minHeight, float maxHeight, float filter);

    void renderTerrain(bool renderType);

    std::vector<glm::vec3> getVertices();
};