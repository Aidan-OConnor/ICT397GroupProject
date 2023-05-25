#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"

Camera::Camera()
{
    this->cameraPos = glm::vec3(-50.0f, 0.0f, 0.0f);
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

    this->grounded = false;
    this->renderTriangle = true;
    this->mouseControls = true;
    this->firstPerson = false;
    this->level = 0;
}

void Camera::processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& playerRotation)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (firstPerson)
    {
        cameraSpeed = static_cast<float>(500.0 * deltaTime);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

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
    }
    else
    {
        float movementSpeed = 15;
        float playerRotationSpeed = 50;
        float distance = 0;

        glm::vec3 movement(0.0f);
        glm::vec3 rotation(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            distance += movementSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            distance -= movementSpeed * deltaTime;
        }

        // Handle character rotation
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotation += playerRotationSpeed * deltaTime * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotation -= playerRotationSpeed * deltaTime * glm::vec3(0.0f, 1.0f, 0.0f);
        }

        playerRotation += rotation;

        playerPosition.x += (float)(distance * glm::sin(glm::radians(playerRotation.y)));
        playerPosition.z += (float)(distance * -glm::cos(glm::radians(playerRotation.y)));
    }

    updateDeltaTime();
}

void Camera::updateMouse(double xposIn, double yposIn)
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

void Camera::updatePosition(glm::vec3 position)
{
    this->spawnPoint = position;
    this->cameraPos = position;
}

void Camera::updateDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
}

void Camera::resetCamera() {
    cameraPos = spawnPoint;
    velocity = 15.0;
    gravity = 0.05;
}

glm::vec3 Camera::getCameraPos()
{
    return(cameraPos);
}

glm::vec3 Camera::getFoot()
{
    return(foot);
}

glm::vec3 Camera::getCameraFront()
{
    return(cameraFront);
}

glm::vec3 Camera::getCameraUp()
{
    return(cameraUp);
}

float Camera::getDeltaTime()
{
    return this->deltaTime;
}

void Camera::setGrounded(bool grounded)
{
    this->grounded = grounded;
}

bool Camera::getRenderType()
{
    return renderTriangle;
}

void Camera::setCameraY(float newValue)
{
    cameraPos.y = newValue;
}

void Camera::setLevel(int value)
{
    this->level = value;
}

void Camera::swapRenderType()
{
    renderTriangle = !renderTriangle;
}

void Camera::swapMouseControls()
{
    mouseControls = !mouseControls;
}