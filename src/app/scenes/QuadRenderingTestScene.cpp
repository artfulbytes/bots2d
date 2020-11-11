#include "QuadRenderingTestScene.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "LineComponent.h"
#include "CircleComponent.h"

#include <glm/glm.hpp>

QuadRenderingTestScene::QuadRenderingTestScene()
{
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    QuadTransform *transform = new QuadTransform();
    transform->position.x = 3.0f;
    transform->position.y = 3.0f;
    transform->position.z = 0.0f;
    transform->size.x = 0.5f;
    transform->size.y = 0.5f;
    transform->rotation = 1.0f;
    QuadComponent *quadComponent = new QuadComponent(color);
    m_scene->createObject(transform, quadComponent, nullptr, nullptr);

    LineTransform *transformLine = new LineTransform();
    transformLine->start.x = 1.0f;
    transformLine->start.y = 1.0f;
    transformLine->end.x = 2.0f;
    transformLine->end.y = 2.0f;
    transformLine->width = 0.05f;
    LineComponent *lineComponent = new LineComponent(color);
    m_scene->createObject(transformLine, lineComponent, nullptr, nullptr);

    CircleTransform *transformCircle = new CircleTransform();
    transformCircle->position.x = 0.0f;
    transformCircle->position.y = 0.0f;
    transformCircle->radius = 25.0f;
    CircleComponent *circleComponent = new CircleComponent(color);
    m_scene->createObject(transformCircle, circleComponent, nullptr, nullptr);
}
