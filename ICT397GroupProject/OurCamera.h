#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>

class OurCamera
{
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 foot;
    glm::vec3 spawnPoint;
    bool firstMouse;
    bool grounded;
    float velocity = 15.0;
    float gravity = 0.05;
    float stamina = 60.0;
    float dt;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float deltaTime;
    float prevFrame;

    bool renderTriangle;
    bool mouseControls;

public:
    OurCamera();

    void processInput(GLFWwindow* window);

    void updateMouse(double xposIn, double yposIn);

    void updatePosition(glm::vec3 position);

    void updateDeltaTime();

    void resetCamera();

    glm::vec3 getCameraPos();

    glm::vec3 getFoot();

    glm::vec3 getCameraFront();

    glm::vec3 getCameraUp();

    float getDeltaTime();

    void setGrounded(bool grounded);

    void setHeight(float height);

    bool getRenderType();
};