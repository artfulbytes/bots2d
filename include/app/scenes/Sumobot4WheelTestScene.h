#ifndef SUMOBOT_4_WHEEL_TEST_SCENE_H_
#define SUMOBOT_4_WHEEL_TEST_SCENE_H_

#include "Scene.h"

#include "playgrounds/Dohyo.h"
#include "shapes/QuadObject.h"


class QuadObject;
class Dohyo;
class Sumobot4Wheel;

class Sumobot4WheelTestScene : public Scene
{
public:
    Sumobot4WheelTestScene();
    ~Sumobot4WheelTestScene();

private:
    struct Background {
        std::unique_ptr<QuadObject> leftSide;
        std::unique_ptr<QuadObject> rightSide;
    };
    void createBackground();
    std::unique_ptr<Background> m_background = std::make_unique<Background>();
    std::unique_ptr<Dohyo> m_dohyo;
    std::unique_ptr<Sumobot4Wheel> m_sumobot4WheelPlated;
    std::unique_ptr<Sumobot4Wheel> m_sumobot4WheelCircuited;
    std::unique_ptr<ControllerComponent> m_platedController;
    std::unique_ptr<ControllerComponent> m_circuitedController;
};

#endif /* SUMOBOT_4_WHEEL_TEST_SCENE_H_ */
