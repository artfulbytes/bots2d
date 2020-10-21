#include "tests/Test.h"

#include <iostream>
#include "imgui.h"

TestMenu::TestMenu(Test*& currentTestPointer) :
    m_currentTest(currentTestPointer)
{
}

void TestMenu::OnImGuiRender()
{
    for (auto& test : m_tests)
    {
        if (ImGui::Button(test.first.c_str())) {
            m_currentTest = test.second();
        }
    }
}
