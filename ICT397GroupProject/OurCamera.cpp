#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "OurCamera.h"

OurCamera::OurCamera()
{
    this->cameraPos = glm::vec3(0.0f, 2.5f, 0.0f);
    this->foot = this->cameraPos;
    this->foot.y -= 2.5;
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->spawnPoint = glm::vec3(0.0f, 3.0f, 0.0f);

    this->firstMouse = true;
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->lastX = 1600.0f / 2.0;
    this->lastY = 1200.0 / 2.0;

    this->deltaTime = 0.0f;
    this->prevFrame = 0.0f;

    this->dt = 0.0f;
    this->grounded = false;
    this->renderTriangle = true;
    this->mouseControls = true;
}

void OurCamera::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(250.0 * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && stamina > 0 && grounded == true) {
        cameraSpeed = cameraSpeed * 1.5;
        stamina -= 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
        cameraSpeed = static_cast<float>(15.0 * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && grounded == true)
        cameraPos.y = foot.y + 2.5;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && grounded == true)
        cameraPos.y = foot.y + 2.5;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        renderTriangle = !renderTriangle;

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        mouseControls = !mouseControls;

    if (mouseControls)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    this->foot = this->cameraPos;
    this->foot.y -= 2.5;

    updateDeltaTime();
}

void OurCamera::updateMouse(double xposIn, double yposIn)
{
    if (mouseControls)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
}

void OurCamera::updatePosition(glm::vec3 position)
{
    this->spawnPoint = position;
    this->cameraPos = position;
}

void OurCamera::updateDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
}

void OurCamera::resetCamera() {
    cameraPos = spawnPoint;
    velocity = 15.0;
    gravity = 0.05;
}

glm::vec3 OurCamera::getCameraPos()
{
    return(cameraPos);
}

glm::vec3 OurCamera::getFoot()
{
    return(foot);
}

glm::vec3 OurCamera::getCameraFront()
{
    return(cameraFront);
}

glm::vec3 OurCamera::getCameraUp()
{
    return(cameraUp);
}

float OurCamera::getDeltaTime()
{
    return this->deltaTime;
}

void OurCamera::setGrounded(bool grounded)
{
    this->grounded = grounded;
}

void OurCamera::setHeight(float height)
{
    this->cameraPos.y = height;
}

bool OurCamera::getRenderType()
{
    return renderTriangle;
}