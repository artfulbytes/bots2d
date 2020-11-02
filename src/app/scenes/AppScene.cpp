#include "AppScene.h"
#include <iostream>

AppScene::AppScene()
{
    /* TODO: There might be cases where we don't ahve a scene? */
    m_scene = new Scene();
}

AppScene::~AppScene()
{
    delete m_scene;
}

void AppScene::update()
{
    m_scene->onUpdate();
}
