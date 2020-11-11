#include "SimulatorApp.h"
#include "SceneMenu.h"
#include "AppScene.h"
#include "QuadRenderingTestScene.h"
#include "PhysicsBoxTestScene.h"
#include "WheelMotorTestScene.h"

SimulatorApp::SimulatorApp()
{
    m_sceneMenu = new SceneMenu(m_currentScene);
    m_sceneMenu->registerScene<QuadRenderingTestScene>("QuadTest");
    m_sceneMenu->registerScene<PhysicsBoxTestScene>("PhysicsBoxTest");
    m_sceneMenu->registerScene<WheelMotorTestScene>("WheelMotorTest");
}

SimulatorApp::~SimulatorApp()
{
    delete m_currentScene;
    delete m_sceneMenu;
}

void SimulatorApp::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_currentScene) {
        m_currentScene->onKeyEvent(keyEvent);
    }
}

void SimulatorApp::onUpdate()
{
    m_sceneMenu->render();
    if (m_currentScene) {
        m_currentScene->update();
    }
}

