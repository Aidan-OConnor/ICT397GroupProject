#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <Vector>
#include <fstream>

#include "Shader.h"
#include "Texture.h"
#include "OurCamera.h"
#include "Terrain.h"
#include "Landscape.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
unsigned int loadCubemap(std::vector<std::string> faces);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

OurCamera camera;
float camHeight = 2.5;

int main()
{
    sol::state lua{};
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dofile(L, "Test.lua");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ICT397 Game Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
    
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");


    Shader shader("vertex.shader", "fragment.shader");
    Shader skyboxShader("skyboxVertex.shader", "skyboxFragment.shader");
    Shader waterShader("waterVertex.shader", "waterFragment.shader");
    Shader modelShader("modelVertex.shader", "modelFragment.shader");

    Model bruiserStance("Models/Bruiser/bruiserStance.obj");

    glm::vec3 camPos = { 0.0f, 250.0f, 0.0f };
    camera.updatePosition(camPos);

    /*
    Landscape landscape, water;
    landscape.loadFromHeightmap("Terrains/VolcanoType6.png", 4, "Images/Ground2.jpg", GL_TEXTURE_2D);
    water.loadFromFaultFormation(1000, 128, 128, (float)landscape.getTerrain().getWidth() / 128, (float)landscape.getTerrain().getHeight() / 128, -5, 5, 0.5, "Images/Water1.jpg", GL_TEXTURE_2D);

    Model ourModel("Models/NewBoat/boat.obj");
    */

    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
    {
        "Images/Skybox/right.png",
        "Images/Skybox/left.png",
        "Images/Skybox/top.png",
        "Images/Skybox/bottom.png",
        "Images/Skybox/front.png",
        "Images/Skybox/back.png",
    };

    unsigned int cubemapTexture = loadCubemap(faces);

    shader.use();
    shader.setInt("tex1", 0);
    shader.setInt("tileSize", 50);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    waterShader.use();
    waterShader.setInt("tex1", 0);
    waterShader.setInt("tileSize", 50);

    bool useImGui = false;

    Landscape tempLandscape;
    float iterations, width, length, minHeight, maxHeight, filter;
    iterations = width = length = minHeight = maxHeight = filter = 0;

    struct imGuiType
    {
        std::string objectType;
        Landscape terrain, water;
        Model* model;
        const char* objectName;
        glm::vec3 translation;
        glm::vec3 scale;
    };

    std::vector<imGuiType> imGuiObjects;

    /*
    ImTemp.objectName = "Volcano";
    ImTemp.translation = {0.0f, 100.0f, 0.0f};
    ImTemp.scale = {0.5f, 6.0f, 0.5f};

    imGuiObjects.push_back(ImTemp);

    ImTemp.objectName = "Water";
    ImTemp.translation = {0.0f, 0.0f, 0.0f};
    ImTemp.scale = {1.0f, 1.0f, 1.0f};

    imGuiObjects.push_back(ImTemp);
    */

    while (!glfwWindowShouldClose(window))
    {
        camera.updateDeltaTime();
        camera.processInput(window);

        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            useImGui = !useImGui;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("model", model);

        waterShader.use();
        waterShader.setMat4("projection", projection);
        waterShader.setMat4("view", view);
        waterShader.setMat4("model", model);

        /*
        model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
        model = glm::translate(model, glm::vec3(0.0f, 20.0f, -1.0f));
        modelShader.setMat4("model", model);
        bruiserStance.Draw(modelShader);
        */

        if (useImGui)
        {
            ImGui::Begin("Control Centre");

            if (ImGui::TreeNode("Create Terrain"))
            {
                if (ImGui::TreeNode("Heightmap"))
                {
                    if (ImGui::Button("Generate", ImVec2(100, 25)))
                    {
                        imGuiType ImTemp;

                        ImTemp.objectType = "Terrain";
                        ImTemp.terrain.loadFromHeightmap("Terrains/VolcanoType6.png", 4, "Images/Ground2.jpg", GL_TEXTURE_2D);
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
                        imGuiType ImTemp;
                        ImTemp.objectType = "Terrain";
                        ImTemp.terrain.loadFromFaultFormation(iterations, width, length, 1, 1, minHeight, maxHeight, filter, "Images/Ground2.jpg", GL_TEXTURE_2D);
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
                        imGuiType ImTemp;
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
                if (ImGui::TreeNode("Bruiser"))
                {
                    if (ImGui::Button("Load", ImVec2(100, 25)))
                    {
                        imGuiType ImTemp;
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
                        imGuiType ImTemp;
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
                        imGuiType ImTemp;
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
                        imGuiType ImTemp;
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
                        imGuiType ImTemp;
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
                        imGuiType ImTemp;
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
                            ImGui::TreePop();
                        }
                    }
                }
                ImGui::TreePop();
            }

            ImGui::End();
        }

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
                    waterShader.setMat4("model", model);
                    imGuiObjects[i].terrain.renderLandscape(camera.getRenderType());
                }
                else if (imGuiObjects[i].objectType == "Model")
                {
                    modelShader.use();
                    glm::mat4 model = glm::mat4(1.0f);
                    modelShader.setMat4("model", model);
                    model = glm::translate(model, { imGuiObjects[i].translation[0], imGuiObjects[i].translation[1], imGuiObjects[i].translation[2] });
                    model = glm::scale(model, { imGuiObjects[i].scale[0], imGuiObjects[i].scale[1], imGuiObjects[i].scale[2] });
                    modelShader.setMat4("model", model);
                    imGuiObjects[i].model->Draw(modelShader);
                }
            }
        }

        /*
        if (useImGui)
        {
            ImGui::Begin("ObjectData");
            for (int i = 0; i < imGuiObjects.size(); i++)
            {
                if (ImGui::TreeNode(imGuiObjects[i].objectName))
                {
                    ImGui::DragFloat("PositionX", &imGuiObjects[i].translation[0], 0.5, -1000, 1000);
                    ImGui::DragFloat("PositionY", &imGuiObjects[i].translation[1], 0.5, -1000, 1000);
                    ImGui::DragFloat("PositionZ", &imGuiObjects[i].translation[2], 0.5, -1000, 1000);
                    ImGui::InputFloat3("Position", &imGuiObjects[i].translation[0]);
                    ImGui::DragFloat("ScaleX", &imGuiObjects[i].scale[0], 0.01, -10, 10);
                    ImGui::DragFloat("ScaleY", &imGuiObjects[i].scale[1], 0.01, -10, 10);
                    ImGui::DragFloat("ScaleZ", &imGuiObjects[i].scale[2], 0.01, -10, 10);
                    ImGui::TreePop();
                }
            }
            ImGui::End();
        }

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, {imGuiObjects[0].translation[0], imGuiObjects[0].translation[1], imGuiObjects[0].translation[2]});
        model = glm::scale(model, {imGuiObjects[0].scale[0], imGuiObjects[0].scale[1], imGuiObjects[0].scale[2]});
        shader.setMat4("model", model);
               
        landscape.renderLandscape(camera.getRenderType());

        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, { 70.0f, 16.5f, 35.0f });
        shader.setMat4("model", model2);

        ourModel.Draw(shader);

        waterShader.use();
        waterShader.setMat4("projection", projection);
        waterShader.setMat4("view", view);

        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, {imGuiObjects[1].translation[0], imGuiObjects[1].translation[1], imGuiObjects[1].translation[2]});
        model3 = glm::scale(model3, {imGuiObjects[1].scale[0], imGuiObjects[1].scale[1] , imGuiObjects[1].scale[2]});
        waterShader.setMat4("model", model3);
        
        water.renderLandscape(camera.getRenderType());
        */
        
        /*
        std::vector<glm::vec3> vertices = landscape.getTerrain().getVertices();

        for (int i = 0; i < vertices.size(); i++)
        {
            if (camera.getFoot().x <= vertices[i].x + 1 && camera.getFoot().x >= vertices[i].x - 1 &&
                camera.getFoot().z <= vertices[i].z + 1 && camera.getFoot().z >= vertices[i].z - 1)
            {
                camera.setHeight((int)vertices[i].y + 5);

                i = vertices.size();
            }
        }
        */

        glDepthFunc(GL_LEQUAL);  
        skyboxShader.use();
        view = glm::mat4(glm::mat3(glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp())));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 2;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    camera.updateMouse(xposIn, yposIn);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}