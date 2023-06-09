#pragma once

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

#include "AI.h"
#include "font_awesome.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Terrain.h"
#include "Landscape.h"
#include "model.h"
#include "Skybox.h"
#include "ImGuiData.h"
#include "Physics.h"
#include "reactphysics3d/reactphysics3d.h"

// ReactPhysics3D namespace
using namespace reactphysics3d;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initShaders(Camera& camera, Shader& shader, Shader& lightingShader, Shader& waterShader, Shader& modelShader, ImGuiData player,
    glm::mat4& projection, glm::mat4& view, glm::mat4& model);
void enterBoat(glm::vec3 playerPos, glm::vec3 boatPos);
void leaveBoat(std::vector<ImGuiData> Docks, glm::vec3 playerPos);
void dockBoat(std::vector<ImGuiData> Docks);
void CenterButtons(std::vector<std::string> names, GLFWwindow* window);

int scrWidth;
int scrHeight;
Camera camera;
float camHeight = 2.5;
bool useImGui = false;
bool playGame = false;
bool isDev = false;
bool canEnterBoat = false;
bool canLeaveBoat = false;
bool boatDocked = false;
glm::vec3 lightPos(8000.0f, 2000.0f, 8000.0f);
int dockIndex;

int run()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    //Windowed
    GLFWwindow* window = glfwCreateWindow(vidMode->width, vidMode->height, "ICT397 Game Engine", NULL, NULL);

    scrWidth = vidMode->width;
    scrHeight = vidMode->height;

    //Fullscreen
    //GLFWwindow* window = glfwCreateWindow(vidMode->width, vidMode->height, "ICT397 Game Engine", glfwGetPrimaryMonitor(), nullptr);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

    Skybox skybox;
    skybox.Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    Shader shader("vertex.shader", "fragment.shader");
    Shader waterShader("waterVertex.shader", "waterFragment.shader");
    Shader modelShader("md2Vert.shader", "md2Frag.shader");
    Shader lightingShader("lightingVertex.shader", "lightingFragment.shader");

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    shader.use();
    shader.setInt("tex1", 0);
    shader.setInt("tex2", 1);
    shader.setInt("tex3", 2);
    shader.setInt("tileSize", 20);

    waterShader.use();
    waterShader.setInt("tex1", 0);
    waterShader.setInt("tileSize", 2.5);

    modelShader.use();
    modelShader.setInt("texture0", 0);

    std::vector<ImGuiData> convertedData;
    ImGuiData imGuiData;

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("MapFiles.lua");

    std::vector<std::string> maps;
    int numMaps = lua["numMaps"];

    for (int i = 0; i < numMaps; i++)
    {
        std::string tempName = "Map" + std::to_string(i+1);
        const char* mapName = tempName.c_str();
        std::string tempMap = lua["maps"][mapName];
        maps.push_back(tempMap);
    }

    imGuiData.loadData(convertedData, maps, 0);
    imGuiData.setGuiData(convertedData);

    std::vector<Terrain> terrains;
    terrains = imGuiData.getTerrains();

    //---------------------------------------------------------------
    Texture testTexture;
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = testTexture.LoadTextureForGUI("Images/MapImages/Map1Blur.jpg", &my_image_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);

    Texture testTexture1;
    int my_image_width1 = 0;
    int my_image_height1 = 0;
    GLuint my_image_texture1 = 0;
    bool ret1 = testTexture1.LoadTextureForGUI("Images/MapImages/Map1.jpg", &my_image_texture1, &my_image_width1, &my_image_height1);
    IM_ASSERT(ret1);

    ImFont* Default = io.Fonts->AddFontFromFileTTF("Fonts/proggy-clean.ttf", 13.0f);
    ImFont* RubikStorm = io.Fonts->AddFontFromFileTTF("Fonts/RubikStorm-Regular.ttf", vidMode->height/9);
    ImFont* RubikDirt = io.Fonts->AddFontFromFileTTF("Fonts/RubikDirt-Regular.ttf", vidMode->height/15);

    static const ImWchar icon_ranges[]{ 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;

    ImFont* icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 60.0f, &icons_config, icon_ranges);

    //--------------------------------------------------------------------

    AI ai;

    //Physics physics;
    //physics.createCameraBody(camera);
    //physics.createTestCapsule();
    //physics.createTerrain(imGuiData);

    while (!glfwWindowShouldClose(window))
    {
        ImGuiData player = imGuiData.getPlayer();
        glm::vec3 playerPosition = player.getTranslation();
        glm::vec3 playerRotation = player.getRotation();

        camera.updateDeltaTime();
        camera.processInput(window, playerPosition, playerRotation);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!playGame)
        {
            camera.setMouseControls(false);
            ImGui::SetNextWindowPos({ -8.0f, -8.0f });
            ImGui::SetNextWindowSize({ (float)(vidMode->width * 1.007), (float)(vidMode->height * 1.009) });
            ImGui::Begin("OpenGL Texture Text", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs);
            ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(vidMode->width + 100, vidMode->height + 10), { 0, 1 }, { 1, 0 });
            ImGui::End();

            ImGui::SetNextWindowPos({ (float)(vidMode->width / 2 - vidMode->width / 5.0f), (float)(vidMode->height / 2 - vidMode->height/2.5f) });
            ImGui::SetNextWindowSize({ (float)(vidMode->width * 1.007), (float)(vidMode->height * 1.009) });
            ImGui::Begin("Title", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs);
            ImGui::PushFont(RubikStorm);
            ImGui::Text("LAN's Island");
            ImGui::PopFont();
            ImGui::End();

            ImGui::SetNextWindowPos({ (float)(vidMode->width / 2 - vidMode->width / 7.2f), (float)(vidMode->height / 2 - vidMode->height / 6.0f) });
            ImGui::SetNextWindowSize({ (float)(vidMode->width * 1.007), (float)(vidMode->height * 1.009) });
            ImGui::Begin("Buttons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            //ImGui::PushFont(RubikDirt);
            ImGui::PushFont(icons_font);
            CenterButtons({ ICON_FA_PLAY"  Play Game", ICON_FA_DESKTOP"  Dev Mode", ICON_FA_POWER_OFF"  Exit Game" }, window);
            ImGui::PopFont();
            //ImGui::ImageButton((void*)(intptr_t)my_image_texture1, ImVec2(100, 100), { 0, 1 }, { 1, 0 });
            ImGui::End();
        }

        if (playGame)
        {
            if (isDev)
            {
                if (useImGui)
                {
                    ImGui::PushFont(Default);
                    imGuiData.RenderUI(camera);
                    ImGui::PopFont();
                }
            }
            else
            {
                std::vector<ImGuiData> NPCs = imGuiData.getNPCs();
                std::vector<ImGuiData> Docks = imGuiData.getDocks();

                if (camera.getPerspective())
                {
                    ai.chasePlayer(NPCs, camera.getCameraPos());
                    enterBoat(camera.getCameraPos(), playerPosition);
                    //physics.setGravity(true);
                }
                else
                {
                    ai.chasePlayer(NPCs, playerPosition);
                    leaveBoat(Docks, playerPosition);
                    //physics.setGravity(false);
                }

                //physics.updateBodies(camera);

                /*if (camera.getCameraPos().y < -50)
                {
                    camera.updatePosition(glm::vec3(-300, 100, 0));
                }*/
                
                std::vector<glm::vec3> temp;
                int newY;
                
                // Terrain walking for main island
                if (camera.getCameraPos().x > -2560 && camera.getCameraPos().x < 2560 && camera.getCameraPos().z > -2560 && camera.getCameraPos().z < 2560)
                {
                    temp = terrains[0].getVertices();
                    newY = terrains[0].getHeightAtPos(temp, camera.getCameraPos().x, camera.getCameraPos().z);
                    camera.setCameraY(newY + 50);
                }

                player.setTranslation(playerPosition);
                player.setRotation(playerRotation);

                dockBoat(Docks);

                imGuiData.setPlayer(player);
                imGuiData.setNPCs(NPCs);
            }

            initShaders(camera, shader, lightingShader, waterShader, modelShader, player, projection, view, model);

            imGuiData.RenderObjects(shader, waterShader, lightingShader, modelShader, camera);

            projection = glm::perspective(glm::radians(90.0f), (float)scrWidth / (float)scrHeight, 0.1f, 1000.0f);
            view = glm::mat4(glm::mat3(glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp())));

            if (!camera.getPerspective())
            {
                view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(camera.getCameraPos()), glm::vec3(player.getTranslation()), camera.getCameraUp())));
            }

            skybox.Draw(view, projection);
        }

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

