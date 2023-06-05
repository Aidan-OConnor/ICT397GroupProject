#pragma once

#include "Texture.h"
#include "Shader.h"
#include <iostream>
#include <string>
#include <vector>

/*
 * @class Skybox
 * @brief Manages the creation of a skybox for the engine
 *
 * @author Lochlyn Edward
 * @version 01
 * @date 22/5/2023 Lochlyn Edward, created
 *
 * @bug No known bugs
 */
class Skybox
{
public:
    
    /*
    * @brief Skybox constructor
    *
    * @return void
    */
    Skybox() = default;
    
    /*
    * @brief Skybox destructor
    *
    * @return void
    */
    ~Skybox();
    
    /*
    * @brief Function to initialise skybox
    *
    * @return void
    */
    void Init();
    
    /*
    * @brief Function to draw the skybox in the scene
    *
    * @param glm::mat4, glm::mat4
    * 
    * @return void
    */
    void Draw(const glm::mat4& view, const glm::mat4& projection);
    
    /*
    * @brief Function for loading texture for cubemap of the skybox
    *
    * @param std::vector<std::string>
    * 
    * @return unsigned int
    */
    unsigned int loadCubemap(std::vector<std::string> faces);
    
    std::unique_ptr<Shader> skyboxShader = nullptr; /// Pointer variable for skybox shader

private:
    
    unsigned int skyboxVAO = 0, skyboxVBO = 0; /// Unsigned int variable for skybox VAO and VBO
    
    unsigned int cubemapTexture = 0; /// Unsigned int variable for skybox texture
    
    std::vector<std::string> faces; /// Vector for the faces of the skybox cube

    float skyboxVertices[108] = { /// Float array of skybox vertice locations
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
};