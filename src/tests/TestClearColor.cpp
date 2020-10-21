#include "tests/TestClearColor.h"
#include <glad/gl.h>
#include "GLError.h"
#include "imgui.h"

TestClearColor::TestClearColor() :
    m_clearColor { 0.2f, 0.3f, 0.8f, 1.0f }
{

}
TestClearColor::~TestClearColor()
{
}

void TestClearColor::OnUpdate(float deltaTime)
{
}

void TestClearColor::OnRender()
{
    /* TODO: Use renderer here instead */
    GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::OnImGuiRender()
{
    ImGui::ColorEdit4("Clear Color", m_clearColor);
}

