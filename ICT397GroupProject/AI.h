#pragma once
#include "ImGuiData.h"

class AI
{
private:
	float movementSpeed;
    std::vector<bool> isRunning;
    float currentFrame;
    float deltaTime;
    float lastFrame;
public:
    AI()
    {
        movementSpeed = 100;
    }

    AI(std::vector<ImGuiData>& NPCs)
    {
        movementSpeed = 100;
        
        for (int i = 0; i < NPCs.size(); i++)
        {
            isRunning.push_back(false);
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
            }
        }

        for (int i = 0; i < NPCs.size(); i++)
        {
            glm::vec3 NPCposition = NPCs[i].getTranslation();
            glm::vec3 NPCrotation = NPCs[i].getRotation();
            float distance = glm::sqrt((NPCposition.x - player.x) * (NPCposition.x - player.x) +
                (NPCposition.z - player.z) * (NPCposition.z - player.z));

            if (distance <= 250.0f)
            {
                if (!isRunning[i])
                {
                    NPCs[i].setAnimationState(RUN);
                    isRunning[i] = true;
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
            else if (distance > 50 && isRunning[i])
            {
                NPCs[i].setAnimationState(STAND);
                isRunning[i] = false;
            }
        }

        //if (distance <= 5.0f) {
        //    float directionX = playerX - positionX;
        //    float directionZ = playerZ - positionZ;
        //    float magnitude = glm::sqrt(directionX * directionX + directionZ * directionZ);
        //
        //    directionX /= magnitude;
        //    directionZ /= magnitude;
        //
        //    positionX += directionX * speed * deltaTime;
        //    positionZ += directionZ * speed * deltaTime;
        //}
	}
};