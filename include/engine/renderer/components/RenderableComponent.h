#ifndef RENDERABLE_COMPONENT_H_
#define RENDERABLE_COMPONENT_H_

#include "Component.h"
#include <glm/glm.hpp>

class RenderableComponent : public Component
{
    public:
        RenderableComponent() {}
        virtual ~RenderableComponent() {}
        virtual void onFixedUpdate() = 0;
};

#endif /* RENDERABLE_COMPONENT_H_ */
