#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

OurCamera camera;
float camHeight = 2.5;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Trials of Crusados", NULL, NULL);
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

    Shader shader("vertex.shader", "fragment.shader");

    glm::vec3 camPos = { 0.0f, 0.0f, 0.0f };
    camera.updatePosition(camPos);

    Landscape landscape, water;
    landscape.loadFromHeightmap("Terrains/test3.jpeg", 1, "Images/Ground2.jpg", GL_TEXTURE_2D);
    water.loadFromFaultFormation(1000, -3, 3, 0.5, "Images/Water1.jpg", GL_TEXTURE_2D);

    shader.use();
    shader.setInt("tex1", 0);
    shader.setInt("tileSize", 50);

    while (!glfwWindowShouldClose(window))
    {
        camera.updateDeltaTime();

        camera.processInput(window);

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, { 0.0f, 0.0f, 0.0f });
        shader.setMat4("model", model);

        landscape.renderLandscape(camera.getRenderType());
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


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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