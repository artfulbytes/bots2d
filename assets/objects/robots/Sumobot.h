#ifndef SUMOBOT_H_
#define SUMOBOT_H_

#include "robots/BaseBot.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

/**
 * Sumobot: a robot that attempts to push another robot out of a circle (Dohyo).
 * Sumobot competition is a popular type of robotics competition.
 *
 * The class provides several pre-defined sumobots as Blueprints. The class is flexible, and you
 * can create a new Blueprint/Specification if you want to resemble your own Sumobot better.
 * See BaseBot for a list of specification parameters.
 */
class Sumobot : public BaseBot
{
public:
    enum class Blueprint { Nsumo, FourWheel, TwoWheelRectangle, TwoWheelRoundBlack, TwoWheelRoundRed };
    static const Specification &getBlueprintSpec(Blueprint blueprint);

    Sumobot(Scene *scene, const Specification &spec,
                 const glm::vec2 &startPosition, const float startRotation);
    ~Sumobot();
};

#endif /* SUMOBOT_H_ */
