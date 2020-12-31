#include "playgrounds/LineFollowerPath.h"
#include "shapes/QuadObject.h"
#include "SceneObject.h"
#include "Renderer.h"

#include <unordered_map>
#include <vector>
#include <iostream>

namespace {
const std::unordered_map<LineFollowerPath::Blueprint, LineFollowerPath::PathPoints> pathBlueprints
({
    { LineFollowerPath::Blueprint::Simple,
        {
            { -0.50f, -0.25f },
            { -0.50f,  0.25f },
            {  0.50f,  0.25f },
            {  0.50f, -0.25f },
        }
    },
    { LineFollowerPath::Blueprint::Tshaped,
        {
            { -0.25f,  0.00f },
            { -0.50f,  0.00f },
            { -0.50f,  0.25f },
            {  0.50f,  0.25f },
            {  0.50f,  0.00f },
            {  0.25f,  0.00f },
            {  0.25f, -0.25f },
            { -0.25f, -0.25f },
        }
    },
    { LineFollowerPath::Blueprint::Mshaped,
        {
            { -0.25f,  0.15f },
            { -0.25f, -0.35f },
            { -0.50f, -0.35f },
            { -0.50f,  0.40f },
            {  0.50f,  0.40f },
            {  0.50f, -0.35f },
            {  0.25f, -0.35f },
            {  0.25f,  0.15f },
        }
    },
});

enum class Direction {
    UpLeft,
    LeftUp,
    DownLeft,
    LeftDown,
    UpRight,
    RightUp,
    DownRight,
    RightDown
};

Direction getDirection(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2)
{
    if (p0.x == p2.x || p0.y == p2.y) {
        std::cout << "Angle between line segments (p0-p1) and (p1-p2) must be right-angled (90 degrees)!" << std::endl;
        assert(false);
    }

    if (p2.x < p0.x) {
        /* Left */
        if (p2.y > p0.y) {
            /* Up */
            if (p0.x == p1.x) {
                return Direction::UpLeft;
            } else {
                return Direction::LeftUp;
            }
        } else {
            /* Down */
            if (p0.x == p1.x) {
                return Direction::DownLeft;
            } else {
                return Direction::LeftDown;
            }
        }
    } else {
        /* Right */
        if (p2.y > p0.y) {
            /* Up */
            if (p0.x == p1.x) {
                return Direction::UpRight;
            } else {
                return Direction::RightUp;
            }
        } else {
            /* Down */
            if (p0.x == p1.x) {
                return Direction::DownRight;
            } else {
                return Direction::RightDown;
            }
        }
    }
}
}

const LineFollowerPath::PathPoints &LineFollowerPath::getBlueprintPathPoints(LineFollowerPath::Blueprint blueprint)
{
    auto blueprintItr = pathBlueprints.find(blueprint);
    if (blueprintItr == pathBlueprints.end()) {
        std::cout << "Blueprint not found!" << std::endl;
        assert(0);
    }
    return blueprintItr->second;
}

LineFollowerPath::LineFollowerPath(Scene *scene, const glm::vec4 &lineColor,
                                   float lineWidth, const LineFollowerPath::PathPoints &pathPoints) :
    SceneObject(scene)
{
    const auto quadCoords = getRightAnglePathQuadCoords(pathPoints, lineWidth);
    const Body2D::Specification bodySpec;
    for (const auto &quadCoord : quadCoords) {
        m_pathQuads.push_back(std::make_unique<QuadObject>(scene, quadCoord, lineColor, &bodySpec, true));
    }
}

LineFollowerPath::~LineFollowerPath()
{
}

std::vector<QuadCoords> LineFollowerPath::getRightAnglePathQuadCoords(const LineFollowerPath::PathPoints &pathPoints, const float width)
{
    std::vector<QuadCoords> quadCoords;
    QuadCoords prevQuadCoords;
    quadCoords.push_back(prevQuadCoords);
    const float normalLength = width / std::sqrt(2);
    for (unsigned int i = 1; i < pathPoints.size() + 1; i++) {
        const glm::vec2 p0 = pathPoints[i-1];
        const glm::vec2 p1 = pathPoints[i % pathPoints.size()];
        const glm::vec2 p2 = pathPoints[(i+1) % pathPoints.size()];

        glm::vec2 normal = (-(p1 - p0) / glm::length(p1 - p0)) + ((p2 - p1) / glm::length(p2-p1));
        normal /= glm::length(normal);
        normal *= normalLength;

        auto prevQuadCoordsPtr = &quadCoords.back();
        QuadCoords nextQuadCoords;
        auto nextQuadCoordsPtr = &nextQuadCoords;


        const bool isLastPoint = (i == pathPoints.size());
        if (isLastPoint) {
            /* Assumes last quad attaches nicely with first quad, that is,
             * the pathPoints must form a complete loop */
            nextQuadCoordsPtr = &quadCoords.front();
        }

        switch (getDirection(p0, p1, p2)) {
        case Direction::UpRight:
            nextQuadCoordsPtr->TopLeft = prevQuadCoordsPtr->TopLeft = p1 - normal;
            nextQuadCoordsPtr->BottomLeft = prevQuadCoordsPtr->TopRight = p1 + normal;
            break;
        case Direction::RightUp:
            nextQuadCoordsPtr->BottomLeft = prevQuadCoordsPtr->TopRight = p1 + normal;
            nextQuadCoordsPtr->BottomRight = prevQuadCoordsPtr->BottomRight = p1 - normal;
            break;
        case Direction::UpLeft:
            nextQuadCoordsPtr->BottomRight = prevQuadCoordsPtr->TopLeft = p1 + normal;
            nextQuadCoordsPtr->TopRight = prevQuadCoordsPtr->TopRight = p1 - normal;
            break;
        case Direction::LeftUp:
            nextQuadCoordsPtr->BottomRight = prevQuadCoordsPtr->TopLeft = p1 + normal;
            nextQuadCoordsPtr->BottomLeft = prevQuadCoordsPtr->BottomLeft = p1 - normal;
            break;
        case Direction::DownLeft:
            nextQuadCoordsPtr->TopRight = prevQuadCoordsPtr->BottomLeft = p1 + normal;
            nextQuadCoordsPtr->BottomRight = prevQuadCoordsPtr->BottomRight = p1 - normal;
            break;
        case Direction::LeftDown:
            nextQuadCoordsPtr->TopLeft = prevQuadCoordsPtr->TopLeft = p1 - normal;
            nextQuadCoordsPtr->TopRight = prevQuadCoordsPtr->BottomLeft = p1 + normal;
            break;
        case Direction::DownRight:
            nextQuadCoordsPtr->TopLeft = prevQuadCoordsPtr->BottomRight = p1 + normal;
            nextQuadCoordsPtr->BottomLeft = prevQuadCoordsPtr->BottomLeft = p1 - normal;
            break;
        case Direction::RightDown:
            nextQuadCoordsPtr->TopRight = prevQuadCoordsPtr->TopRight = p1 - normal;
            nextQuadCoordsPtr->TopLeft = prevQuadCoordsPtr->BottomRight = p1 + normal;
            break;
        }
        if (!isLastPoint) {
            quadCoords.push_back(*nextQuadCoordsPtr);
        }
    }

    return quadCoords;
}
