#pragma once
#include <iostream>
#include <glm.hpp>
#include <Vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

/*
 * @class Terrain
 * @brief Class for creating terrains either through a heightmap or an algorithm
 *
 * Terrain is a class that can be used to generate terrains through two methods;
 * loading a height map or generating a fault formation. The class also contains
 * the necesarry function to render the terrain.
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 21/4/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

class Terrain
{
private:
    /// Stores the width of a terrain from an image
    int width;
    /// Stores the height of a terrain from an image
    int height;
    /// Stores the number of channels from an image
    int channels;
    /// Stores the pixel data from an image
    unsigned char* heightMap;
    /// Stores a scale for the width of a terrain
    float widthScale;
    /// Stores the vertex location for a terrain
    std::vector<glm::vec3> vertices;
    /// Stores the indice locations for a terrain
    std::vector<unsigned> indices;
    /// Stores the resolution scale for a terrain
    int rez;
    /// Stores the number of strips for a terrain grid
    int numOfStrips;
    /// Stores the number of triangles per strip for a terrain grid
    int numOfTrisPerStrip;
    /// Stores a terrains vertex array object
    unsigned int terrainVAO;
    /// Stores a terrains vertex buffer object
    unsigned int terrainVBO;
    /// Stores a terrains index buffer object
    unsigned int terrainIBO;

    /*
     * @brief Initializes terrain VAO
     *
     * Initializes the vertex array object for the generated
     * terrain.
     *
     * @return void
     */
    void initVAO();

    /*
     * @brief Returns maximum Y value
     *
     * Calculates the maximum Y values of a given set of
     * vec3's and returns it.
     *
     * @param list
     * @return maxY
     */
    float getMaxY(std::vector<glm::vec3> list);

    /*
     * @brief Returns minimum Y value
     *
     * Calculates the minimum Y values of a given set of
     * vec3's and returns it.
     *
     * @param list
     * @return minY
     */
    float getMinY(std::vector<glm::vec3> list);

    /*
     * @brief Gets the Y values for a fault formation grid
     *
     * Calculates the height for each point on a grid
     * based on the number of iterations the terrain will perform
     * the fault formation algorithm on.
     *
     * @param faultHeights, fWidth, fHeight, iterations, minHeight, maxHeight
     * @return void
     */
    void getHeights(float** faultHeights, int fWidth, int fHeight, float iterations, float minHeight, float maxHeight);

    /*
     * @brief Applies the Fir filter to the fault formation grid
     *
     * Calculates the height for each point on a grid
     * through applying the Fir filter 4 times along the
     * grid.
     *
     * @param faultHeights, fWidth, fHeight, iterations, filter
     * @return void
     */
    void applyFilter(float** faultHeights, int fWidth, int fHeight, float iterations, float filter);

    /*
     * @brief Normalises the heights of the fault formation grid
     *
     * Calculates the height for each point on a grid
     * through normalising the heights for each point.
     *
     * @param maxHeight, minHeight
     * @return void
     */
    void normaliseHeights(float maxHeight, float minHeight);
public:
    /*
     * @brief Default Terrain constructor
     *
     * Default terrain constructor for when a terrain
     * object is created without any parameters.
     *
     * @return void
     */
    Terrain() {}

    /*
    * @brief Terrain constructor for loading a height map
    *
    * Terrain constructor for when the terrain object is to
    * load in a height map on creation.
    *
    * @param fileName, scale
    * @return void
    */
    Terrain(const char* fileName, float scale);

    /*
    * @brief Terrain constructor for loading a fault formation
    *
    * Terrain constructor for when the terrain object is to
    * generate a fault formation upon creation.
    *
    * @param iterations, fWidth, fHeight, widthScale, heightScale, minHeight, maxHeight, filter
    * @return void
    */
    Terrain(float iterations, float fWidth, float fHeight, float widthScale, float heightScale, float minHeight, float maxHeight, float filter);

    /*
    * @brief Function for loading a terrain from a height map
    *
    * This function is used to generate a terrain based off of a
    * height map stored in the given file name.
    *
    * @param fileName, scale
    * @return void
    */
    void loadHeightMap(const char* fileName, float scale);

    /*
    * @brief Function for loading a terrain using a fault formation
    *
    * This function is used to generate a terrain based off of a 
    * fault formation algorithm with the variables given.
    *
    * @param iterations, fWidth, fHeight, widthScale, heightScale, minHeight, maxHeight, filter
    * @return void
    */
    void generateFaultFormation(float iterations, float fWidth, float fHeight, float widthScale, float heightScale, float minHeight, float maxHeight, float filter);

    /*
    * @brief Renders terrain
    *
    * This function renders the terrain based off of a given
    * render type stored as a boolean, which will either draw
    * a triangle strip or a wireframe.
    *
    * @param renderType
    * @return void
    */
    void renderTerrain(bool renderType);

    /*
    * @brief Returns the vertices of the terrain
    *
    * This function returns a vector of the points for
    * the currently created terrain
    *
    * @return vertices
    */
    std::vector<glm::vec3> getVertices();

    /*
    * @brief Returns the terrain width
    *
    * This function returns the terrains width by using
    * the scale previously used to generate the terrain
    *
    * @return width
    */
    int getWidth();

    /*
    * @brief Returns the terrain height
    *
    * This function returns the terrains height by using
    * the scale previously used to generate the terrain
    *
    * @return height
    */
    int getHeight();
    
    /*
    * @brief Returns the height of a on object
    *
    * This function returns the height an object
    * should be at the given x and z values
    *
    * @return heightAtPos
    */
    int getHeightAtPos(std::vector<glm::vec3> temp, int x, int z);
};