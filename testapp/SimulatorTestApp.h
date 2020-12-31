#ifndef SIMULATOR_TEST_APP_H_
#define SIMULATOR_TEST_APP_H_

#include "Application.h"
#include <memory>

class Scene;
class SceneMenu;

/**
 * Test application with test scenes.
 */
class SimulatorTestApp : public Application
{
public:
    SimulatorTestApp();
    ~SimulatorTestApp();
    void onKeyEvent(const Event::Key &keyEvent) override;
    void onFixedUpdate(double stepTime) override;

private:
    Scene *m_currentScene = nullptr;
    std::unique_ptr<SceneMenu> m_sceneMenu;
};

#endif /* SIMULATOR_TEST_APP_H_ */
