#include "SimulatorTestApp.h"
#include "SceneMenu.h"
#include "DrawTestScene.h"
#include "SpriteAnimationTestScene.h"
#include "PhysicsTestScene.h"
#include "WheelMotorTestScene.h"
#include "SumobotTestScene.h"
#include "LineFollowerTestScene.h"

SimulatorTestApp::SimulatorTestApp() :
    Application("bots2dtest")
{
    m_sceneMenu = std::make_unique<SceneMenu>(this, m_currentScene);
    m_sceneMenu->registerScene<DrawTestScene>("DrawTest");
    m_sceneMenu->registerScene<SpriteAnimationTestScene>("SpriteAnimationTest");
    m_sceneMenu->registerScene<PhysicsTestScene>("PhysicsTest");
    m_sceneMenu->registerScene<WheelMotorTestScene>("WheelMotorTest");
    m_sceneMenu->registerScene<SumobotTestScene>("SumobotTest");
    m_sceneMenu->registerScene<LineFollowerTestScene>("LineFollowerTest");
}

SimulatorTestApp::~SimulatorTestApp()
{
    delete m_currentScene;
}

void SimulatorTestApp::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_currentScene) {
        m_currentScene->onKeyEvent(keyEvent);
    }
}

void SimulatorTestApp::onFixedUpdate(float stepTime)
{
    m_sceneMenu->render();
    if (m_currentScene) {
        m_currentScene->onFixedUpdate(stepTime);
    }
}

