#ifndef APP_SCENE_H_
#define APP_SCENE_H_

#include "Scene.h"
#include <string>

class AppScene
{
public:
    AppScene();
    ~AppScene();

    void update();
protected:
    Scene *m_scene;
};
#endif /* APP_SCENE_H_ */
