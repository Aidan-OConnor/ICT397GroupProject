#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Texture.h"
#include <vector>
class VertexBuffer
{
public:
    VertexBuffer();
    void CreateVertexBuffer(int s = 0);
    void DeleteVertexBuffer();

    void* MapBufferToMemory(int iUsageHint);
    void* MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength);
    void UnmapBuffer();

    void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
    void UploadDataToGPU(int iUsageHint);

    void AddData(void* ptrData, unsigned int uiDataSize);

    void* GetDataPointer();
    unsigned int GetBufferID();

    int GetCurrentSize();

private:
    unsigned int vbo;
    int iSize;
    int iCurrentSize;
    int iBufferType;
    std::vector<unsigned char> data;

    bool bDataUploaded;
};
#endif