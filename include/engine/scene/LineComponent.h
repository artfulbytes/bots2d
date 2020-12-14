#ifndef LINE_COMPONENT_H_
#define LINE_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

class LineComponent : public RenderableComponent
{
    public:
        LineComponent(const LineTransform *transform, const glm::vec4& color) :
            m_transform(transform),
            m_color(color) {}
        void onFixedUpdate() override {
            assert(m_transform != nullptr);
            Renderer::drawLine(m_transform->start, m_transform->end, m_transform->width, m_color);
        }
    private:
        const LineTransform *const m_transform = nullptr;
        glm::vec4 m_color;
};

#endif /* LINE_COMPONENT_H_ */
