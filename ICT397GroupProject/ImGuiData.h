#pragma once
#include "Landscape.h"
#include "model.h"

/*
 * @class ImGuiData
 * @brief Contains data and functions for loading and rendering objects
 *
 * ImGuiData stores all the data related to objects that have been loaded
 * in from ImGui or are to be rendered using locations generated by ImGui,
 * as well as the functions necessary to load the GUI and create new objects
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 21/4/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

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

class ImGuiData
{
private:
    /// Stores the iterations for a fault formation
    float iterations;
    /// Stores the width for a fault formation
    float width;
    /// Stores the length for a fault formation
    float length;
    /// Stores the minimum height for a fault formation
    float minHeight;
    /// Stores the maximum height for a fault formation
    float maxHeight; 
    /// Stores the filter for a fault formation
    float filter;
    /// Stores the data types file path as a const char *
    std::string filepath;
    /// Stores the data types texture path as a const char *
    std::string texturePath;
    /// Stores the objects type in a string
    std::string objectType;
    /// Stores the landscape type in a string
    std::string landscapeType;
    /// Stores a landscape for a terrain
    Landscape terrain;
    /// Stores a landscape for water
    Landscape water;
    /// Stores a temporary landscape for pushing data
    Landscape tempLandscape;
    /// Stores a model
    Model* model;
    /// Stores the object name
    const char* objectName;
    /// Stores the 3D translation of an object
    glm::vec3 translation;
    /// Stores the 3D scale of an object
    glm::vec3 scale;
    /// Stores the 3D rotation of an object
    glm::vec3 rotation;
    /// Stores all of the loaded objects
    std::vector<ImGuiData> imGuiObjects;
    /// Stores all of the data loaded in from Lua
    std::vector<objectData> luaMap;
    /// Stores a tempory string
    std::string tempName;
public:
    /// Temporary terrain for pushing data
    Terrain temp;
    /// Default height scale for a terrain
    int heightScale = 1;

    /*
     * @brief Default ImGuiData constructor
     *
     * The default constructor for an ImGuiObject that
     * initializes some necessary values to 0
     *
     * @return void
     */
    ImGuiData()
    {
        this->iterations = this->width = this->length = 
            this->minHeight = this->maxHeight = this->filter = 0;
    }

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
        ImGuiData tempGuiData;
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
            if (i == 0)
            {
                Landscape tempTerrain;
                tempTerrain.loadFromHeightmap(luaMap[i].filepath, 1, luaMap[i].texturePath, GL_TEXTURE_2D);
                tempTerrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                tempGuiData.setTerrain(tempTerrain);
                tempGuiData.setObjectType(luaMap[i].objectType);
                tempGuiData.setFilePath(luaMap[i].filepath);
                tempGuiData.setTexturePath(luaMap[i].texturePath);
            }
            else if (i == 1)
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

    int getNumType(std::string modelType)
    {
        int numOfType = 0;

        for (int i = 0; i < imGuiObjects.size(); i++)
        {
            if (imGuiObjects[i].objectType == modelType)
            {
                numOfType++;
            }
        }

        return numOfType;
    }

    /*
     * @brief Renders the UI
     *
     * This functions renders all of the UI for the scene
     * to allows users to customize their map.
     *
     * @return void
     */
    void RenderUI()
    {
        ImGui::Begin("Control Centre");

        if (ImGui::TreeNode("Create Terrain"))
        {
            if (ImGui::TreeNode("Heightmap"))
            {
                if (ImGui::Button("Generate", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;

                    ImTemp.objectType = "Terrain";
                    ImTemp.landscapeType = "Heightmap";
                    ImTemp.terrain.loadFromHeightmap("Terrains/VolcanoType11.png", 1, "Images/sand2.jpg", GL_TEXTURE_2D);
                    ImTemp.terrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Terrains/VolcanoType11.png";
                    ImTemp.texturePath = "Images/sand2.jpg";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Fault Formation"))
            {
                ImGui::InputFloat("Iterations", &iterations);
                ImGui::InputFloat("Width", &width);
                ImGui::InputFloat("Length", &length);
                ImGui::InputFloat("Minimim height", &minHeight);
                ImGui::InputFloat("Maximum height ", &maxHeight);
                ImGui::InputFloat("Filter", &filter);

                if (ImGui::Button("Generate", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Terrain";
                    ImTemp.landscapeType = "Formation";
                    ImTemp.terrain.loadFromFaultFormation(iterations, width, length, 1, 1, minHeight, maxHeight, filter, "Images/sand2.jpg", GL_TEXTURE_2D);
                    ImTemp.terrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.texturePath = "Images/sand2.jpg";

                    imGuiObjects.push_back(ImTemp);

                    iterations = width = length = minHeight = maxHeight = filter = 0;
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Create Water"))
        {
            if (ImGui::TreeNode("Fault Formation"))
            {
                ImGui::InputFloat("Iterations", &iterations);
                ImGui::InputFloat("Width", &width);
                ImGui::InputFloat("Length", &length);
                ImGui::InputFloat("Minimim height", &minHeight);
                ImGui::InputFloat("Maximum height ", &maxHeight);
                ImGui::InputFloat("Filter", &filter);

                if (ImGui::Button("Generate", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Water";
                    ImTemp.landscapeType = "Formation";
                    ImTemp.terrain.loadFromFaultFormation(iterations, width, length, 1, 1, minHeight, maxHeight, filter, "Images/Water1.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.texturePath = "Images/Water1.jpg";

                    imGuiObjects.push_back(ImTemp);

                    iterations = width = length = minHeight = maxHeight = filter = 0;
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Load Model"))
        {
            if (ImGui::TreeNode("Bruiser T-Pose"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/Bruiser/bruiserTpose.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/Bruiser/bruiserTpose.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Bruiser Stance"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/Bruiser/bruiserStance.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/Bruiser/bruiserStance.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Boat"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/NewBoat/boat.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/NewBoat/boat.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Palm Tree"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/PalmTree/palmtree.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/PalmTree/palmtree.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Rock1"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/Rocks/rock.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/Rocks/rock.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Rock2"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/Rocks/rock2.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/Rocks/rock2.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Rock3"))
            {
                if (ImGui::Button("Load", ImVec2(100, 25)))
                {
                    ImGuiData ImTemp;
                    ImTemp.objectType = "Model";
                    ImTemp.model = new Model("Models/Rocks/rock3.obj");
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };
                    ImTemp.filepath = "Models/Rocks/rock3.obj";

                    imGuiObjects.push_back(ImTemp);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Object Data"))
        {
            if (imGuiObjects.size() != 0)
            {
                for (int i = 0; i < imGuiObjects.size(); i++)
                {
                    std::string tempName;
                    if (imGuiObjects[i].objectType == "Terrain")
                    {
                        tempName = "Terrain" + std::to_string(i);
                    }
                    else if (imGuiObjects[i].objectType == "Water")
                    {
                        tempName = "Water" + std::to_string(i);
                    }
                    else if (imGuiObjects[i].objectType == "Model")
                    {
                        tempName = "Model" + std::to_string(i);
                    }

                    imGuiObjects[i].objectName = tempName.c_str();

                    if (ImGui::TreeNode(imGuiObjects[i].objectName))
                    {
                        ImGui::DragFloat("PositionX", &imGuiObjects[i].translation[0], 0.5, -1000, 1000);
                        ImGui::DragFloat("PositionY", &imGuiObjects[i].translation[1], 0.5, -1000, 1000);
                        ImGui::DragFloat("PositionZ", &imGuiObjects[i].translation[2], 0.5, -1000, 1000);
                        ImGui::InputFloat3("Position", &imGuiObjects[i].translation[0]);
                        ImGui::DragFloat("ScaleX", &imGuiObjects[i].scale[0], 0.005, -50, 50);
                        ImGui::DragFloat("ScaleY", &imGuiObjects[i].scale[1], 0.005, -50, 50);
                        ImGui::DragFloat("ScaleZ", &imGuiObjects[i].scale[2], 0.005, -50, 50);
                        ImGui::InputFloat3("Scale", &imGuiObjects[i].scale[0]);
                        ImGui::DragFloat("RotationX", &imGuiObjects[i].rotation[0], 0.01, -360, 360);
                        ImGui::DragFloat("RotationY", &imGuiObjects[i].rotation[1], 0.01, -360, 360);
                        ImGui::DragFloat("RotationZ", &imGuiObjects[i].rotation[2], 0.01, -360, 360);
                        ImGui::InputFloat3("Rotation", &imGuiObjects[i].rotation[0]);
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Save/Load Map"))
        {
            if (ImGui::Button("Save", ImVec2(100, 25)))
            {
                int numOfTerrain = getNumType("Terrain");
                int numOfWater = getNumType("Water");
                int numOfModel = getNumType("Model");
                int tCount = 0;
                int wCount = 0;
                int mCount = 0;

                std::ofstream luaMap;
                luaMap.open("Test.lua");
                luaMap << "numTerrains = " << numOfTerrain << "\n";
                luaMap << "numWater = " << numOfWater << "\n";
                luaMap << "numModels = " << numOfModel << "\n";

                for (int i = 0; i < imGuiObjects.size(); i++)
                {
                    if (imGuiObjects[i].objectType == "Terrain")
                    {
                        luaMap << "terrain" << tCount << " = objectData.new()\n";
                        tCount++;
                    }
                    else if (imGuiObjects[i].objectType == "Water")
                    {
                        luaMap << "water" << wCount << " = objectData.new()\n";
                        wCount++;
                    }
                    else
                    {
                        luaMap << "model" << mCount << " = objectData.new()\n";
                        mCount++;
                    }
                }

                tCount = wCount = mCount = 0;

                for (int i = 0; i < imGuiObjects.size(); i++)
                {
                    if (imGuiObjects[i].objectType == "Terrain")
                    {
                        luaMap << "\n";
                        luaMap << "terrain" << tCount << ".objectType = \"" << imGuiObjects[i].objectType << "\"\n";
                        luaMap << "terrain" << tCount << ".filepath = \"" << imGuiObjects[i].filepath << "\"\n";
                        luaMap << "terrain" << tCount << ".texturePath = \"" << imGuiObjects[i].texturePath << "\"\n";
                        luaMap << "terrain" << tCount << ".tx = " << imGuiObjects[i].translation.x << "\n";
                        luaMap << "terrain" << tCount << ".ty = " << imGuiObjects[i].translation.y << "\n";
                        luaMap << "terrain" << tCount << ".tz = " << imGuiObjects[i].translation.z << "\n";
                        luaMap << "terrain" << tCount << ".sx = " << imGuiObjects[i].scale.x << "\n";
                        luaMap << "terrain" << tCount << ".sy = " << imGuiObjects[i].scale.y << "\n";
                        luaMap << "terrain" << tCount << ".sz = " << imGuiObjects[i].scale.z << "\n";
                        luaMap << "terrain" << tCount << ".rx = " << imGuiObjects[i].rotation.x << "\n";
                        luaMap << "terrain" << tCount << ".ry = " << imGuiObjects[i].rotation.y << "\n";
                        luaMap << "terrain" << tCount << ".rz = " << imGuiObjects[i].rotation.z << "\n";
                        tCount++;
                    }
                    else if (imGuiObjects[i].objectType == "Water")
                    {
                        luaMap << "\n";
                        luaMap << "water" << wCount << ".objectType = \"" << imGuiObjects[i].objectType << "\"\n";
                        luaMap << "water" << wCount << ".iterations = " << imGuiObjects[i].iterations << "\n";
                        luaMap << "water" << wCount << ".width = " << imGuiObjects[i].width << "\n";
                        luaMap << "water" << wCount << ".length = " << imGuiObjects[i].length << "\n";
                        luaMap << "water" << wCount << ".minHeight = " << imGuiObjects[i].minHeight << "\n";
                        luaMap << "water" << wCount << ".maxHeight = " << imGuiObjects[i].maxHeight << "\n";
                        luaMap << "water" << wCount << ".filter = " << imGuiObjects[i].filter << "\n";
                        luaMap << "water" << wCount << ".texturePath = \"" << imGuiObjects[i].texturePath << "\"\n";
                        luaMap << "water" << wCount << ".tx = " << imGuiObjects[i].translation.x << "\n";
                        luaMap << "water" << wCount << ".ty = " << imGuiObjects[i].translation.y << "\n";
                        luaMap << "water" << wCount << ".tz = " << imGuiObjects[i].translation.z << "\n";
                        luaMap << "water" << wCount << ".sx = " << imGuiObjects[i].scale.x << "\n";
                        luaMap << "water" << wCount << ".sy = " << imGuiObjects[i].scale.y << "\n";
                        luaMap << "water" << wCount << ".sz = " << imGuiObjects[i].scale.z << "\n";
                        luaMap << "water" << wCount << ".rx = " << imGuiObjects[i].rotation.x << "\n";
                        luaMap << "water" << wCount << ".ry = " << imGuiObjects[i].rotation.y << "\n";
                        luaMap << "water" << wCount << ".rz = " << imGuiObjects[i].rotation.z << "\n";
                        wCount++;
                    }
                    else
                    {
                        luaMap << "\n";
                        luaMap << "model" << mCount << ".objectType = \"" << imGuiObjects[i].objectType << "\"\n";
                        luaMap << "model" << mCount << ".filepath = \"" << imGuiObjects[i].filepath << "\"\n";
                        luaMap << "model" << mCount << ".tx = " << imGuiObjects[i].translation.x << "\n";
                        luaMap << "model" << mCount << ".ty = " << imGuiObjects[i].translation.y << "\n";
                        luaMap << "model" << mCount << ".tz = " << imGuiObjects[i].translation.z << "\n";
                        luaMap << "model" << mCount << ".sx = " << imGuiObjects[i].scale.x << "\n";
                        luaMap << "model" << mCount << ".sy = " << imGuiObjects[i].scale.y << "\n";
                        luaMap << "model" << mCount << ".sz = " << imGuiObjects[i].scale.z << "\n";
                        luaMap << "model" << mCount << ".rx = " << imGuiObjects[i].rotation.x << "\n";
                        luaMap << "model" << mCount << ".ry = " << imGuiObjects[i].rotation.y << "\n";
                        luaMap << "model" << mCount << ".rz = " << imGuiObjects[i].rotation.z << "\n";
                        mCount++;
                    }
                }

                luaMap.close();
            }
            if (ImGui::Button("Load", ImVec2(100, 25)))
            {
                imGuiObjects.clear();
                loadData(imGuiObjects, "Test.lua");
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }

    /*
     * @brief Renders the objects stored
     *
     * This function renders all of the objects stored
     * in the ImGuiData according to the object type
     *
     * @param shader, waterShader, lightingShader, camera
     * @return void
     */
    void RenderObjects(Shader shader, Shader waterShader, Shader lightingShader, OurCamera camera)
    {
        if (imGuiObjects.size() != 0)
        {
            for (int i = 0; i < imGuiObjects.size(); i++)
            {
                if (imGuiObjects[i].objectType == "Terrain")
                {
                    shader.use();
                    glm::mat4 model = glm::mat4(1.0f);
                    shader.setMat4("model", model);
                    model = glm::translate(model, { imGuiObjects[i].translation[0], imGuiObjects[i].translation[1], imGuiObjects[i].translation[2] });
                    model = glm::scale(model, { imGuiObjects[i].scale[0], imGuiObjects[i].scale[1], imGuiObjects[i].scale[2] });
                    model = glm::rotate(model, imGuiObjects[i].rotation[0], glm::vec3(1, 0, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[1], glm::vec3(0, 1, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[2], glm::vec3(0, 0, 1));
                    shader.setMat4("model", model);
                    imGuiObjects[i].terrain.renderLandscape(camera.getRenderType());
                    temp = imGuiObjects[i].terrain.getTerrain();
                    heightScale = imGuiObjects[i].scale[1];
                }
                else if (imGuiObjects[i].objectType == "Water")
                {
                    waterShader.use();
                    glm::mat4 model = glm::mat4(1.0f);
                    waterShader.setMat4("model", model);
                    model = glm::translate(model, { imGuiObjects[i].translation[0], imGuiObjects[i].translation[1], imGuiObjects[i].translation[2] });
                    model = glm::scale(model, { imGuiObjects[i].scale[0], imGuiObjects[i].scale[1], imGuiObjects[i].scale[2] });
                    model = glm::rotate(model, imGuiObjects[i].rotation[0], glm::vec3(1, 0, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[1], glm::vec3(0, 1, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[2], glm::vec3(0, 0, 1));
                    waterShader.setMat4("model", model);
                    imGuiObjects[i].terrain.renderWater(camera.getRenderType());
                }
                else if (imGuiObjects[i].objectType == "Model")
                {
                    lightingShader.use();
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, { imGuiObjects[i].translation[0], imGuiObjects[i].translation[1], imGuiObjects[i].translation[2] });
                    model = glm::scale(model, { imGuiObjects[i].scale[0], imGuiObjects[i].scale[1], imGuiObjects[i].scale[2] });
                    model = glm::rotate(model, imGuiObjects[i].rotation[0], glm::vec3(1, 0, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[1], glm::vec3(0, 1, 0));
                    model = glm::rotate(model, imGuiObjects[i].rotation[2], glm::vec3(0, 0, 1));
                    lightingShader.setMat4("model", model);
                    imGuiObjects[i].model->Draw(lightingShader);
                }
            }
        }
    }

    /*
     * @brief Sets the ImGuiData objects
     *
     * This functions sets the ImGuiData to store objects
     * that have been loaded from elsewhere but still to be
     * used by ImGui
     *
     * @param dataVec
     * @return void
     */
    void setGuiData(std::vector<ImGuiData> dataVec)
    {
        this->imGuiObjects = dataVec;
    }

    /*
     * @brief Sets the terrain
     *
     * This functions sets a terrain landscape to 
     * be stored in the ImGuiData
     *
     * @param landscape
     * @return void
     */
    void setTerrain(Landscape landscape)
    {
        this->terrain = landscape;
    }

    /*
     * @brief Sets the water
     *
     * This functions sets a water landscape to 
     * be stored in the ImGuiData
     *
     * @param landscape
     * @return void
     */
    void setWater(Landscape landscape)
    {
        this->water = landscape;
    }

    /*
     * @brief Sets the model
     *
     * This functions sets a model to be stored
     * in the ImGuiData using a given filepath 
     * to a model
     *
     * @param modelPath
     * @return void
     */
    void setModel(const char* modelPath)
    {
        this->model = new Model(modelPath);
    }

    /*
     * @brief Sets the translation
     *
     * This functions sets the translation
     * of an object stored in ImGuiData
     *
     * @param translate
     * @return void
     */
    void setTranslation(glm::vec3 translate)
    {
        this->translation = translate;
    }

    /*
     * @brief Sets the scale
     *
     * This functions sets the scale
     * of an object stored in ImGuiData
     *
     * @param scale
     * @return void
     */
    void setScale(glm::vec3 scale)
    {
        this->scale = scale;
    }

    /*
     * @brief Sets the rotate
     *
     * This functions sets the rotation
     * of an object stored in ImGuiData
     *
     * @param rotate
     * @return void
     */
    void setRotation(glm::vec3 rotate)
    {
        this->rotation = rotation;
    }

    /*
     * @brief Sets the object type
     *
     * This functions sets the object type
     * of an object stored in ImGuiData
     *
     * @param type
     * @return void
     */
    void setObjectType(std::string type)
    {
        objectType = type;
    }

    /*
     * @brief Sets the file path
     *
     * This functions sets the file path
     * of an object stored in ImGuiData
     *
     * @param filePath
     * @return void
     */
    void setFilePath(const char* filePath)
    {
        filepath = filePath;
    }

    /*
     * @brief Sets the texture path
     *
     * This functions sets the texture path
     * of an object stored in ImGuiData
     *
     * @param texturepath
     * @return void
     */
    void setTexturePath(const char* texturepath)
    {
        texturePath = texturepath;
    }

    /*
     * @brief Sets the number of iterations
     *
     * This functions sets the number of iterations
     * of a landscape stored in ImGuiData
     *
     * @param numIterations
     * @return void
     */
    void setNumIterations(float numIterations)
    {
        iterations = numIterations;
    }

    /*
     * @brief Sets the terrain width
     *
     * This functions sets the width
     * of a landscape stored in ImGuiData
     *
     * @param Width
     * @return void
     */
    void setWidth(float Width)
    {
        width = Width;
    }

    /*
     * @brief Sets the terrain length
     *
     * This functions sets the length
     * of a landscape stored in ImGuiData
     *
     * @param Length
     * @return void
     */
    void setLength(float Length)
    {
        length = Length;
    }

    /*
     * @brief Sets the terrain minHeight
     *
     * This functions sets the minimum height
     * of a landscape stored in ImGuiData
     *
     * @param MinHeight
     * @return void
     */
    void setMinHeight(float MinHeight)
    {
        minHeight = MinHeight;
    }

    /*
     * @brief Sets the terrain maxHeight
     *
     * This functions sets the maximum height
     * of a landscape stored in ImGuiData
     *
     * @param MaxHeight
     * @return void
     */
    void setMaxHeight(float MaxHeight)
    {
        maxHeight = MaxHeight;
    }

    /*
     * @brief Sets the terrain filter
     *
     * This functions sets the filter
     * of a landscape stored in ImGuiData
     *
     * @param Filter
     * @return void
     */
    void setFilter(float Filter)
    {
        filter = Filter;
    }

    /*
     * @brief Gets the heightScale
     *
     * This function returns the height scale of an
     * object stored in ImGuiData
     *
     * @return heightScale
     */
    int getHeightScale()
    {
        return heightScale;
    }
};