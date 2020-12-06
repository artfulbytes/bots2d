#include "SimulatorApp.h"
#include "SceneMenu.h"
#include "AppScene.h"
#include "DrawTestScene.h"
#include "SpriteAnimationTestScene.h"
#include "PhysicsBoxTestScene.h"
#include "WheelMotorTestScene.h"
#include "Sumobot4WheelTestScene.h"

SimulatorApp::SimulatorApp()
{
    m_sceneMenu = new SceneMenu(m_currentScene);
    m_sceneMenu->registerScene<DrawTestScene>("DrawTest");
    m_sceneMenu->registerScene<SpriteAnimationTestScene>("SpriteAnimationTest");
    m_sceneMenu->registerScene<PhysicsBoxTestScene>("PhysicsBoxTest");
    m_sceneMenu->registerScene<WheelMotorTestScene>("WheelMotorTest");
    m_sceneMenu->registerScene<Sumobot4WheelTestScene>("Sumobot4WheelTest");
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

void SimulatorApp::onFixedUpdate(double stepTime)
{
    m_sceneMenu->render();
    if (m_currentScene) {
        m_currentScene->onFixedUpdate(stepTime);
    }
}

