#pragma once
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <Vector>

#include "reactphysics3d/reactphysics3d.h"
#include "Camera.h"
#include "ImGuiData.h"

using namespace reactphysics3d;

/*
 * @class Physics
 * @brief Manages the rigidbodies and colliders for physics objects
 *
 * Physics creates and updates physics bodies for interactions that
 * require physics interactions in the engine
 *
 * @author Lochlyn Edward
 * @version 01
 * @date 5/6/2023 Lochlyn Edward, created
 *
 * @bug No known bugs
 */
class Physics
{
public:

	/*
	* @brief Physics constructor
	*
	* Sets up the physicsWorld and its settings
	*
	* @return void
	*/
	Physics();

	/*
	* @brief Creates a large box collider in the middle of the scene for testing
	*
	* @return void
	*/
	void createTestCapsule();

	/*
	* @brief Creates the collider and rigidbody for the player camera
	*
	* @param Camera
	*
	* @return void
	*/
	void createCameraBody(Camera& camera);

	void createBoatBody(glm::vec3 boatPos);

	/*
	* @brief Updates the position of rigidbodies and colliders in the scene based on physics interactions
	*
	* @param Camera
	*
	* @return void
	*/
	void updateBodies(Camera& camera, ImGuiData &player);

	/*
	* @brief Creates a terrain collider and rigidbody for the terrain objects in the scene
	*
	* @return void
	*/
	void createTerrain(ImGuiData imGuiData);

	void createMainIslandBoundary();

	void createIslandBoundary1();

	void createIslandBoundary2();

	void createIslandBoundary3();

	void createIslandBoundary4();

	void createIslandBoundary5();

	void createIslandBoundary6();

	void createMapBoundary();

	void setGravity(bool gravity);

	void toggleBoundaries(bool check);

private:

	PhysicsCommon physicsCommon; /// Reactphysics3d physicsCommon variable for managing physics

	PhysicsWorld* world; /// Reactphysics3d world variable for managing physics objects contained in the world

	float timeStep; /// Float variable to determine when to update physics

	std::chrono::duration<double> deltaTime; /// Float variable for time between frames

	std::chrono::time_point<std::chrono::system_clock> currentFrame; /// Float variable for time of current frame

	std::chrono::time_point<std::chrono::system_clock> prevFrame; /// Float variable for time of previous frame

	long double accumulator; /// Long double variable for determing whether the physics world will be updated

	Vector3 position; /// Reactphysics3d Vector3 variable for position of bodies

	Quaternion orientation; /// Reactphysics3d Quaternion variable for orientation of bodies

	Transform transform; /// Reactphysics3d Transform variable for transform of bodies

	RigidBody* camBody; /// Reactphysics3d RigidBody variable for collisions

	RigidBody* boundaries; // Main island

	RigidBody* island1; // Outer islands

	RigidBody* island2;

	RigidBody* island4;

	RigidBody* island5;

	RigidBody* island6;

	Collider* collider; /// Reactphysics3d Collider variable for collisions

	PhysicsWorld::WorldSettings settings; /// Reactphysics3d setting variable for altering settings of the physics world

	float* heightData; /// Float variable for array of heightfield values
};