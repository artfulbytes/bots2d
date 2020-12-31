#ifndef QUAD_COMPONENT_H_
#define QUAD_COMPONENT_H_

#include "components/RenderableComponent.h"
#include "QuadCoords.h"

#include <glm/glm.hpp>

class QuadTransform;

/**
 * Renders a quad from four arbitrary points (QuadCoords).
 *
 * It can't be used directly in a Scene, instead it must be assigned to
 * a Scene object to be updated each simulation iteration.
 */
class QuadComponent : public RenderableComponent
{
public:
    QuadComponent(const QuadTransform *transform, const glm::vec4 &color);
    ~QuadComponent();

    void onFixedUpdate() override;
private:
    const QuadTransform *const m_transform = nullptr;
    glm::vec4 m_color;
};

#endif /* QUAD_COMPONENT_H_ */