void CenterButtons(std::vector<std::string> names, GLFWwindow* window)
{
    const auto& style = ImGui::GetStyle();

    for (int i = 0; i < names.size(); i++)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(180, 180, 180, 100).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(180, 255, 180, 100).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(180, 255, 180, 100).Value);
        if (ImGui::Button(names[i].c_str(), { (float)(scrWidth/3.6), (float)(scrHeight/9) }))
        {
            if (i == 0)
            {
                playGame = true;
                camera.setMouseControls(true);
            }
            else if (i == 1)
            {
                playGame = true;
                isDev = true;
                camera.setMouseControls(true);
            }
            else if (i == 2)
            {
                glfwSetWindowShouldClose(window, true);
            }
        }
        ImGui::PopStyleColor(3);
    }
}

void enterBoat(glm::vec3 playerPos, glm::vec3 boatPos)
{
    canLeaveBoat = false;

    float distance = glm::sqrt((playerPos.x - boatPos.x) * (playerPos.x - boatPos.x) +
        (playerPos.z - boatPos.z) * (playerPos.z - boatPos.z));

    if (distance < 100)
        canEnterBoat = true;
    else
        canEnterBoat = false;
}

void leaveBoat(std::vector<ImGuiData> Docks, glm::vec3 playerPos)
{
    canEnterBoat = false;

    for (int i = 0; i < Docks.size(); i++)
    {
        float distance = glm::sqrt((Docks[i].getTranslation().x - playerPos.x) * (Docks[i].getTranslation().x - playerPos.x) +
            (Docks[i].getTranslation().z - playerPos.z) * (Docks[i].getTranslation().z - playerPos.z));

        if (distance < 100)
        {
            canLeaveBoat = true;
            dockIndex = i;
            i = Docks.size();
        }
        else
            canLeaveBoat = false;
    }
}

