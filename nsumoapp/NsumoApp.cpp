#include "NsumoApp.h"
#include "SceneMenu.h"
#include "NsumoScene.h"

NsumoApp::NsumoApp() :
    Application("nsumoapp")
{
    m_sceneMenu->registerScene<NsumoScene>("Nsumo");
    m_sceneMenu->setCurrentScene("Nsumo");
}

NsumoApp::~NsumoApp()
{
}

void NsumoApp::onKeyEvent(const Event::Key &keyEvent)
{
    (void)keyEvent;
}

void NsumoApp::onFixedUpdate()
{
}
