#ifndef NSUMO_SCENE_H
#define NSUMO_SCENE_H

#include "Scene.h"

class Dohyo;
class Sumobot;
class KeyboardController;
class NsumoMicrocontroller;

class NsumoScene : public Scene
{
public:
    NsumoScene();
    ~NsumoScene();
    virtual void onFixedUpdate() override;
private:
    std::unique_ptr<Dohyo> m_dohyo;
    std::unique_ptr<Sumobot> m_fourWheelBotOpponent;
    std::unique_ptr<KeyboardController> m_keyboardController;
    std::unique_ptr<Sumobot> m_fourWheelBot;
    std::unique_ptr<NsumoMicrocontroller> m_microcontroller;
};

#endif // NSUMO_SCENE_H
