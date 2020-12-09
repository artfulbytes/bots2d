#include "AppScene.h"
#include "AppObject.h"

AppScene::AppScene() :
    m_scene(new Scene())
{
}

AppScene::~AppScene()
{
    for (AppObject *obj : m_appObjects) {
        delete obj;
    }
    delete m_scene;
}

void AppScene::addObject(AppObject &appObject) {
    m_appObjects.push_back(&appObject);
}

void AppScene::onKeyEvent(const Event::Key &keyEvent)
{
    m_scene->onKeyEvent(keyEvent);
}

void AppScene::onFixedUpdate(double stepTime)
{
    for (auto obj : m_appObjects) {
        /* Only call update on objects without parent (the parent should update its child objects) */
        if (nullptr == obj->getParent()) {
            obj->onFixedUpdate(stepTime);
        }
    }
    m_scene->onFixedUpdate(stepTime);
}
