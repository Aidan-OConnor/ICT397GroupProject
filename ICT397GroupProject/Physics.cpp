#include "Physics.h"

using namespace reactphysics3d;

Physics::Physics()
{
    timeStep = 1.0 / 144.0;
    currentFrame = std::chrono::system_clock::now();
    prevFrame = std::chrono::system_clock::now();
    accumulator = 0.0;

    settings.gravity = Vector3(0, 0, 0);
    world = physicsCommon.createPhysicsWorld(settings);
}

void Physics::createTestCube()
{
    // Rigidbody for a cube at the centre of the scene, using for testing
    position = Vector3(150, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall = world->createRigidBody(transform);
    wall->setType(BodyType::STATIC);

    // Collision body for testing cube
    SphereShape* sphereShape = physicsCommon.createSphereShape(1000);
    transform = Transform::identity();
    collider = wall->addCollider(sphereShape, transform);
}

void Physics::createCameraBody(Camera &camera) 
{
    // Camera rigidbody
    glm::vec3 camPos = camera.getCameraPos();
    position = Vector3(camPos.x, camPos.y, camPos.z);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    camBody = world->createRigidBody(transform);
    camBody->setType(BodyType::DYNAMIC);
    camBody->enableGravity(true);

    // Camera collision body in the shape of a capsule
    float radius = 5.0f;
    float height = 10.0f;
    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(radius, height);
    transform = Transform::identity();
    collider = camBody->addCollider(capsuleShape, transform);
}

void Physics::updateBodies(Camera &camera)
{
    // Getting time between frames for physics updates
    currentFrame = std::chrono::system_clock::now();
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
    accumulator += deltaTime.count();

    // Updating position of camera rigidbody
    glm::vec3 camPos;
    camPos = camera.getCameraPos();
    position = Vector3(camPos.x, camPos.y, camPos.z);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    camBody->setTransform(transform);

    // Update the physics world
    while(accumulator >= timeStep)
    {
        world->update(timeStep);
        accumulator -= timeStep;
    }

    // Update the position of the camera to match the rigidbody, this will prevent the camera itself from moving through other colliders
    transform = camBody->getTransform();
    position = transform.getPosition();
    camBody->setAngularVelocity(Vector3(0, 0, 0));
    camBody->setLinearVelocity(Vector3(0, 0, 0));
    glm::vec3 newCamPos(position.x, position.y, position.z);
    camera.updatePosition(newCamPos);
}

void Physics::createTerrain(ImGuiData &imGuiData)
{
    std::vector<Terrain> terrains;
    terrains = imGuiData.getTerrains();
    std::vector<glm::vec3> temp = terrains[0].getVertices();

    heightData = new float[terrains[0].getSize() * terrains[0].getSize()];
    HeightFieldShape* terrainShape;

    for (int x = 0; x < terrains[0].getSize(); ++x) {
        for (int z = 0; z < terrains[0].getSize(); ++z) {
            heightData[x * terrains[0].getSize() + z] = terrains[0].getHeightAtPos(x, z, terrains[0].getSize() * terrains[0].getSize());
        }
    }

    terrainShape = physicsCommon.createHeightFieldShape(terrains[0].getSize(), terrains[0].getSize(), 0, 512, heightData, HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);
    
    position = Vector3(-50, 50, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* rigidBody = world->createRigidBody(transform);
    rigidBody->setType(BodyType::STATIC);
    
    collider = rigidBody->addCollider(terrainShape, Transform::identity());
    collider->getMaterial().setBounciness(0.0f);  // Adjust as needed
    collider->getMaterial().setFrictionCoefficient(0.5f);  // Adjust as needed
}