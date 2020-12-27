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
        void setEnabled(float enabled)
        {
            m_enabled = enabled;
        }

    protected:
        bool m_enabled = true;
};

#endif /* RENDERABLE_COMPONENT_H_ */
