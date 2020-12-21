#ifndef HOLLOW_CIRCLE_COMPONENT_H_
#define HOLLOW_CIRCLE_COMPONENT_H_

#include "Renderer.h"
#include "components/RenderableComponent.h"
#include "components/Transforms.h"

#include <cassert>

class HollowCircleComponent : public RenderableComponent
{
    public:
        HollowCircleComponent(const HollowCircleTransform *transform, const glm::vec4 &fillColor, const glm::vec4 &borderColor) :
            m_transform(transform), m_fillColor(fillColor), m_borderColor(borderColor) {
            assert(transform != nullptr);
        }

        void onFixedUpdate() override {
            Renderer::drawCircle(m_transform->position, m_transform->outerRadius, m_borderColor);
            Renderer::drawCircle(m_transform->position, m_transform->innerRadius, m_fillColor);
        }
    private:
        const HollowCircleTransform *const m_transform = nullptr;
        const glm::vec4 m_fillColor;
        const glm::vec4 m_borderColor;
};

#endif /* HOLLOW_CIRCLE_COMPONENT_H_ */
