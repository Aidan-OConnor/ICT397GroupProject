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

    glm::vec3 camPos = { 0.0f, 250.0f, 0.0f };
    camera.updatePosition(camPos);

    Landscape landscape, water, landscape2;
    landscape.loadFromHeightmap("Terrains/VolcanoType6.png", 4, "Images/Ground2.jpg", GL_TEXTURE_2D);
    water.loadFromFaultFormation(1000, 128, 128, (float)landscape.getTerrain().getWidth()/128, (float)landscape.getTerrain().getHeight()/128, -5, 5, 0.5, "Images/Water1.jpg", GL_TEXTURE_2D);

    Model ourModel("Models/Boat/boat.obj");

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
        "Images/Skybox/left.png",
        "Images/Skybox/right.png",
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

        static float vTranslation[] = { 0.0f, 100.0f, 0.0f };
        static float vScale[] = { 0.5f, 6.0f, 0.5f };
        static float wTranslation[] = { 0.0f, 0.0f, 0.0f };
        static float wScale[] = { 1.0f, 1.0f, 1.0f };

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, {vTranslation[0], vTranslation[1], vTranslation[2]});
        model = glm::scale(model, {vScale[0], vScale[1], vScale[2]});
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
        model3 = glm::translate(model3, {wTranslation[0], wTranslation[1], wTranslation[2] });
        model3 = glm::scale(model3, { wScale[0], wScale[1] , wScale[2] });
        waterShader.setMat4("model", model3);
        
        water.renderLandscape(camera.getRenderType());
        
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

        if (useImGui)
        {
            ImGui::Begin("Volcano");
            ImGui::SliderFloat("PositionX", &vTranslation[0], -1000, 1000);
            ImGui::SliderFloat("PositionY", &vTranslation[1], -1000, 1000);
            ImGui::SliderFloat("PositionZ", &vTranslation[2], -1000, 1000);
            ImGui::SliderFloat("ScaleX", &vScale[0], -10, 10);
            ImGui::SliderFloat("ScaleY", &vScale[1], -10, 10);
            ImGui::SliderFloat("ScaleZ", &vScale[2], -10, 10);
            ImGui::End();

            ImGui::Begin("Water");
            ImGui::SliderFloat("PositionX", &wTranslation[0], -1000, 1000);
            ImGui::SliderFloat("PositionY", &wTranslation[1], -1000, 1000);
            ImGui::SliderFloat("PositionZ", &wTranslation[2], -1000, 1000);
            ImGui::SliderFloat("ScaleX", &wScale[0], -10, 10);
            ImGui::SliderFloat("ScaleY", &wScale[1], -10, 10);
            ImGui::SliderFloat("ScaleZ", &wScale[2], -10, 10);
            ImGui::End();
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