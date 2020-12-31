#ifndef LINE_FOLLOWER_PATH_H_
#define LINE_FOLLOWER_PATH_H_

#include "SceneObject.h"
#include "Body2DUserData.h"
#include <glm/glm.hpp>
#include <vector>
#include "QuadCoords.h"

class QuadObject;

class LineFollowerPath : public SceneObject
{
public:
    typedef std::vector<glm::vec2> PathPoints;
    enum class Blueprint { Simple, Tshaped, Mshaped };
    static const PathPoints &getBlueprintPathPoints(LineFollowerPath::Blueprint blueprint);
    LineFollowerPath(Scene *scene, const glm::vec4 &lineColor, float lineWidth, const PathPoints &pathPoints);
    ~LineFollowerPath();

private:
    std::vector<QuadCoords> getRightAnglePathQuadCoords(const PathPoints &pathPoints, const float width);
    std::vector<std::unique_ptr<QuadObject>> m_pathQuads;
};

#endif /* LINE_FOLLOWER_PATH_H_ */
