#ifndef SCENE_MENU_H_
#define SCENE_MENU_H_

#include "AppScene.h"
#include <string>
#include <functional>

class SceneMenu
{
public:
    SceneMenu(AppScene*& appScene);

    void render();

    template<typename T>
    void registerScene(const std::string& name)
    {
        m_scenes.push_back(std::make_pair(name, []() { return new T(); }));
    }
private:
    AppScene*& m_currentScene;

    std::vector<std::pair<std::string, std::function<AppScene*()>>> m_scenes;
};

#endif /* SCENE_MENU_H_ */
