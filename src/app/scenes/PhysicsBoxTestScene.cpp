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
    transformBox->position.x = 2.9f;
    transformBox->position.y = 8.0f;
    transformBox->position.z = 0.0f;
    transformBox->size.x = 0.5f;
    transformBox->size.y = 0.5f;
    QuadComponent *boxComponent = new QuadComponent(color);
    auto fallingBody = new Body2D(*world, *transformBox, true, 1.0f);
    m_scene->addObject(new SceneObject(transformBox, boxComponent, fallingBody));

    QuadTransform *transformGround = new QuadTransform();
    transformGround->position.x = 2.0f;
    transformGround->position.y = 3.0f;
    transformGround->position.z = 0.0f;
    transformGround->size.x = 1.0f;
    transformGround->size.y = 0.2f;
    transformGround->rotation = 0.0f;

    QuadComponent *groundComponent = new QuadComponent(color);
    auto staticBody = new Body2D(*world, *transformGround, false, 1.0f);
    m_scene->addObject(new SceneObject(transformGround, groundComponent, staticBody));
}
