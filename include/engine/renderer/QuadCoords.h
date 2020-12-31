#ifndef QUAD_COORDS_H_
#define QUAD_COORDS_H_
#include <glm/glm.hpp>

struct QuadCoords {
    glm::vec2 BottomLeft = {-0.5f, -0.5f};
    glm::vec2 BottomRight = {0.5, -0.5f};
    glm::vec2 TopRight = {0.5f, 0.5f};
    glm::vec2 TopLeft = {-0.5f, 0.5f};

    QuadCoords() {}
    QuadCoords(glm::vec2 bottomLeft, glm::vec2 bottomRight, glm::vec2 topRight, glm::vec2 topLeft) :
        BottomLeft(bottomLeft), BottomRight(bottomRight), TopRight(topRight), TopLeft(topLeft) {}
    QuadCoords(const QuadCoords& quadCoords) :
        BottomLeft(quadCoords.BottomLeft), BottomRight(quadCoords.BottomRight),
        TopRight(quadCoords.TopRight), TopLeft(quadCoords.TopLeft) {}

    void operator*=(const float multiplier)
    {
        this->BottomLeft *= multiplier;
        this->BottomRight *= multiplier;
        this->TopRight *= multiplier;
        this->TopLeft *= multiplier;
    }

    QuadCoords operator*(const float multiplier)
    {
        QuadCoords quadCoords =
        {
            this->BottomLeft * multiplier,
            this->BottomRight * multiplier,
            this->TopRight * multiplier,
            this->TopLeft * multiplier
        };
        return quadCoords;
    }
};

#endif /* QUAD_COORDS_H_ */
