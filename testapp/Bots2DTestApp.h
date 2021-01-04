#ifndef BOTS2D_TEST_APP_H_
#define BOTS2D_TEST_APP_H_

#include "Application.h"
#include <memory>

class Scene;
class SceneMenu;

/**
 * Test application with test scenes.
 */
class Bots2DTestApp : public Application
{
public:
    Bots2DTestApp();
    ~Bots2DTestApp();
    void onKeyEvent(const Event::Key &keyEvent) override;
    void onFixedUpdate(float stepTime) override;

private:
    Scene *m_currentScene = nullptr;
    std::unique_ptr<SceneMenu> m_sceneMenu;
};

#endif /* BOTS2D_TEST_APP_H_ */
