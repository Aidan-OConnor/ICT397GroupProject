#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    bDataUploaded = false;
    vbo = 0;
}

void VertexBuffer::CreateVertexBuffer(int s)
{
    glGenBuffers(1, &vbo);
    data.reserve(s);
    iSize = s;
    iCurrentSize = 0;
}

void VertexBuffer::DeleteVertexBuffer()
{
    glDeleteBuffers(1, &vbo);
    bDataUploaded = false;
    data.clear();
}

void* VertexBuffer::MapBufferToMemory(int iUsageHint)
{
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
    return ptrRes;
}


void* VertexBuffer::MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength)
{
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
    return ptrRes;
}

void VertexBuffer::UnmapBuffer()
{
    glUnmapBuffer(iBufferType);
}

void VertexBuffer::BindVBO(int a_iBufferType)
{
    iBufferType = a_iBufferType;
    glBindBuffer(iBufferType, vbo);
}

void VertexBuffer::UploadDataToGPU(int iDrawingHint)
{
    glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
    bDataUploaded = true;
    data.clear();
}

void VertexBuffer::AddData(void* ptrData, unsigned int uiDataSize)
{
    data.insert(data.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + uiDataSize);
    iCurrentSize += uiDataSize;
}

void* VertexBuffer::GetDataPointer()
{
    if (bDataUploaded)return NULL;
    return (void*)data[0];
}

unsigned int VertexBuffer::GetBufferID()
{
    return vbo;
}

int VertexBuffer::GetCurrentSize()
{
    return iCurrentSize;
}