#include "SceneMenu.h"
#include "ImGuiOverlay.h"

SceneMenu::SceneMenu(AppScene*& appScene) :
    m_currentScene(appScene)
{
}

void SceneMenu::render()
{
    ImGuiOverlay::begin("Scene menu");
    for (auto& scene : m_scenes)
    {
        if (ImGuiOverlay::button(scene.first.c_str())) {
            delete m_currentScene;
            m_currentScene = scene.second();
        }
    }
    ImGuiOverlay::end();
}
