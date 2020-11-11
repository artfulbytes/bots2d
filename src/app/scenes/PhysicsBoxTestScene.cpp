#include "PhysicsBoxTestScene.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "Body2D.h"

#include <glm/glm.hpp>

PhysicsBoxTestScene::PhysicsBoxTestScene()
{
    PhysicsWorld *world = new PhysicsWorld(PhysicsWorld::Gravity::SideView);
    m_scene->setPhysicsWorld(world);

    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    QuadTransform *transformBox = new QuadTransform();
    transformBox->position.x = 1.0f;
    transformBox->position.y = 1.0f;
    transformBox->position.z = 0.0f;
    transformBox->size.x = 0.05f;
    transformBox->size.y = 0.05f;
    QuadComponent *boxComponent = new QuadComponent(color);
    auto fallingBody = new Body2D(*world, *transformBox, true, 1.0f);
    m_scene->createObject(transformBox, boxComponent, fallingBody, nullptr);

    QuadTransform *transformGround = new QuadTransform();
    transformGround->position.x = 1.03f;
    transformGround->position.y = 0.0f;
    transformGround->position.z = 0.0f;
    transformGround->size.x = 0.2f;
    transformGround->size.y = 0.05f;
    transformGround->rotation = 0.0f;

    QuadComponent *groundComponent = new QuadComponent(color);
    auto staticBody = new Body2D(*world, *transformGround, false, 1.0f);
    m_scene->createObject(transformGround, groundComponent, staticBody, nullptr);
}
