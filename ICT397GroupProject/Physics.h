#pragma once

#include "reactphysics3d/reactphysics3d.h"
#include "Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <Vector>

using namespace reactphysics3d;

class Physics
{
public:
	
	Physics();

	void createTestCube();

	void createCameraBody(Camera &camera);

	void updateBodies(Camera &camera);

private:

	PhysicsCommon physicsCommon;

	PhysicsWorld* world;

	float timeStep;

	std::chrono::duration<double> deltaTime;

	std::chrono::time_point<std::chrono::system_clock> currentFrame;

	std::chrono::time_point<std::chrono::system_clock> prevFrame;

	long double accumulator;

	Vector3 position;

	Quaternion orientation;

	Transform transform;

	RigidBody* camBody;

	Collider* collider;
};