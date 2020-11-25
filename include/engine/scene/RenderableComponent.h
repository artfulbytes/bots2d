#ifndef RENDERABLE_COMPONENT_H_
#define RENDERABLE_COMPONENT_H_

#include "Component.h"
#include <glm/glm.hpp>

class RenderableComponent : public Component
{
    public:
        RenderableComponent(const glm::vec4& color) :
            m_color(color) {}
        virtual void render() const = 0;
    protected:
        glm::vec4 m_color;
};

#endif /* RENDERABLE_COMPONENT_H_ */
