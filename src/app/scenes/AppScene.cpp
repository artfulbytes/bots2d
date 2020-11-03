#include "AppScene.h"
#include <iostream>

AppScene::AppScene() :
    m_scene(new Scene())
{
}

AppScene::~AppScene()
{
    delete m_scene;
}

void AppScene::update()
{
    m_scene->onUpdate();
}
