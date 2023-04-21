#pragma once
#include <sol/sol.hpp>
#include "ImGuiData.h"

struct objectData
{
    const char* filepath;
    const char* texturePath;
    const char* objectType;
    float iterations, width, length, minHeight, maxHeight, filter;
    float tx, ty, tz;
    float sx, sy, sz;
    float rx, ry, rz;

    objectData() {};
};

class LuaData
{
private:
	sol::state lua;
    std::vector<objectData> luaMap;
    std::string tempName;
    ImGuiData tempGuiData;
public:
    void loadData(std::vector<ImGuiData>& convertedData)
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

        lua.script_file("MapData.lua");

        objectData& tempObject = lua["heightmap"];
        luaMap.push_back(tempObject);

        objectData& tempObject1 = lua["waterFormation"];
        luaMap.push_back(tempObject1);

        for (int i = 0; i < 30; i++)
        {
            tempName = "model" + std::to_string(i + 1);
            const char* modelName = tempName.c_str();
            objectData& tempObject2 = lua[modelName];
            luaMap.push_back(tempObject2);
        }

        for (int i = 0; i < luaMap.size(); i++)
        {
            if (i == 0)
            {
                Landscape tempTerrain;
                tempTerrain.loadFromHeightmap(luaMap[i].filepath, 1, luaMap[i].texturePath, GL_TEXTURE_2D);
                tempTerrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                tempGuiData.setTerrain(tempTerrain);
                tempGuiData.setObjectType(luaMap[i].objectType);
            }
            else if (i == 1)
            {
                Landscape tempTerrain;
                tempTerrain.loadFromFaultFormation(luaMap[i].iterations, luaMap[i].width, luaMap[i].length,
                    1, 1, luaMap[i].minHeight, luaMap[i].maxHeight, luaMap[i].filter,
                    luaMap[i].texturePath, GL_TEXTURE_2D);
                tempGuiData.setTerrain(tempTerrain);
                tempGuiData.setObjectType(luaMap[i].objectType);
            }
            else
            {
                tempGuiData.setModel(luaMap[i].filepath);
                tempGuiData.setObjectType(luaMap[i].objectType);
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