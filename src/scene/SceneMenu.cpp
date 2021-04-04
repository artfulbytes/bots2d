#include "SceneMenu.h"
#include "ImGuiOverlay.h"
#include "Camera.h"
#include "Application.h"

SceneMenu::SceneMenu(Scene*& scene) :
    m_currentScene(scene)
{
}

SceneMenu::~SceneMenu()
{
    if (m_currentScene) {
        delete m_currentScene;
    }
}

void SceneMenu::setFps(unsigned int fps)
{
    m_fps = fps;
}

void SceneMenu::setAvgPhysicsSteps(unsigned int avgPhysicsSteps)
{
    m_avgPhysicsSteps = avgPhysicsSteps;
}

void SceneMenu::setWarningMessage(std::string message)
{
    m_warningMessage = message;
}

void SceneMenu::setCurrentScene(std::string sceneName)
{
    for (auto &scene : m_scenes) {
        if (scene.first == sceneName) {
            m_currentScene = scene.second();
            Camera::reset();
        }
    }
}

void SceneMenu::render()
{
    ImGuiOverlay::begin("Scene menu", 15.0f, 15.0f, 230.0f, 450.0f);
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
        ImGuiOverlay::text("Scene: " + m_currentScene->getDescription());
    }
    if (!m_warningMessage.empty())
    {
        ImGuiOverlay::text("Warning: " + m_warningMessage);
    }
    ImGuiOverlay::text("FPS: " + std::to_string(m_fps) + " Hz");
    if (m_avgPhysicsSteps)
    {
        ImGuiOverlay::text("Physics step rate: " + std::to_string(m_avgPhysicsSteps) + " Hz");
    } else {
        ImGuiOverlay::text("Physics step rate: ");
    }
    ImGuiOverlay::text("");
    ImGuiOverlay::text("Move camera up     <w>");
    ImGuiOverlay::text("Move camera left   <a>");
    ImGuiOverlay::text("Move camera down   <s>");
    ImGuiOverlay::text("Move camera right  <d>");
    ImGuiOverlay::text("Zoom camera        <Scroll>");
    ImGuiOverlay::text("Reset camera       <r>");
    ImGuiOverlay::end();
}
