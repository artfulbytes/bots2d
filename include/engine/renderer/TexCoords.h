#ifndef TEX_COORDS_H_
#define TEX_COORDS_H_
#include <glm/glm.hpp>

namespace {
    float texCoordMax = 1.0f;
    float texCoordMin = 0.0f;
}

struct TexCoords {
    glm::vec2 BottomLeft = {0.0f, 0.0f};
    glm::vec2 BottomRight = {1.0f, 0.0f};
    glm::vec2 TopRight = {1.0f, 1.0f};
    glm::vec2 TopLeft = {0.0f, 1.0f};

    void assertLimits() const {
        assertLimit(BottomLeft);
        assertLimit(BottomRight);
        assertLimit(TopRight);
        assertLimit(TopLeft);
    }

    static void assertLimit(const glm::vec2 &vec) {
        assert(texCoordMin <= vec.x && vec.x <= texCoordMax);
        assert(texCoordMin <= vec.y && vec.y <= texCoordMax);
    }

    void operator=(const TexCoords &texCoords) {
        this->BottomLeft = texCoords.BottomLeft;
        this->BottomRight = texCoords.BottomRight;
        this->TopRight = texCoords.TopRight;
        this->TopLeft = texCoords.TopLeft;
        this->assertLimits();
    }
};

#endif /* TEX_COORDS_H_ */
