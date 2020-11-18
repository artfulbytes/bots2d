#ifndef LINE_COMPONENT_H_
#define LINE_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

class LineComponent : public RenderableComponent
{
    public:
        LineComponent(const glm::vec4& color) :
            RenderableComponent(color) {}
        /* TODO: Template this? */
        /* TODO: const? */
        void render() {
            LineTransform *transform = dynamic_cast<LineTransform*>(m_parent->getTransform());
            /* Transform of parent must be a line transform! */
            assert(transform != nullptr);
            Renderer::drawLine(transform->start, transform->end, transform->width, m_color);
        }
};

#endif /* LINE_COMPONENT_H_ */