void dockBoat(std::vector<ImGuiData> Docks)
{
    if (boatDocked)
    {
        glm::vec3 camPosition = Docks[dockIndex].getTranslation();
        camPosition.y += 20;
        camera.updatePosition(camPosition);
        boatDocked = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    camera.updateMouse(xposIn, yposIn);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (playGame)
    {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            playGame = false;
            isDev = false;
        }

        if (isDev)
        {
            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            {
                useImGui = !useImGui;
                camera.swapMouseControls();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            camera.swapRenderType();
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && (canEnterBoat || canLeaveBoat))
        {
            camera.setPerspective(!camera.getPerspective());

            if (canLeaveBoat)
            {
                boatDocked = true;
            }
        }
    }
}

void initShaders(Camera& camera, Shader& shader, Shader& lightingShader, Shader& waterShader, Shader& modelShader, ImGuiData player, 
    glm::mat4& projection, glm::mat4& view, glm::mat4& model)
{
    shader.use();

    projection = glm::perspective(glm::radians(90.0f), (float)scrWidth / (float)scrHeight, 0.1f, 25000.0f);
    shader.setMat4("projection", projection);

    view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());

    if (!camera.getPerspective())
    {
        view = glm::lookAt(glm::vec3(camera.getCameraPos()), glm::vec3(player.getTranslation()), camera.getCameraUp());
    }

    shader.setMat4("view", view);

    model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.getCameraPos());

    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("light.constant", 0.5f);
    lightingShader.setFloat("light.linear", 0.00000014f);
    lightingShader.setFloat("light.quadratic", 0.0000000007f);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);

    waterShader.use();
    waterShader.setMat4("projection", projection);
    waterShader.setMat4("view", view);
    waterShader.setMat4("model", model);

    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
    modelShader.setMat4("model", model);
    modelShader.setMat4("normal", model);
}