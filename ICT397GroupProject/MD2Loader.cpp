#include "MD2Loader.h"
MD2Loader::MD2Loader() {}

animationState MD2Loader::animationList[21] =
{
    // first, last, fps
    {   0,  39,  9 },	// STAND
    {  40,  45, 10 },	// RUN
    {  46,  53, 10 },	// ATTACK
    {  54,  57,  7 },	// PAIN_A
    {  58,  61,  7 },	// PAIN_B
    {  62,  65,  7 },	// PAIN_C
    {  66,  71,  7 },	// JUMP
    {  72,  83,  7 },	// FLIP
    {  84,  94,  7 },	// SALUTE
    {  95, 111, 10 },	// FALLBACK
    { 112, 122,  7 },	// WAVE
    { 123, 134,  6 },	// POINTIING
    { 135, 153, 10 },	// CROUCH_STAND
    { 154, 159,  7 },	// CROUCH_WALK
    { 160, 168, 10 },	// CROUCH_ATTACK
    { 196, 172,  7 },	// CROUCH_PAIN
    { 173, 177,  5 },	// CROUCH_DEATH
    { 178, 183,  7 },	// DEATH_FALLBACK
    { 184, 189,  7 },	// DEATH_FALLFORWARD
    { 190, 197,  7 },	// DEATH_FALLBACKSLOW
    { 198, 198,  5 },	// BOOM
};

std::string sMD2AnimationNames[MAX_ANIMATIONS] =
{
        "Stand",
        "Run",
        "Attack",
        "Pain A",
        "Pain B",
        "Pain C",
        "Jump",
        "Flip",
        "Salute",
        "Fallback",
        "Wave",
        "Pointing",
        "Crouch Stand",
        "Crouch Walk",
        "Crouch Attack",
        "Crouch Pain",
        "Crouch Death",
        "Death Fallback",
        "Death Fall Forward",
        "Death Fallback Slow",
        "Boom"
};

glm::vec3 anorms[162] =
{
    #include "anorms.h"
};

