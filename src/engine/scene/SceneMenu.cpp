#include "SceneMenu.h"
#include "ImGuiOverlay.h"
#include "Camera.h"

SceneMenu::SceneMenu(Scene*& scene) :
    m_currentScene(scene)
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
            Camera::reset();
        }
    }
    ImGuiOverlay::text("");
    if (m_currentScene != nullptr) {
        ImGuiOverlay::text(m_currentScene->getDescription().c_str());
        ImGuiOverlay::text("");
    }
    ImGuiOverlay::text("Move camera up     <w>");
    ImGuiOverlay::text("Move camera left   <a>");
    ImGuiOverlay::text("Move camera down   <s>");
    ImGuiOverlay::text("Move camera right  <d>");
    ImGuiOverlay::text("Zoom camera        <Scroll>");
    ImGuiOverlay::text("Reset camera       <r>");
    ImGuiOverlay::end();
}
