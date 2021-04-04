#ifndef SUMOBOT_TEST_SCENE_H_
#define SUMOBOT_TEST_SCENE_H_

#include "Scene.h"

class RectObject;
class Dohyo;
class Sumobot;
class NsumoMicrocontroller;
class KeyboardController;

class SumobotTestScene : public Scene
{
public:
    SumobotTestScene();
    ~SumobotTestScene();
    virtual void onFixedUpdate() override;

private:
    struct Background {
        std::unique_ptr<RectObject> leftSide;
        std::unique_ptr<RectObject> middleStripe;
        std::unique_ptr<RectObject> rightSide;
    };
    void createBackground();
    void createTuningMenu();
    std::unique_ptr<ImGuiMenu> m_tuningMenu;
    std::unique_ptr<Background> m_background = std::make_unique<Background>();
    std::unique_ptr<Dohyo> m_dohyo;
    std::unique_ptr<Sumobot> m_fourWheelBot;
    std::unique_ptr<Sumobot> m_fourWheelBotOpponent;
    std::unique_ptr<Sumobot> m_twoWheelRectangleBot;
    std::unique_ptr<Sumobot> m_twoWheelRoundBlackBot;
    std::unique_ptr<Sumobot> m_twoWheelRoundRedBot;
    std::unique_ptr<NsumoMicrocontroller> m_microcontroller;
    std::unique_ptr<KeyboardController> m_keyboardController;
};

#endif /* SUMOBOT_TEST_SCENE_H_ */
