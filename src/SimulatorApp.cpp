#include "SimulatorApp.h"

#include "ImGuiOverlay.h"
#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestDrawBasicShapes.h"

SimulatorApp::SimulatorApp()
{
    m_testMenu = new TestMenu(m_currentTest);
    m_currentTest = m_testMenu;

    m_testMenu->RegisterTest<TestClearColor>("Clear Color");
    m_testMenu->RegisterTest<TestDrawBasicShapes>("Draw basic shapes");
}

SimulatorApp::~SimulatorApp()
{
    delete m_currentTest;
    if (m_currentTest != m_testMenu) {
        delete m_testMenu;
    }
}

void SimulatorApp::onUpdate() {
    if (m_currentTest)
    {
        m_currentTest->OnUpdate(0.0f);
        m_currentTest->OnRender();
        ImGuiOverlay::begin("Test");
        if (m_currentTest != m_testMenu && ImGuiOverlay::button("<-"))
        {
            delete m_currentTest;
            m_currentTest = m_testMenu;
        }
        m_currentTest->OnImGuiRender();
        ImGuiOverlay::end();
    }
}

