#ifndef LINE_FOLLOWER_H_
#define LINE_FOLLOWER_H_

#include "robots/BaseBot.h"

#include <glm/glm.hpp>

/**
 * LineFollower - a robot that attempts to follow a path laid out as a black
 * (or sometimes white) stripe.
 *
 * The class provides pre-defined LineFollower specs as Blueprints. The class is flexible, and you
 * can create a new Blueprint/Specification if you want to better resemble your own LineFollower.
 * See BaseBot for a list of specification parameters.
 */
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
