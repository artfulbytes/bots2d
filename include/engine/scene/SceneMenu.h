#ifndef SCENE_MENU_H_
#define SCENE_MENU_H_

#include "Scene.h"
#include <string>
#include <functional>

class Application;

class SceneMenu
{
public:
    SceneMenu(Application *app, Scene*& scene);

    void render();

    template<typename T>
    void registerScene(const std::string& name)
    {
        m_scenes.push_back(std::make_pair(name, []() { return new T(); }));
    }
private:
    Scene*& m_currentScene;
    Application *m_appPtr;
    std::vector<std::pair<std::string, std::function<Scene*()>>> m_scenes;
};

#endif /* SCENE_MENU_H_ */
