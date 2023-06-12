#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Camera.h"

Camera::Camera()
{
    this->cameraPos = glm::vec3(0, 150, 0);
    this->foot = this->cameraPos;
    this->foot.y -= 2.5;
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->spawnPoint = glm::vec3(-500, 100, 0);

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
    this->firstPerson = true;
    this->level = 0;

    this->cameraDistance = 100;
    this->cameraHeight = 50;
    this->movementSpeed = 0;
    this->forwardSpeed = 0;
    this->backwardSpeed = 0;
    this->maxMoveSpeed = 300;
}

void Camera::processInput(GLFWwindow* window, glm::vec3& PlayerPosition, glm::vec3& PlayerRotation)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (firstPerson)
    {
        cameraSpeed = static_cast<float>(175.0 * deltaTime);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos.x += cameraSpeed * cameraFront.x;
            cameraPos.z += cameraSpeed * cameraFront.z;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && stamina > 0)
        {
            cameraSpeed = static_cast<float>(200.0 * deltaTime);
            cameraPos.x += cameraSpeed * cameraFront.x;
            cameraPos.z += cameraSpeed * cameraFront.z;
            stamina -= 0.5;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos.x -= cameraSpeed * cameraFront.x;
            cameraPos.z -= cameraSpeed * cameraFront.z;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            velocity = velocity + gravity * deltaTime;
            cameraPos.y = cameraPos.y + velocity * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
            velocity = velocity + gravity * deltaTime;
            cameraPos.y = cameraPos.y - velocity * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS && stamina <= 60.0)
            stamina += 0.2;

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
        float playerRotationSpeed = 1;
        float distance = 0;
        float forwardDistance = 0;
        float backwardDistance = 0;

        glm::vec3 movement(0.0f);
        glm::vec3 rotation(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if (forwardSpeed < maxMoveSpeed)
                forwardSpeed += 100 * deltaTime;
            else
            {
                forwardSpeed = maxMoveSpeed;
            }
            backwardSpeed = 0;
            distance += forwardSpeed * deltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (backwardSpeed < maxMoveSpeed/4)
                backwardSpeed += 25 * deltaTime;
            forwardSpeed = 0;
            distance -= backwardSpeed * deltaTime;
        }
        else
        {
            if (forwardSpeed > 0)
                forwardSpeed -= 200 * deltaTime;
            if (backwardSpeed > 0)
                backwardSpeed -= 50 * deltaTime;
            distance += forwardSpeed * deltaTime;
            distance -= backwardSpeed * deltaTime;
        }

        // Handle character rotation
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotation += playerRotationSpeed * deltaTime * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotation -= playerRotationSpeed * deltaTime * glm::vec3(0.0f, 1.0f, 0.0f);
        }

        PlayerRotation += rotation;

        PlayerPosition.z += (float)(distance * glm::sin(PlayerRotation.y));
        PlayerPosition.x += (float)(distance * -glm::cos(PlayerRotation.y));

        this->playerPosition = PlayerPosition;
        this->playerRotation = PlayerRotation;

        cameraPos.x = playerPosition.x + cameraDistance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraPos.y = playerPosition.y + cameraHeight + cameraDistance * sin(glm::radians(-pitch));
        cameraPos.z = playerPosition.z + cameraDistance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
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

        if (firstPerson)
        {
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
        else
        {
            if (pitch > 20.0f)
                pitch = 20.0f;
            if (pitch < -20.0f)
                pitch = -20.0f;
        }
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

bool Camera::getPerspective()
{
    return this->firstPerson;
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

void Camera::setPerspective(bool perspective)
{
    this->firstPerson = perspective;
}

void Camera::swapRenderType()
{
    renderTriangle = !renderTriangle;
}

void Camera::swapMouseControls()
{
    mouseControls = !mouseControls;
}

void Camera::setMouseControls(bool status)
{
    this->mouseControls = status;
}

void Camera::setVelocity(float value)
{
    velocity = value;
}