void MD2Loader::loadModel(const char* modelFile, const char* textureFile)
{
    FILE* fp;

    fopen_s(&fp, modelFile, "rb");

    fread(&this->header, sizeof(md2_t), 1, fp);

    char* buffer = new char[this->header.numFrames * this->header.frameSize];
    fseek(fp, this->header.ofsFrames, SEEK_SET);
    fread(buffer, sizeof(char), this->header.numFrames * this->header.frameSize, fp);

    this->vVertices.resize(this->header.numFrames, std::vector<glm::vec3>(this->header.numVerts));
    this->vNormals.resize(this->header.numFrames, std::vector<int>(this->header.numVerts));

    for (int i = 0; i < this->header.numFrames; i++)
    {
        frame_md2* frame_ptr = (frame_md2*)&buffer[this->header.frameSize * i];

        for (int j = 0; j < this->header.numVerts; j++)
        {
            this->vVertices[i][j].x = frame_ptr->translate[0] + (float(frame_ptr->verts[j].v[0]) * frame_ptr->scale[0]);
            this->vVertices[i][j].y = frame_ptr->translate[1] + (float(frame_ptr->verts[j].v[1]) * frame_ptr->scale[1]);
            this->vVertices[i][j].z = frame_ptr->translate[2] + (float(frame_ptr->verts[j].v[2]) * frame_ptr->scale[2]);

            this->vNormals[i][j] = frame_ptr->verts[j].lightNormIndex;
        }
    }

    this->glCommands.resize(this->header.numCmds);
    fseek(fp, this->header.ofsCmds, SEEK_SET);
    fread(&this->glCommands[0], sizeof(int), this->header.numCmds, fp);

    int i = 0;
    int iTotalVertices = 0;

    this->vboFrameVerts.resize(this->header.numFrames);
    for (int i = 0; i < this->header.numFrames; i++)
    {
        this->vboFrameVerts[i].CreateVertexBuffer();
    }
    this->vboTexCoords.CreateVertexBuffer();
    int x = 0;
    while (true)
    {
        x++;
        int action = this->glCommands[i];
        if (action == 0)
            break;

        int renderMode = action < 0 ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP;
        int numVertices = action < 0 ? -action : action;
        i++;

        this->renderModes.push_back(renderMode);
        this->numRenderVertices.push_back(numVertices);

        for (int j = 0; j < numVertices; j++)
        {
            float s = *((float*)(&this->glCommands[i++]));
            float t = *((float*)(&this->glCommands[i++]));
            t = 1.0f - t;
            int vi = this->glCommands[i++];

            this->vboTexCoords.AddData(&s, 4);
            this->vboTexCoords.AddData(&t, 4);

            for (int k = 0; k < this->header.numFrames; k++)
            {
                this->vboFrameVerts[k].AddData(&this->vVertices[k][vi], 12);
                this->vboFrameVerts[k].AddData(&anorms[this->vNormals[k][vi]], 12);
            }
        }
    }

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    for (int i = 0; i < this->header.numFrames; i++)
    {
        this->vboFrameVerts[i].BindVBO();
        this->vboFrameVerts[i].UploadDataToGPU(GL_STATIC_DRAW);
    }

    this->vboFrameVerts[0].BindVBO();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

    this->vboTexCoords.BindVBO();
    this->vboTexCoords.UploadDataToGPU(GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    Texture texture;
    this->texId = texture.TextureFromFile(textureFile);
}


void MD2Loader::renderModel(animationState* animState, glm::vec3 position, float rotation, float direction, Shader& shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texId);

    shader.use();
    shader.setInt("texture0", 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.1f));
    model = glm::rotate(model, rotation, glm::vec3(1, 0, 0));
    model = glm::rotate(model, direction, glm::vec3(0, 0, 1));
    shader.setMat4("model", model);
    shader.setMat4("normal", model);

    glBindVertexArray(this->VAO);
    int iTotalOffset = 0;

    if (animState == NULL)
    {
        glEnableVertexAttribArray(0);
        this->vboFrameVerts[0].BindVBO();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
        shader.setFloat("fInterpolation", -1.0f);

        for (int i = 0; i < this->renderModes.size(); i++)
        {
            glDrawArrays(this->renderModes[i], iTotalOffset, this->numRenderVertices[i]);
            iTotalOffset += this->numRenderVertices[i];
        }
    }
    else
    {
        glEnableVertexAttribArray(0);
        this->vboFrameVerts[animState->currFrame].BindVBO();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

        glEnableVertexAttribArray(3);
        this->vboFrameVerts[animState->nextFrame].BindVBO();
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

        glEnableVertexAttribArray(2);
        this->vboFrameVerts[animState->currFrame].BindVBO();
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

        glEnableVertexAttribArray(4);
        this->vboFrameVerts[animState->nextFrame].BindVBO();
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

        shader.setFloat("fInterpolation", animState->interpol);
        for (int i = 0; i < this->renderModes.size(); i++)
        {
            glDrawArrays(this->renderModes[i], iTotalOffset, this->numRenderVertices[i]);
            iTotalOffset += this->numRenderVertices[i];
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

animationState MD2Loader::startAnimation(animationType type)
{
    animationState animState;
    animState.startFrame = this->animationList[type].startFrame;
    animState.endFrame = this->animationList[type].endFrame;
    animState.currFrame = this->animationList[type].startFrame;
    animState.nextFrame = this->animationList[type].startFrame + 1;

    animState.fps = this->animationList[type].fps;
    animState.type = type;

    animState.currTime = 0.0f;
    animState.prevTime = 0.0f;

    animState.interpol = 0.0f;

    return animState;
}

void MD2Loader::updateAnimation(animationState* animState, float fTimePassed)
{
    animState->currTime += fTimePassed;

    if (animState->currTime - animState->prevTime > (1.0f / float(animState->fps)))
    {
        animState->prevTime = animState->currTime;

        animState->currFrame = animState->nextFrame;
        animState->nextFrame++;
        if (animState->nextFrame > animState->endFrame)
            animState->nextFrame = animState->startFrame;
    }

    animState->interpol = float(animState->fps) * (animState->currTime - animState->prevTime);
}

glm::vec3 MD2Loader::getPos()
{
    return this->position;
}

void MD2Loader::setPos(glm::vec3 tempPos)
{
    this->position = tempPos;
}
