#ifndef APP_SCENE_H_
#define APP_SCENE_H_

#include "Scene.h"
#include "PhysicsWorld.h"

class AppObject;
class AppScene
{
public:
    AppScene();
    ~AppScene();

    Scene *getScene() const { return m_scene; }
    void addObject(AppObject &appObject);
    void onKeyEvent(const Event::Key &keyEvent);
    void onFixedUpdate(double stepTime);

protected:
    Scene *m_scene = nullptr;

private:
    std::vector<AppObject *> m_appObjects;
};
#endif /* APP_SCENE_H_ */
