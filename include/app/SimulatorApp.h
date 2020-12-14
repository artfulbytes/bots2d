#ifndef SIMULATOR_APP_H_
#define SIMULATOR_APP_H_

#include "Application.h"
#include <memory>

class Scene;
class SceneMenu;

class SimulatorApp : public Application
{
public:
    SimulatorApp();
    ~SimulatorApp();
    void onKeyEvent(const Event::Key &keyEvent) override;
    void onFixedUpdate(double stepTime) override;

private:
    Scene *m_currentScene = nullptr;
    std::unique_ptr<SceneMenu> m_sceneMenu;
};

#endif /* SIMULATOR_APP_H_ */
