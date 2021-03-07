#include "Bots2DTestApp.h"
#include "SceneMenu.h"
#include "DrawTestScene.h"
#include "SpriteAnimationTestScene.h"
#include "PhysicsTestScene.h"
#include "WheelMotorTestScene.h"
#include "PhysicsBotTestScene.h"
#include "SumobotTestScene.h"
#include "LineFollowerTestScene.h"

Bots2DTestApp::Bots2DTestApp() :
    Application("bots2dtest")
{
    m_sceneMenu = std::make_unique<SceneMenu>(this, m_currentScene);
    m_sceneMenu->registerScene<DrawTestScene>("DrawTest");
    m_sceneMenu->registerScene<SpriteAnimationTestScene>("SpriteAnimationTest");
    m_sceneMenu->registerScene<PhysicsTestScene>("PhysicsTest");
    m_sceneMenu->registerScene<WheelMotorTestScene>("WheelMotorTest");
    m_sceneMenu->registerScene<PhysicsBotTestScene>("PhysicsBotTest");
    m_sceneMenu->registerScene<SumobotTestScene>("SumobotTest");
    m_sceneMenu->registerScene<LineFollowerTestScene>("LineFollowerTest");
}

Bots2DTestApp::~Bots2DTestApp()
{
    delete m_currentScene;
}

void Bots2DTestApp::onKeyEvent(const Event::Key &keyEvent)
{
    if (m_currentScene) {
        m_currentScene->onKeyEvent(keyEvent);
    }
}

void Bots2DTestApp::onFixedUpdate(float stepTime)
{
    m_sceneMenu->render();
    if (m_currentScene) {
        m_currentScene->onFixedUpdate(stepTime);
    }
}
