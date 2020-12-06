#ifndef LINE_COMPONENT_H_
#define LINE_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

class LineComponent : public RenderableComponent
{
    public:
        LineComponent(const glm::vec4& color) : m_color(color) {}
        void onFixedUpdate() override {
            const auto transform = dynamic_cast<const LineTransform*>(m_parent->getTransform());
            /* Transform of parent must be a line transform! */
            assert(transform != nullptr);
            Renderer::drawLine(transform->start, transform->end, transform->width, m_color);
        }
    private:
        glm::vec4 m_color;
};

#endif /* LINE_COMPONENT_H_ */
