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

#include "Shader.h"
#include "Texture.h"
#include "OurCamera.h"
#include "Terrain.h"
#include "Landscape.h"
#include "model.h"
#include "Skybox.h"
#include "ImGuiData.h"
#include "reactphysics3d/reactphysics3d.h"

// ReactPhysics3D namespace
using namespace reactphysics3d;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initShaders(OurCamera& camera, Shader& shader, Shader& lightingShader, Shader& modelShader, Shader& waterShader);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
OurCamera camera;
float camHeight = 2.5;
bool useImGui = false;
glm::vec3 lightPos(200.0f, 50.0f, 200.0f);

int run()
{
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
    Shader modelShader("modelVertex.shader", "modelFragment.shader");
    Shader lightingShader("lightingVertex.shader", "lightingFragment.shader");

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    shader.use();
    shader.setInt("tex1", 0);
    shader.setInt("tex2", 1);
    shader.setInt("tex3", 2);
    shader.setInt("tileSize", 50);

    waterShader.use();
    waterShader.setInt("tex1", 0);
    waterShader.setInt("tileSize", 5);

    std::vector<ImGuiData> convertedData;
    ImGuiData imGuiData;

    imGuiData.loadData(convertedData, "Test.lua");
    imGuiData.setGuiData(convertedData);

    // Reactphysics initialisation
    PhysicsCommon physicsCommon;
    PhysicsWorld* world = physicsCommon.createPhysicsWorld();

    while (!glfwWindowShouldClose(window))
    {
        camera.updateDeltaTime();
        camera.processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        initShaders(camera, shader, lightingShader, modelShader, waterShader);

        if (useImGui)
        {
            imGuiData.RenderUI(camera);
        }

        imGuiData.RenderObjects(shader, waterShader, lightingShader, camera);

        //std::vector<glm::vec3> test = imGuiData.temp.getVertices();
        //glm::vec3 camPos = camera.getCameraPos();
        //int currentX = camPos.x;
        //int currentZ = camPos.z;
        //int currentY = imGuiData.temp.getHeightAtPos(test, currentX, currentZ, imGuiData.getHeightScale());
        //if (currentY != 0) {
        //    camera.setCameraY(currentY);
        //    camera.setLevel(currentY);
        //}
        //else {
        //    camera.setLevel(currentY + 4);
        //}

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp())));
        skybox.Draw(view, projection);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        useImGui = !useImGui;
        camera.swapMouseControls();
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        camera.swapRenderType();
    }

}

void initShaders(OurCamera& camera, Shader& shader, Shader& lightingShader, Shader& modelShader, Shader& waterShader)
{
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
    shader.setMat4("projection", projection);

    glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
    shader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.getCameraPos());

    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("light.constant", 0.5f);
    lightingShader.setFloat("light.linear", 0.000014f);
    lightingShader.setFloat("light.quadratic", 0.00000007f);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);

    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);
    modelShader.setMat4("model", model);

    waterShader.use();
    waterShader.setMat4("projection", projection);
    waterShader.setMat4("view", view);
    waterShader.setMat4("model", model);
}