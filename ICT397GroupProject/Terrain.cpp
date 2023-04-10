#pragma once
#include "Terrain.h"

Terrain::Terrain(const char* fileName, float scale)
{
    heightMap = stbi_load(fileName, &width, &height, &channels, 0);
    rez = 1;

    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    unsigned bytePerPixel = channels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = heightMap + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            glm::vec3 tempVert;

            tempVert.x = ((-height / 2.0f + height * i / (float)height) * scale);
            tempVert.y = ((int)y * yScale - yShift);
            tempVert.z = ((-width / 2.0f + width * j / (float)width) * scale);

            vertices.push_back(tempVert);
        }
    }

    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }

    numOfStrips = (height - 1) / rez;
    numOfTrisPerStrip = (width / rez) * 2 - 2;

    initVAO();
}

Terrain::Terrain(float iterations, float minHeight, float maxHeight, float filter)
{
    const int faultWidth = 400;
    const int faultHeight = 400;

    float faultHeights[faultWidth][faultHeight];

    getHeights(faultHeights, faultWidth, faultHeight, iterations, minHeight, maxHeight);
    applyFilter(faultHeights, faultWidth, faultHeight, iterations, filter);

    for (int i = 0; i < faultWidth; i++) {
        for (int j = 0; j < faultHeight; j++) {
            glm::vec3 tempVert;

            tempVert.x = ((-faultHeight / 2.0f + faultHeight * i / (float)faultHeight));
            tempVert.y = faultHeights[i][j];
            tempVert.z = ((-faultWidth / 2.0f + faultWidth * j / (float)faultWidth));

            vertices.push_back(tempVert);
        }
    }

    normaliseHeights(minHeight, maxHeight);

    rez = 1;

    for (unsigned i = 0; i < faultHeight - 1; i += rez)
    {
        for (unsigned j = 0; j < faultWidth; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + faultWidth * (i + k * rez));
            }
        }
    }

    numOfStrips = (faultHeight - 1) / rez;
    numOfTrisPerStrip = (faultWidth / rez) * 2 - 2;

    initVAO();
}

void Terrain::initVAO()
{
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
}

float Terrain::getMaxY(std::vector<glm::vec3> list)
{
    float max = list[1].y;

    for (int i = 1; i < list.size(); i++)
    {
        if (list[i].y > max)
        {
            max = list[i].y;
        }
    }

    return max;
}

float Terrain::getMinY(std::vector<glm::vec3> list)
{
    float min = list[1].y;

    for (int i = 1; i < list.size(); i++)
    {
        if (list[i].y < min)
        {
            min = list[i].y;
        }
    }

    return min;
}

void Terrain::loadHeightMap(const char* fileName, float scale)
{
    heightMap = stbi_load(fileName, &width, &height, &channels, 0);
    rez = 1;

    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    unsigned bytePerPixel = channels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = heightMap + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            glm::vec3 tempVert;

            tempVert.x = ((-height / 2.0f + height * i / (float)height) * scale);
            tempVert.y = ((int)y * yScale - yShift);
            tempVert.z = ((-width / 2.0f + width * j / (float)width) * scale);

            vertices.push_back(tempVert);
        }
    }

    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }

    numOfStrips = (height - 1) / rez;
    numOfTrisPerStrip = (width / rez) * 2 - 2;

    initVAO();
}

void Terrain::getHeights(float faultHeights[][400], int faultWidth, int faultHeight, float iterations, float minHeight, float maxHeight)
{
    for (int i = 0; i < faultWidth; i++)
    {
        for (int j = 0; j < faultHeight; j++)
        {
            faultHeights[i][j] = 0;
        }
    }

    float deltaHeight = minHeight - maxHeight;

    for (int i = 0; i < iterations; i++) {
        float IterationRatio = ((float)i / (float)iterations);
        float Height = maxHeight - IterationRatio * deltaHeight;

        glm::vec2 temp1, temp2;

        temp1.x = rand() % faultWidth;
        temp1.y = rand() % faultHeight;

        temp2.x = rand() % faultWidth;
        temp2.y = rand() % faultHeight;

        int DirX = temp2.x - temp1.x;
        int DirZ = temp2.y - temp1.y;

        for (int z = 0; z < faultWidth; z++) {
            for (int x = 0; x < faultHeight; x++) {
                int DirX_in = x - temp1.x;
                int DirZ_in = z - temp1.y;

                int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;

                if (CrossProduct > 0) {
                    float CurHeight = faultHeights[z][x];
                    faultHeights[z][x] = CurHeight + Height;
                }
            }
        }
    }
}

