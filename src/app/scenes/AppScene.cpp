#include "AppScene.h"
#include "AppObject.h"
#include <iostream>

AppScene::AppScene() :
    m_scene(new Scene())
{
}

AppScene::~AppScene()
{
    for (auto obj : m_appObjects) {
        delete obj;
    }
    delete m_scene;
}

void AppScene::addObject(AppObject &appObject) {
    m_appObjects.push_back(&appObject);
}

void AppScene::update()
{
    for (auto obj : m_appObjects) {
        /* Only call update on objects without parent (the parent should update its child objects) */
        if (nullptr == obj->getParent()) {
            obj->update();
        }
    }
    m_scene->onUpdate();
}
