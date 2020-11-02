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
    transform->position.x = 10.0f;
    transform->position.y = 300.0f;
    transform->position.z = 0.0f;
    transform->size.x = 100.0f;
    transform->size.y = 100.0f;
    transform->rotation = 1.0f;
    QuadComponent *quadComponent = new QuadComponent(color);
    m_scene->addObject(new SceneObject(transform, quadComponent, nullptr));

    LineTransform *transformLine = new LineTransform();
    transformLine->start.x = 200.0f;
    transformLine->end.x = 400.0f;
    transformLine->start.y = 300.0f;
    transformLine->end.y = 600.0f;
    transformLine->width = 5.0f;
    LineComponent *lineComponent = new LineComponent(color);
    m_scene->addObject(new SceneObject(transformLine, lineComponent, nullptr));

    CircleTransform *transformCircle = new CircleTransform();
    transformCircle->position.x = 700.0f;
    transformCircle->position.y = 700.0f;
    transformCircle->radius = 200.0f;
    CircleComponent *circleComponent = new CircleComponent(color);
    m_scene->addObject(new SceneObject(transformCircle, circleComponent, nullptr));
}
