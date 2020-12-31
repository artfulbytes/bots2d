#ifndef LINE_FOLLOWER_H_
#define LINE_FOLLOWER_H_

#include "robots/BaseBot.h"

#include <glm/glm.hpp>

class LineFollower : public BaseBot
{
public:
    enum class Blueprint { FourFrontSensors };
    static const Specification &getBlueprintSpec(Blueprint blueprint);

    LineFollower(Scene *scene, const Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation);
    ~LineFollower();
};

#endif /* LINE_FOLLOWER_H_ */
