#include "SceneMenu.h"
#include "ImGuiOverlay.h"
#include "Camera.h"
#include "Application.h"

SceneMenu::SceneMenu(Application *app, Scene*& scene) :
    m_appPtr(app),
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
    if (m_appPtr != nullptr)
    {
        ImGuiOverlay::text("");
        ImGuiOverlay::text("  " + std::to_string(m_appPtr->getAvgFps()) + " fps");
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
