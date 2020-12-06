#ifndef CIRCLE_COMPONENT_H_
#define CIRCLE_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

class CircleComponent : public RenderableComponent
{
    public:
        CircleComponent(const glm::vec4& color) : m_color(color) {}
        void onFixedUpdate() override {
            const auto transform = dynamic_cast<const CircleTransform*>(m_parent->getTransform());
            /* Transform of parent must be a circle transform! */
            assert(transform != nullptr);
            Renderer::drawCircle(transform->position, transform->radius, m_color);
        }
    private:
        glm::vec4 m_color;
};

#endif /* CIRCLE_COMPONENT_H_ */
