#ifndef QUAD_COMPONENT_H_
#define QUAD_COMPONENT_H_

#include "RenderableComponent.h"
#include "Transforms.h"
#include "Renderer.h"
#include <cassert>

/* TODO: Rename the renderable components */
class QuadComponent : public RenderableComponent
{
    public:
        /* TODO Const pointer to transform? */
        QuadComponent(const glm::vec4& color) :
            RenderableComponent(color) {}
        void render() {
            /* TODO: Handle the graphics component the same way as the physics component */
            TransformComponent *transformComp = m_parent->getTransform();
            QuadTransform *transform = dynamic_cast<QuadTransform*>(transformComp);
            /* Transform of parent must be a quad transform! */
            assert(transform != nullptr);
            Renderer::drawQuad(transform->position, transform->size, transform->rotation, m_color);
        }
};

#endif /* QUAD_COMPONENT_H_ */
