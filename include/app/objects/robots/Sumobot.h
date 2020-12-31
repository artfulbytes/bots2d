#ifndef SUMOBOT_H_
#define SUMOBOT_H_

#include "robots/BaseBot.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class Sumobot : public BaseBot
{
public:
    enum class Blueprint { FourWheel, TwoWheelRectangle, TwoWheelRoundBlack, TwoWheelRoundRed };
    static const Specification &getBlueprintSpec(Blueprint blueprint);

    Sumobot(Scene *scene, const Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation);
    ~Sumobot();
};

#endif /* SUMOBOT_H_ */
