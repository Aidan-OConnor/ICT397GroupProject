#include "Physics.h"

using namespace reactphysics3d;

Physics::Physics()
{
    timeStep = 1.0 / 144.0;
    currentFrame = std::chrono::system_clock::now();
    prevFrame = std::chrono::system_clock::now();
    accumulator = 0.0;

    settings.gravity = Vector3(0, -10000, 0);
    world = physicsCommon.createPhysicsWorld(settings);
}

void Physics::createTestCapsule()
{
    // Rigidbody for a capsule at the centre of the scene, using for testing, can be used for volcano hitbox
    position = Vector3(0, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall = world->createRigidBody(transform);
    wall->setType(BodyType::STATIC);

    // Collision body for testing capsule
    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(515, 800);
    transform = Transform::identity();
    collider = wall->addCollider(capsuleShape, transform);
}

void Physics::createMainIslandBoundary()
{
    position = Vector3(0, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    boundaries = world->createRigidBody(transform);
    boundaries->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(2275, 200);
    transform = Transform::identity();
    collider = boundaries->addCollider(capsuleShape, transform);
}

void Physics::createIslandBoundary1()
{
    position = Vector3(5700, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    island1 = world->createRigidBody(transform);
    island1->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(1200, 200);
    transform = Transform::identity();
    collider = island1->addCollider(capsuleShape, transform);
}

void Physics::createIslandBoundary2()
{
    position = Vector3(-5400, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    island2 = world->createRigidBody(transform);
    island2->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(1200, 200);
    transform = Transform::identity();
    collider = island2->addCollider(capsuleShape, transform);
}

void Physics::createIslandBoundary4()
{
    position = Vector3(-2850, 0, -6000);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    island4 = world->createRigidBody(transform);
    island4->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(1200, 200);
    transform = Transform::identity();
    collider = island4->addCollider(capsuleShape, transform);
}

void Physics::createIslandBoundary5()
{
    position = Vector3(-1455, 0, 6130);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    island5 = world->createRigidBody(transform);
    island5->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(1200, 200);
    transform = Transform::identity();
    collider = island5->addCollider(capsuleShape, transform);
}

void Physics::createIslandBoundary6()
{
    position = Vector3(2640, 0, 5245);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    island6 = world->createRigidBody(transform);
    island6->setType(BodyType::STATIC);

    CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(1200, 200);
    transform = Transform::identity();
    collider = island6->addCollider(capsuleShape, transform);
}

void Physics::createMapBoundary()
{
    position = Vector3(0, 0, -12330);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall = world->createRigidBody(transform);
    wall->setType(BodyType::STATIC);

    BoxShape* boxShape = physicsCommon.createBoxShape(Vector3(26000, 200, 200));
    transform = Transform::identity();
    collider = wall->addCollider(boxShape, transform);

    position = Vector3(0, 0, 12330);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall2 = world->createRigidBody(transform);
    wall2->setType(BodyType::STATIC);

    BoxShape* boxShape2 = physicsCommon.createBoxShape(Vector3(26000, 200, 200));
    transform = Transform::identity();
    collider = wall2->addCollider(boxShape2, transform);

    position = Vector3(-12330, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall3 = world->createRigidBody(transform);
    wall3->setType(BodyType::STATIC);

    BoxShape* boxShape3 = physicsCommon.createBoxShape(Vector3(200, 200, 26000));
    transform = Transform::identity();
    collider = wall3->addCollider(boxShape2, transform);

    position = Vector3(12330, 0, 0);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* wall4 = world->createRigidBody(transform);
    wall4->setType(BodyType::STATIC);

    BoxShape* boxShape4 = physicsCommon.createBoxShape(Vector3(200, 200, 26000));
    transform = Transform::identity();
    collider = wall4->addCollider(boxShape2, transform);
}

void Physics::createCameraBody(Camera& camera)
{
    // Camera rigidbody
    glm::vec3 camPos = camera.getCameraPos();
    position = Vector3(camPos.x, camPos.y, camPos.z);
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    camBody = world->createRigidBody(transform);
    camBody->setType(BodyType::DYNAMIC);
    camBody->enableGravity(true);

    // Camera collision body in the shape of a sphere
    SphereShape* sphereShape = physicsCommon.createSphereShape(20);
    transform = Transform::identity();
    collider = camBody->addCollider(sphereShape, transform);
    camBody->setMass(0.01);
    camBody->setIsAllowedToSleep(false);
    camBody->setAngularDamping(0.2);
    camBody->setLinearDamping(0.2);
}

void Physics::updateBodies(Camera& camera, ImGuiData& player)
{
    // Getting time between frames for physics updates
    currentFrame = std::chrono::system_clock::now();
    deltaTime = currentFrame - prevFrame;
    prevFrame = currentFrame;
    accumulator += deltaTime.count();

    // Updating position of camera rigidbody
    glm::vec3 camPos;
    camPos = camera.getCameraPos();
    glm::vec3 playerPosition = player.getTranslation();
    Vector3 boatPos;
    if (camera.getPerspective())
    {
        position = Vector3(camPos.x, camPos.y, camPos.z);
        orientation = Quaternion::identity();
        transform = Transform(position, orientation);
        camBody->setTransform(transform);
    }
    else
    {
        boatPos = Vector3(playerPosition.x, playerPosition.y, playerPosition.z);
        orientation = Quaternion::identity();
        transform = Transform(boatPos, orientation);
        camBody->setTransform(transform);
    }

    // Update the physics world
    while (accumulator >= timeStep)
    {
        world->update(timeStep);
        accumulator -= timeStep;
    }

    // Update the position of the camera to match the rigidbody, this will prevent the camera itself from moving through other colliders
    transform = camBody->getTransform();
    position = transform.getPosition();
    boatPos = transform.getPosition();
    camBody->setAngularVelocity(Vector3(0, 0, 0));
    camBody->setLinearVelocity(Vector3(0, 0, 0));
    glm::vec3 newCamPos(position.x, position.y, position.z);
    glm::vec3 newBoatPos(boatPos.x, boatPos.y, boatPos.z);

    if (camera.getPerspective())
    {
        camera.updatePosition(newCamPos);
    }
    else
    {
        player.setTranslation(newBoatPos);
    }
}

void Physics::createTerrain(ImGuiData imGuiData)
{
    std::vector<Terrain> terrains;
    terrains = imGuiData.getTerrains();

    heightData = new float[terrains[0].getWidth() * terrains[0].getWidth()];

    for (int x = 0; x < terrains[0].getWidth(); ++x) {
        for (int z = 0; z < terrains[0].getWidth(); ++z) {
            heightData[x * terrains[0].getWidth() + z] = terrains[0].getHeightAtPos(static_cast<float>(x), static_cast<float>(z));
        }
    }

    HeightFieldShape* terrainShape = physicsCommon.createHeightFieldShape(terrains[0].getWidth(), terrains[0].getWidth(), -100, 200, heightData, HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1, 1.0, Vector3(10, 1, 10));

    position = Vector3(0, 125, 0);  
    orientation = Quaternion::identity();
    transform = Transform(position, orientation);
    RigidBody* rigidBody = world->createRigidBody(transform);
    rigidBody->setType(BodyType::STATIC);

    collider = rigidBody->addCollider(terrainShape, Transform::identity());
    collider->getMaterial().setBounciness(0.0f);  // Adjust as needed
    collider->getMaterial().setFrictionCoefficient(0.1f);  // Adjust as needed
}

void Physics::setGravity(bool gravity)
{
    camBody->enableGravity(gravity);
}

void Physics::toggleBoundaries(bool check)
{
    boundaries->setIsActive(check);
    island1->setIsActive(check);
    island2->setIsActive(check);
    island4->setIsActive(check);
    island5->setIsActive(check);
    island6->setIsActive(check);
}