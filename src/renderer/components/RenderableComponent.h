#ifndef RENDERABLE_COMPONENT_H_
#define RENDERABLE_COMPONENT_H_

#include "Component.h"
#include <glm/glm.hpp>

/**
 * Base class for components that renders.
 */
class RenderableComponent : public Component
{
    public:
        RenderableComponent() {}
        virtual ~RenderableComponent() {}
        /**
        * Called every simulation iteration (if assigned to a Scene Object).
        */
        virtual void onFixedUpdate() = 0;
        /**
         * Enable or disable rendering.
         */
        void setEnabled(float enabled)
        {
            m_enabled = enabled;
        }

    protected:
        bool m_enabled = true;
};

#endif /* RENDERABLE_COMPONENT_H_ */
