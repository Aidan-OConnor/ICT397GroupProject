#pragma once
#ifndef MD2Loader_H
#define MD2Loader_h

#include <vector>
#include <cstdio>
#include <fstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "VertexBuffer.h"

/*
 * @class MD2Loader
 * @brief Contains data and functions for loading and rendering md2Models
 *
 * MD2Loader contains all the data and functions necessary to load an md2
 * formatted file with a given filepath and then render the loaded model
 * and specified animations
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 09/06/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

typedef float int3[3];
struct md2_t
{
    int identify;
    int version;

    int skinWidth;
    int skinhHight;
    int frameSize;

    int numSkins;
    int numVerts;
    int numTex;
    int numTri;
    int numCmds;
    int numFrames;

    int ofsSkins;
    int ofsTex;
    int ofsTri;
    int ofsFrames;
    int ofsCmds;
    int ofsEnd;

};

struct vertex_md2
{
    unsigned char v[3];
    unsigned char lightNormIndex;
};

struct frame_md2
{
    float scale[3];
    float translate[3];
    char name[16];
    vertex_md2 verts[1];
};

struct animation_md2
{
    int firstFrame;
    int lastFrame;
    int fps;
};

struct animationState
{
    int	startFrame;
    int	endFrame;
    int	fps;
    float currTime;
    float prevTime;
    float interpol;
    int	type;
    int	currFrame;
    int	nextFrame;
};

typedef enum {
    STAND,
    RUN,
    ATTACK,
    PAIN_A,
    PAIN_B,
    PAIN_C,
    JUMP,
    FLIP,
    SALUTE,
    FALLBACK,
    WAVE,
    POINTING,
    CROUCH_STAND,
    CROUCH_WALK,
    CROUCH_ATTACK,
    CROUCH_PAIN,
    CROUCH_DEATH,
    DEATH_FALLBACK,
    DEATH_FALLFORWARD,
    DEATH_FALLBACKSLOW,
    BOOM,

    MAX_ANIMATIONS

} animationType ;

class MD2Loader
{
public:
    /*
     * @brief Default MD2Loader constructor
     *
     * The default constructor for an MD2Model without
     * any parameters
     *
     * @return void
     */
    MD2Loader();

    /*
     * @brief Loads in an md2 formatted model
     *
     * Loads in the data stored in an md2 file as well
     * as its texture
     *
     * @param modelFile, textureFile
     * @return void
     */
    void loadModel(const char* modelFile, const char* textureFile);

    /*
     * @brief Renders the model
     *
     * This functions renders the model in its
     * current animation state
     *
     * @param animState, shader
     * @return void
     */
    void renderModel(animationState* animState, Shader& shader);

    /*
     * @brief Starts a given animation
     *
     * This function starts an animation type
     * as received through a paramater
     *
     * @param type
     * @return void
     */
    animationState startAnimation(animationType type);

    /*
     * @brief Updates the animation state
     *
     * This functions updates the animation
     * as per the amount of time that has passed
     * since the last update
     *
     * @param animState, fTimePassed
     * @return void
     */
    void updateAnimation(animationState* animState, float fTimePassed);

    /*
     * @brief Gets the position of a model
     *
     * This functions gets the position of the
     * md2 model
     *
     * @return void
     */
    glm::vec3 getPos();

    /*
     * @brief Sets the position of the model
     *
     * This function sets the position of the model
     * as per a given location
     *
     * @return void
     */
    void setPos(glm::vec3 tempPos);

    /*
     * @brief Gets the animation status
     *
     * This function gets the status of the animation
     * in terms of whether or not it has finished
     *
     * @return void
     */
    bool getAnimationStatus();

    /// Stores list of animations
    static animationState animationList[21];
    /// Stores the model shader
    Shader shader;
private:
    /// Stores the texture vertex array object
    unsigned int VAO;
    /// Stores the id of the stored texture
    unsigned int texId;
    /// Stores the model position
    glm::vec3 position;
    /// Stores an md2 files header data
    md2_t header;
    /// Stores the vertices for every frame of the model
    std::vector< std::vector<glm::vec3> > vVertices;
    /// Stores the normals for every frame of the model
    std::vector <std::vector< int > > vNormals;
    /// Stores the gl commands stored in the md2 file
    std::vector<int> glCommands;
    /// Stores the vertex data the vertex buffer objects of the model
    std::vector<VertexBuffer> vboFrameVerts;
    /// Stores the texture coordinates of the vertex buffer object
    VertexBuffer vboTexCoords;
    /// Stores a list of render modes
    std::vector<int> renderModes;
    /// Stores a list of the number of rendered vertices
    std::vector<int> numRenderVertices;
    /// Stores a check for whether an animation has finished or not
    bool finishedAnimation;
};
#endif
