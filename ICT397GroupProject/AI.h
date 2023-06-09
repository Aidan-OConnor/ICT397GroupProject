#pragma once
#include "ImGuiData.h"

/*
 * @class AI
 * @brief Contains data and functions for AI behaviour
 *
 * AI stores the data required to check for interactions
 * between the player and the AI and adjusts the behaviour
 * of the NPCs accordingly
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 09/06/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

class AI
{
private:
    /// AI move speed
	float movementSpeed;
    /// Running status for all AI
    std::vector<bool> isRunning;
    /// Attacking status for all AI
    std::vector<bool> isAttacking;
    /// Stores current frame for delta time
    float currentFrame;
    /// Stores the delta time
    float deltaTime;
    /// Stores the last frame for delta time
    float lastFrame;
    /// Stores the player health
    int playerHealth;
public:
    /*
     * @brief Default AI constructor
     *
     * The default constructor for an AI objects that sets the
     * movement speed and player health
     *
     * @return void
     */
    AI()
    {
        movementSpeed = 175;
        playerHealth = 40;
    }

    /*
     * @brief AI constructor with a value
     *
     * AI constructor for when AI are stored during
     * initialization
     *
     * @param NPCs
     * @return void
     */
    AI(std::vector<ImGuiData>& NPCs)
    {
        movementSpeed = 175;
        playerHealth = 40;
        
        for (int i = 0; i < NPCs.size(); i++)
        {
            isRunning.push_back(false);
            isAttacking.push_back(false);
        }
    }

    /*
     * @brief Behaviour for an AI
     *
     * This functions has the AI chase and attack
     * a player if the distance is short enough
     *
     * @return void
     */
	void chasePlayer(std::vector<ImGuiData>& NPCs, glm::vec3& player)
	{
        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (NPCs.size() > isRunning.size())
        {
            for (int i = 0; i < NPCs.size() - isRunning.size(); i++)
            {
                isRunning.push_back(false);
                isAttacking.push_back(false);
            }
        }

        for (int i = 0; i < NPCs.size(); i++)
        {
            glm::vec3 NPCposition = NPCs[i].getTranslation();
            glm::vec3 NPCrotation = NPCs[i].getRotation();
            float distance = glm::sqrt((NPCposition.x - player.x) * (NPCposition.x - player.x) +
                (NPCposition.z - player.z) * (NPCposition.z - player.z));

            if (distance <= 850.0f && distance > 50)
            {
                if (!isRunning[i])
                {
                    NPCs[i].setAnimationState(RUN);
                    isRunning[i] = true;
                    isAttacking[i] = false;
                }

                float directionX = player.x - NPCposition.x;
                float directionZ = player.z - NPCposition.z;
                float magnitude = glm::sqrt(directionX * directionX + directionZ * directionZ);

                if (magnitude > 0.0f) {
                    directionX /= magnitude;
                    directionZ /= magnitude;

                    NPCposition.x += directionX * movementSpeed * deltaTime;
                    NPCposition.z += directionZ * movementSpeed * deltaTime;

                    NPCrotation.z = glm::degrees(atan2(directionX, directionZ));
                    NPCrotation = glm::radians(NPCrotation);
                }
                
                NPCs[i].setTranslation(NPCposition);

                NPCs[i].setRotation(NPCrotation);
            }
            else if (distance <= 50)
            {
                if (!isAttacking[i])
                {
                    NPCs[i].setAnimationState(ATTACK);
                    isAttacking[i] = true;
                    isRunning[i] = false;
                }

                if (NPCs[i].getAnimStatus())
                {
                    playerHealth--;
                }
            }
            else if (distance > 250 && isRunning[i])
            {
                NPCs[i].setAnimationState(STAND);
                isRunning[i] = false;
                isAttacking[i] = false;
            }
        }
	}

    /*
     * @brief Returns player health
     *
     * This functions returns the players current
     * health as a max total of 10
     *
     * @return playerHealth/4
     */
    int getPlayerHealth()
    {
        return playerHealth/4;
    }

    /*
     * @brief Resets players health
     *
     * This function resets the player health
     * back to maximum
     *
     * @return void
     */
    void resetPlayerHealth()
    {
        playerHealth = 40;
    }
};