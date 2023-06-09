#pragma once
#include "ImGuiData.h"

class AI
{
private:
	float movementSpeed;
    std::vector<bool> isRunning;
    std::vector<bool> isAttacking;
    float currentFrame;
    float deltaTime;
    float lastFrame;
    int playerHealth;
public:
    AI()
    {
        movementSpeed = 175;
        playerHealth = 40;
    }

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

    int getPlayerHealth()
    {
        return playerHealth/4;
    }

    void resetPlayerHealth()
    {
        playerHealth = 40;
    }
};