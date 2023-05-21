#pragma once
#include <sol/sol.hpp>
#include "ImGuiData.h"

/*
 * @class LuaData
 * @brief Contains data loaded from Lua
 *
 * LuaData stores the data loaded in from a Lua script
 * to be converted into objects to be rendered for the scene
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 21/4/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

class LuaData
{
private:
    /// Stores a sol state to embed Lua to C++
	sol::state lua;
    /// Stores all of the data loaded in from Lua
    std::vector<objectData> luaMap;
    /// Stores a tempory string
    std::string tempName;
    /// Stores temporary object data to be pushed back into a vector
    ImGuiData tempGuiData;
public:
    /*
     * @brief Loads in and stores data from Lua
     *
     * This function loads in map data from a Lua script
     * and stores it in a more useable data type
     *
     * @param convertedData
     * @return void
     */
    void loadData(std::vector<ImGuiData>& convertedData, std::string fileName)
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.new_usertype<objectData>("objectData", sol::constructors<void()>(),
            "filepath", &objectData::filepath, "texturePath", &objectData::texturePath, "objectType", &objectData::objectType,
            "iterations", &objectData::iterations, "width", &objectData::width,
            "length", &objectData::length, "minHeight", &objectData::minHeight,
            "maxHeight", &objectData::maxHeight, "filter", &objectData::filter,
            "tx", &objectData::tx, "ty", &objectData::ty, "tz", &objectData::tz,
            "sx", &objectData::sx, "sy", &objectData::sy, "sz", &objectData::sz,
            "rx", &objectData::rx, "ry", &objectData::ry, "rz", &objectData::rz
        );

        lua.script_file(fileName);

        int numOfTerrain = lua["numTerrains"];
        int numOfWater = lua["numWater"];
        int numOfModel = lua["numModels"];

        for (int i = 0; i < numOfTerrain; i++)
        {
            tempName = "terrain" + std::to_string(i);
            const char* terrainName = tempName.c_str();
            objectData& tempObject = lua[terrainName];
            luaMap.push_back(tempObject);
        }

        for (int i = 0; i < numOfWater; i++)
        {
            tempName = "water" + std::to_string(i);
            const char* waterName = tempName.c_str();
            objectData& tempObject1 = lua[waterName];
            luaMap.push_back(tempObject1);
        }

        for (int i = 0; i < numOfModel; i++)
        {
            tempName = "model" + std::to_string(i);
            const char* modelName = tempName.c_str();
            objectData& tempObject2 = lua[modelName];
            luaMap.push_back(tempObject2);
        }

        for (int i = 0; i < luaMap.size(); i++)
        {
            std::string objectType = luaMap[i].objectType;

            if (objectType == "Terrain")
            {
                Landscape tempTerrain;
                tempTerrain.loadFromHeightmap(luaMap[i].filepath, 1, luaMap[i].texturePath, GL_TEXTURE_2D);
                tempTerrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                tempGuiData.setTerrain(tempTerrain);
                tempGuiData.setObjectType(luaMap[i].objectType);
                tempGuiData.setFilePath(luaMap[i].filepath);
                tempGuiData.setTexturePath(luaMap[i].texturePath);
            }
            else if (objectType == "Water")
            {
                Landscape tempTerrain;
                tempTerrain.loadFromFaultFormation(luaMap[i].iterations, luaMap[i].width, luaMap[i].length,
                    1, 1, luaMap[i].minHeight, luaMap[i].maxHeight, luaMap[i].filter,
                    luaMap[i].texturePath, GL_TEXTURE_2D);
                tempGuiData.setTerrain(tempTerrain);
                tempGuiData.setObjectType(luaMap[i].objectType);
                tempGuiData.setTexturePath(luaMap[i].texturePath);
                tempGuiData.setNumIterations(luaMap[i].iterations);
                tempGuiData.setWidth(luaMap[i].width);
                tempGuiData.setLength(luaMap[i].length);
                tempGuiData.setMinHeight(luaMap[i].minHeight);
                tempGuiData.setMaxHeight(luaMap[i].maxHeight);
                tempGuiData.setFilter(luaMap[i].filter);
            }
            else
            {
                tempGuiData.setModel(luaMap[i].filepath);
                tempGuiData.setObjectType(luaMap[i].objectType);
                tempGuiData.setFilePath(luaMap[i].filepath);
            }

            glm::vec3 tempVec;

            tempVec.x = luaMap[i].tx;
            tempVec.y = luaMap[i].ty;
            tempVec.z = luaMap[i].tz;
            tempGuiData.setTranslation(tempVec);

            tempVec.x = luaMap[i].sx;
            tempVec.y = luaMap[i].sy;
            tempVec.z = luaMap[i].sz;
            tempGuiData.setScale(tempVec);

            tempVec.x = luaMap[i].rx;
            tempVec.y = luaMap[i].ry;
            tempVec.z = luaMap[i].rz;
            tempGuiData.setRotation(tempVec);

            convertedData.push_back(tempGuiData);
        }
    }
};