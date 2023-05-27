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
    MD2Loader();

    void loadModel(const char* modelFile, const char* textureFile);

    void renderModel(animationState* animState, glm::vec3 position, float rotation, float direction, Shader& shader);

    animationState startAnimation(animationType type);
    void updateAnimation(animationState* animState, float fTimePassed);
    glm::vec3 getPos();
    void setPos(glm::vec3 tempPos);

    static animationState animationList[21];
    Shader shader;
private:
    unsigned int modelVAO, VAO, VBO, EBO, texId;

    glm::vec3 position;

    std::vector<unsigned int> uiFramesBuffer;

    md2_t header;
    std::vector< std::vector<glm::vec3> > vVertices;
    std::vector <std::vector< int > > vNormals;
    std::vector<int> glCommands;

    std::vector<VertexBuffer> vboFrameVerts;
    VertexBuffer vboTexCoords;

    std::vector<int> renderModes;
    std::vector<int> numRenderVertices;
};
#endif
