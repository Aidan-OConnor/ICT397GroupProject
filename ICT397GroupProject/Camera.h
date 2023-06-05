#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

    /**
     * @class Camera
     * @brief Manages the first person camera and movement of the scene
     *
     * @author Aidan O'Connor
     * @version 01
     * @date 10/03/2023 Aidan O'Connor, created
     *
     * @author Lochlyn Edward
     * @version 02
     * @date 21/03/2023 Lochlyn Edward, added and modified functions
     *
     * @todo Implement collision and improve overall camera movement
     *
     * @bug No known bugs
     */

class Camera
{
private:
    glm::vec3 cameraPos; /// Vector for position of the camera
    glm::vec3 cameraFront; /// Vector for front direction of the camera
    glm::vec3 cameraUp; /// Vector for the up direction of the camera
    glm::vec3 spawnPoint; /// Vector for the location the camera will start/be reset to
    glm::vec3 playerPosition;
    glm::vec3 playerRotation;
    bool firstMouse; /// Boolean for the initial use of the mouse
    bool grounded; /// Boolean to check if the camera is on the ground
    float velocity = 500.0; /// Float for the movement speed of the camera
    float gravity = 0.05; /// Float for gravity of the camera, affects how fast it will fall
    float stamina = 60.0; /// Float for stamina of the character, determines whether sprinting is possible
    float cameraSpeed; ///Determines the speed at which the camera moves
    float yaw; /// Float for yaw angle, used to make the camera look around
    float pitch; /// Float for pitch of the camera, used to make the camera look around
    float lastX; /// Float for the latest X value of the mouse cursor
    float lastY; /// Float for the latest Y value of the mouse cursor
    float deltaTime; /// Float for deltaTime, used to manage movement of camera
    float prevFrame; /// Float for the previous frame, used to calculate deltaTime
    int level; /// Integer for the Y level of the camera, used to keep it on a particular Y level

    float playerRotationSpeed;

    bool renderTriangle; /// Boolean used to determine whether the program renders the terrain in wireframe mode or not
    bool mouseControls; /// Boolean used to determine what mode of mouse control the user is in, allows for a switch between first person control and mouse cursor control
    bool firstPerson; ///Determines whether the camera is a first person or third person controller

    float cameraDistance;
    float cameraHeight;
    float maxMoveSpeed;
    float movementSpeed;
    float forwardSpeed;
    float backwardSpeed;

public:
    /*
    * @brief Default Camera constructor
    *
    * Default constructor to create a camera object. 
    * Sets the variables of the camera object to 
    * pre-determined default values
    *
    * @return void
    */
    Camera();

     /*
     * @brief Processes key input
     *
     * This function is used to process key input into
     * movement and functions of the camera, such as
     * walking, changing between render modes, and
     * exiting the game
     *
     * @param window
     * @return void
     */
    void processInput(GLFWwindow* windowm, glm::vec3& PlayerPosition, glm::vec3& PlayerRotation);

    /*
    * @brief Processes mouse
    *
    * This function is used to process mouse input that
    * allows for looking around using the mouse
    *
    * @param xposIn, yposIn
    * @return void
    */
    void updateMouse(double xposIn, double yposIn);

    /*
    * @brief Updates the camera position and spawn point
    *
    * This function is used to set the cameras position
    * and spawn point as per the position passed to it
    *
    * @param position
    * @return void
    */
    void updatePosition(glm::vec3 position);

    /*
    * @brief Updates the deltaTime
    *
    * This function is used to update the delta time
    * value that is used to move the player with key input
    *
    * @return void
    */
    void updateDeltaTime();

    /*
    * @brief Reset camera to start position
    *
    * This function is used to reset the cameras position
    * to a specified location
    *
    * @return void
    */
    void resetCamera();

    /*
    * @brief Returns the camera position
    *
    * This function is used to return the cameras position
    *
    * @return cameraPos
    */
    glm::vec3 getCameraPos();

    /*
    * @brief Returns the cameras front vector
    *
    * This function is used to return the cameras front vector
    *
    * @return cameraPos
    */
    glm::vec3 getCameraFront();

    /*
    * @brief Returns the cameras up direction
    *
    * This function is used to return the cameras up vector
    *
    * @return cameraPos
    */
    glm::vec3 getCameraUp();

    /*
    * @brief Returns the delta time value
    *
    * This function is used to return the delta time value
    *
    * @return cameraPos
    */
    float getDeltaTime();

    bool getPerspective();

    /*
    * @brief Returns the camera position
    *
    * This function is used to set the value of the grounded
    * variable after having checked for collision with the ground
    *
    * @param grounded
    * @return void
    */
    void setGrounded(bool grounded);

    /*
    * @brief Returns the render type of the camera
    *
    * This function is used to get the render type of the camera through
    * a boolean, to determine what render mode the camera is in
    *
    * @return bool
    */
    bool getRenderType();

    /*
    * @brief Sets the Y value of the camera
    *
    * This function is used to set the Y value of the camera to
    * modify its vertical position
    *
    * @param newValue
    * @return void
    */
    void setCameraY(float newValue);
    
    /*
    * @brief Sets the Y value of the camera that it should be on
    *
    * This function is used to set the Y value that the camera is meant
    * to currently be located on, attempting to keep it grounded
    *
    * @param value
    * @return void
    */
    void setLevel(int value);

    void setPerspective(bool perspective);

    void swapRenderType();

    void swapMouseControls();
};