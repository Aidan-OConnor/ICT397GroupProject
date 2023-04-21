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
#include "ImGuiData.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadTexture(const char* path);
void initShaders(OurCamera& camera, Shader& shader, Shader& lightingShader, Shader& modelShader, Shader& waterShader, Shader& skyboxShader);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

OurCamera camera;
float camHeight = 2.5;

glm::vec3 lightPos(200.0f, 50.0f, 200.0f);

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

int main()
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
    Shader lightingShader("lightingVertex.shader", "lightingFragment.shader");

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

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.new_usertype<objectData>("objectData", sol::constructors<void()>(),
        "filepath", &objectData::filepath, "texturePath", &objectData::texturePath, "objectType", &objectData::objectType,
        "iterations", &objectData::iterations, "width", &objectData::width,
        "length", &objectData::length, "minHeight", &objectData::minHeight,
        "maxHeight", &objectData::maxHeight, "filter", &objectData::filter,
        "tx", &objectData::tx, "ty", &objectData::ty, "tz", &objectData::tz,
        "sx", &objectData::sx, "sy", &objectData::sy, "sz", &objectData::sz,
        "rx", &objectData::rx, "ry", &objectData::ry, "tz", &objectData::rz
    );

    std::vector<objectData> luaMap;
    std::string tempName, tempLocation;
    ImGuiData imGuiData, tempGuiData;
    std::vector<ImGuiData> convertedData;

    lua.script_file("model0.lua");
    objectData& tempObject = lua["heightmap"];
    luaMap.push_back(tempObject);

    Landscape tempTerrain;
    tempTerrain.loadFromHeightmap(luaMap[0].filepath, 1, luaMap[0].texturePath, GL_TEXTURE_2D);
    tempTerrain.addTextures("Images/Ground2.jpg", GL_TEXTURE_2D, "Images/Grass.jpg", GL_TEXTURE_2D);
    tempGuiData.setTerrain(tempTerrain);

    glm::vec3 tempVec;

    tempGuiData.setObjectType(luaMap[0].objectType);

    tempVec.x = luaMap[0].tx;
    tempVec.y = luaMap[0].ty;
    tempVec.z = luaMap[0].tz;
    tempGuiData.setTranslation(tempVec);

    tempVec.x = luaMap[0].sx;
    tempVec.y = luaMap[0].sy;
    tempVec.z = luaMap[0].sz;
    tempGuiData.setScale(tempVec);

    tempVec.x = luaMap[0].rx;
    tempVec.y = luaMap[0].ry;
    tempVec.z = luaMap[0].rz;
    tempGuiData.setRotation(tempVec);

    convertedData.push_back(tempGuiData);

    lua.script_file("model1.lua");
    objectData& tempObject1 = lua["waterFormation"];
    luaMap.push_back(tempObject1);

    tempTerrain.loadFromFaultFormation(luaMap[1].iterations, luaMap[1].width, luaMap[1].length,
        1, 1, luaMap[1].minHeight, luaMap[1].maxHeight, luaMap[1].filter,
        luaMap[1].texturePath, GL_TEXTURE_2D);
    tempGuiData.setTerrain(tempTerrain);

    tempGuiData.setObjectType(luaMap[1].objectType);

    tempVec.x = luaMap[1].tx;
    tempVec.y = luaMap[1].ty;
    tempVec.z = luaMap[1].tz;
    tempGuiData.setTranslation(tempVec);

    tempVec.x = luaMap[1].sx;
    tempVec.y = luaMap[1].sy;
    tempVec.z = luaMap[1].sz;
    tempGuiData.setScale(tempVec);

    tempVec.x = luaMap[1].rx;
    tempVec.y = luaMap[1].ry;
    tempVec.z = luaMap[1].rz;
    tempGuiData.setRotation(tempVec);

    convertedData.push_back(tempGuiData);

    for (int i = 2; i < 32; i++)
    {
        tempName = "model" + std::to_string(i-1);
        tempLocation = "model" + std::to_string(i) + ".lua";
        const char* modelName = tempName.c_str();
        const char* modelLocation = tempLocation.c_str();
        lua.script_file(modelLocation);
        objectData& tempObject2 = lua[modelName];
        luaMap.push_back(tempObject2);

        tempGuiData.setModel(luaMap[i].filepath);

        tempGuiData.setObjectType(luaMap[i].objectType);

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

    imGuiData.setGuiData(convertedData);

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
    shader.setInt("tex2", 1);
    shader.setInt("tex3", 2);
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

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            useImGui = !useImGui;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        initShaders(camera, shader, lightingShader, modelShader, waterShader, skyboxShader);

        if (useImGui)
        {
            imGuiData.RenderUI();
        }

        imGuiData.RenderObjects(shader, waterShader, lightingShader, camera);

        skyboxShader.use();
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

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void initShaders(OurCamera& camera, Shader& shader, Shader& lightingShader, Shader& modelShader, Shader& waterShader, Shader& skyboxShader)
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

    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    view = glm::mat4(glm::mat3(glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp())));
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
}