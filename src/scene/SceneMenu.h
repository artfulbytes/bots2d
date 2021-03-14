#ifndef SCENE_MENU_H_
#define SCENE_MENU_H_

#include "Scene.h"
#include <string>
#include <functional>

class Application;

/**
 * A GUI sidebar menu that provides a list of selectable scenes and keeps
 * track of the current Scene. It also displays useful simulation info and statistics.
 */
class SceneMenu
{
public:
    SceneMenu(Scene*& scene);
    ~SceneMenu();

    void render();

    template<typename T>
    void registerScene(const std::string& name)
    {
        m_scenes.push_back(std::make_pair(name, []() { return new T(); }));
    }
    void setFps(unsigned int fps);
    void setAvgPhysicsSteps(unsigned int avgPhysicsSteps);
    void setWarningMessage(std::string message);
private:
    Scene*& m_currentScene;
    std::vector<std::pair<std::string, std::function<Scene*()>>> m_scenes;
    unsigned int m_fps = 0;
    unsigned int m_avgPhysicsSteps = 0;
    std::string m_warningMessage;
};

#endif /* SCENE_MENU_H_ */
