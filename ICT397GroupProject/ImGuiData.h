#pragma once
#include "Landscape.h"
#include "model.h"

class ImGuiData
{
private:
    float iterations, width, length, minHeight, maxHeight, filter;
    std::string objectType;
    Landscape terrain, water, tempLandscape;
    Model* model;
    const char* objectName;
    glm::vec3 translation, scale, rotation;
    std::vector<ImGuiData> imGuiObjects;
public:
    ImGuiData()
    {
        this->iterations = this->width = this->length = 
            this->minHeight = this->maxHeight = this->filter = 0;
    }

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
                    ImTemp.terrain.loadFromHeightmap("Terrains/VolcanoType11.png", 1, "Images/Ground3.jpg", GL_TEXTURE_2D);
                    ImTemp.terrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };

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
                    ImTemp.terrain.loadFromFaultFormation(iterations, width, length, 1, 1, minHeight, maxHeight, filter, "Images/Ground3.jpg", GL_TEXTURE_2D);
                    ImTemp.terrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };

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
                    ImTemp.terrain.loadFromFaultFormation(iterations, width, length, 1, 1, minHeight, maxHeight, filter, "Images/Water1.jpg", GL_TEXTURE_2D);
                    ImTemp.translation = { 0.0f, 0.0f, 0.0f };
                    ImTemp.scale = { 1.0f, 1.0f, 1.0f };

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

        ImGui::End();
    }

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

    void setGuiData(std::vector<ImGuiData> dataVec)
    {
        this->imGuiObjects = dataVec;
    }

    void setTerrain(Landscape landscape)
    {
        this->terrain = landscape;
    }

    void setWater(Landscape landscape)
    {
        this->water = landscape;
    }

    void setModel(const char* modelPath)
    {
        this->model = new Model(modelPath);
    }

    void setTranslation(glm::vec3 translate)
    {
        this->translation = translate;
    }

    void setScale(glm::vec3 scale)
    {
        this->scale = scale;
    }

    void setRotation(glm::vec3 rotate)
    {
        this->rotation = rotation;
    }

    void setObjectType(std::string type)
    {
        objectType = type;
    }
};