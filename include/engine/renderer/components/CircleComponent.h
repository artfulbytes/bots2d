#ifndef CIRCLE_COMPONENT_H_
#define CIRCLE_COMPONENT_H_

#include "Renderer.h"
#include "components/RenderableComponent.h"
#include "components/Transforms.h"

#include <cassert>

class CircleComponent : public RenderableComponent
{
    public:
        CircleComponent(const CircleTransform *transform, const glm::vec4& color) :
            m_transform(transform), m_color(color) {
            assert(transform != nullptr);
        }

        void onFixedUpdate() override {
            Renderer::drawCircle(m_transform->position, m_transform->radius, m_color);
        }
    private:
        const CircleTransform *const m_transform = nullptr;
        glm::vec4 m_color;
};

#endif /* CIRCLE_COMPONENT_H_ */