void Terrain::applyFilter(float faultHeights[][400], int faultWidth, int faultHeight, float iterations, float filter)
{
    for (int z = 0; z < faultHeight; z++) {
        float PrevVal = faultHeights[0][z];
        for (int x = 1; x < faultWidth; x++) {
            float CurVal = faultHeights[x][z];
            float NewVal = filter * PrevVal + (1 - filter) * CurVal;
            faultHeights[x][z] = NewVal;
        }
    }

    for (int z = 0; z < faultHeight; z++) {
        float PrevVal = faultHeights[faultWidth - 1][z];
        for (int x = faultWidth - 2; x >= 0; x--) {
            float CurVal = faultHeights[x][z];
            float NewVal = filter * PrevVal + (1 - filter) * CurVal;
            faultHeights[x][z] = NewVal;
        }
    }

    for (int x = 0; x < faultWidth; x++) {
        float PrevVal = faultHeights[x][0];
        for (int z = 1; z < faultHeight; z++) {
            float CurVal = faultHeights[x][z];
            float NewVal = filter * PrevVal + (1 - filter) * CurVal;
            faultHeights[x][z] = NewVal;
        }
    }

    for (int x = 0; x < faultWidth; x++) {
        float PrevVal = faultHeights[x][faultHeight - 1];
        for (int z = faultHeight - 2; z >= 0; z--) {
            float CurVal = faultHeights[x][z];
            float NewVal = filter * PrevVal + (1 - filter) * CurVal;
            faultHeights[x][z] = NewVal;
        }
    }
}

void Terrain::normaliseHeights(float maxHeight, float minHeight)
{
    float maxY = getMaxY(vertices);
    float minY = getMinY(vertices);

    if (maxY > minY)
    {
        float minMaxDelta = maxY - minY;
        float minMaxRange = maxHeight - minHeight;

        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i].y = ((vertices[i].y - minY) / minMaxDelta) * minMaxRange + minHeight;
        }
    }
}

void Terrain::generateFaultFormation(float iterations, float minHeight, float maxHeight, float filter)
{
    const int faultWidth = 400;
    const int faultHeight = 400;

    float faultHeights[faultWidth][faultHeight];

    getHeights(faultHeights, faultWidth, faultHeight, iterations, minHeight, maxHeight);
    applyFilter(faultHeights, faultWidth, faultHeight, iterations, filter);

    for (int i = 0; i < faultWidth; i++) {
        for (int j = 0; j < faultHeight; j++) {
            glm::vec3 tempVert;

            tempVert.x = ((-faultHeight / 2.0f + faultHeight * i / (float)faultHeight));
            tempVert.y = faultHeights[i][j];
            tempVert.z = ((-faultWidth / 2.0f + faultWidth * j / (float)faultWidth));

            vertices.push_back(tempVert);
        }
    }

    normaliseHeights(minHeight, maxHeight);

    rez = 1;

    for (unsigned i = 0; i < faultHeight - 1; i += rez)
    {
        for (unsigned j = 0; j < faultWidth; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + faultWidth * (i + k * rez));
            }
        }
    }

    numOfStrips = (faultHeight - 1) / rez;
    numOfTrisPerStrip = (faultWidth / rez) * 2 - 2;

    initVAO();
}

void Terrain::renderTerrain(bool renderType)
{
    glBindVertexArray(this->terrainVAO);

    if (renderType)
    {
        for (unsigned strip = 0; strip < numOfStrips; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP, numOfTrisPerStrip + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned) * (numOfTrisPerStrip + 2) * strip));
        }
    }
    else
    {
        for (unsigned strip = 0; strip < numOfStrips; strip++)
        {
            glDrawElements(GL_LINE_STRIP, numOfTrisPerStrip + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned) * (numOfTrisPerStrip + 2) * strip));
        }
    }
}

std::vector<glm::vec3> Terrain::getVertices()
{
    return vertices;
}
