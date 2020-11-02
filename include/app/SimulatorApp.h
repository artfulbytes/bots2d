#ifndef SIMULATOR_APP_H_
#define SIMULATOR_APP_H_

#include "Application.h"

class AppScene;
class SceneMenu;

class SimulatorApp : public Application
{
public:
    SimulatorApp();
    ~SimulatorApp();
    void onUpdate() override;

private:
    AppScene *m_currentScene = nullptr;
    SceneMenu *m_sceneMenu = nullptr;
};

#endif /* SIMULATOR_APP_H_ */
