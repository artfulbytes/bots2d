#ifndef SUMOBOT_TEST_SCENE_H_
#define SUMOBOT_TEST_SCENE_H_

#include "Scene.h"

class RectObject;
class Dohyo;
class Sumobot;
class MicrocontrollerSumobot4WheelExample;
class KeyboardController;

class SumobotTestScene : public Scene
{
public:
    SumobotTestScene();
    ~SumobotTestScene();

private:
    struct Background {
        std::unique_ptr<RectObject> leftSide;
        std::unique_ptr<RectObject> middleStripe;
        std::unique_ptr<RectObject> rightSide;
    };
    void createBackground();
    std::unique_ptr<Background> m_background = std::make_unique<Background>();
    std::unique_ptr<Dohyo> m_dohyo;
    std::unique_ptr<Sumobot> m_fourWheelBot;
    std::unique_ptr<Sumobot> m_twoWheelRectangleBot;
    std::unique_ptr<Sumobot> m_twoWheelRoundBlackBot;
    std::unique_ptr<Sumobot> m_twoWheelRoundRedBot;
    std::unique_ptr<MicrocontrollerSumobot4WheelExample> m_microcontroller;
    std::unique_ptr<KeyboardController> m_keyboardController;
};

#endif /* SUMOBOT_TEST_SCENE_H_ */
