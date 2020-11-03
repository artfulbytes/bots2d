#include "SimulatorApp.h"
#include "SceneMenu.h"
#include "AppScene.h"
#include "QuadRenderingTestScene.h"
#include "PhysicsBoxTestScene.h"

SimulatorApp::SimulatorApp()
{
    m_sceneMenu = new SceneMenu(m_currentScene);
    m_sceneMenu->registerScene<QuadRenderingTestScene>("QuadTest");
    m_sceneMenu->registerScene<PhysicsBoxTestScene>("PhysicsBoxTest");
}

SimulatorApp::~SimulatorApp()
{
    delete m_currentScene;
    delete m_sceneMenu;
}

void SimulatorApp::onUpdate() {
    m_sceneMenu->render();
    if (m_currentScene)
    {
        m_currentScene->update();
    }
}

