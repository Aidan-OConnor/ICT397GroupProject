#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Texture.h"
#include <vector>

/*
     * @class VertexBuffer
     * @brief Stores vertex buffer data
     *
     * Vertex buffer stores data and contains functions for managing
     * vertex buffers required for md2 data loading
     *
     * @author Aidan O'Connor
     * @version 01
     * @date 09/06/2023 Aidan O'Connor, finished
     *
     * @bug No bugs have currently been found
     */

class VertexBuffer
{
public:
    /*
     * @brief Default constructor
     *
     * Default constructor for a vertex buffer
     *
     * @return void
     */
    VertexBuffer();

    /*
     * @brief Creates a new vertex buffer
     *
     * This function generates a new vertex buffer
     *
     * @param s
     * @return void
     */
    void CreateVertexBuffer(int s = 0);

    /*
     * @brief Deletes a vertex buffer
     *
     * This function deletes an already generated
     * vertex buffer
     *
     * @return void
     */
    void DeleteVertexBuffer();

    /*
     * @brief Maps buffer data to memory
     *
     * This function maps the buffer data to memory
     *
     * @return void
     */
    void* MapBufferToMemory(int iUsageHint);

    /*
     * @brief Maps buffer data to memory
     *
     * This function maps the buffer data to memory
     * using a range
     *
     * @return void
     */
    void* MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength);

    /*
     * @brief Unmaps buffered data
     *
     * Unmaps previously mapped data
     *
     * @return void
     */
    void UnmapBuffer();

    /*
     * @brief Binds VBO
     *
     * This function binds the vertex buffer object
     *
     * @param a_iBufferType
     * @return void
     */
    void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);

    /*
     * @brief Initializes stored buffer data
     *
     * This function initializes stored buffer data
     * to the GPu
     *
     * @param iUsageHint
     * @return void
     */
    void UploadDataToGPU(int iUsageHint);

    /*
     * @brief Adds data to the buffer
     *
     * This function inserts more data
     *
     * @param ptrData, uiDataSize
     * @return void
     */
    void AddData(void* ptrData, unsigned int uiDataSize);

    /*
     * @brief Gets pointer to data
     *
     * This returns a pointer to the first element
     * of the stored data
     *
     * @return (void*)data[0]
     */
    void* GetDataPointer();

    /*
     * @brief Gets the buffer ID
     *
     * This function returns the buffer ID
     *
     * @return vbo
     */
    unsigned int GetBufferID();

    /*
     * @brief Returns the size of the buffer
     *
     * This function returns the current size of the
     * vertex buffer
     *
     * @return iCurrentSize
     */
    int GetCurrentSize();

private:
    /// Vertex buffer ID
    unsigned int vbo;
    /// Current size of buffer
    int iCurrentSize;
    /// Stores the buffer type
    int iBufferType;
    /// Stores the buffer data
    std::vector<unsigned char> data;
    /// Bool for status of the datas upload
    bool bDataUploaded;
};
#endif