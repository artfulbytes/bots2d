#include "tests/TestDrawBasicShapes.h"
#include "GLError.h"
#include "Renderer.h"

#include "imgui.h"

TestDrawBasicShapes::TestDrawBasicShapes()
{
}

TestDrawBasicShapes::~TestDrawBasicShapes()
{
}

void TestDrawBasicShapes::OnUpdate(float deltaTime)
{
}

void TestDrawBasicShapes::OnRender()
{
    GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    static float rotation = 0;
    const glm::vec4 colorCircleBorder(1.0f, 1.0f, 1.0f, 1.0f);
    Renderer::drawCircle(100.0f, glm::vec3(700.0f, 200.0f, 0.0), colorCircleBorder);
    const glm::vec4 colorCircle(0.0f, 0.0f, 0.0f, 1.0f);
    Renderer::drawCircle(90.0f, glm::vec3(700.0f, 200.0f, 0.0f), colorCircle);
    const glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    rotation += 0.01f;
    Renderer::drawQuad(glm::vec3(1000.0f, 900.0f, 0.0f), glm::vec2(310.0f, 10.0f), rotation, color);
    const glm::vec4 color1(0.25f, 0.75f, 1.0f, 1.0f);
    Renderer::drawQuad(glm::vec3(10.0f, 500.0f, 0.0f), glm::vec2(500.0f, 10.0f), rotation, color1);
    const glm::vec4 color2(0.5f, 0.5f, 0.5f, 1.0f);
    Renderer::drawQuad(glm::vec3(900.0f, 300.0f, 0.0f), glm::vec2(150.0f, 5.0f), -rotation, color2);
    const glm::vec4 color3(0.0f, 1.0f, 0.0f, 1.0f);
    Renderer::drawQuad(glm::vec3(300.0f, 300.0f, 0.0f), glm::vec2(310.0f, 40.0f), -rotation, color3);
    const glm::vec4 color4(0.0f, 1.0f, 1.0f, 1.0f);
    Renderer::drawQuad(glm::vec3(900.0f, 500.0f, 0.0f), glm::vec2(310.0f, 60.0f), rotation, color4);
    const glm::vec4 color5(1.0f, 1.0f, 0.3f, 1.0f);
    Renderer::drawQuad(glm::vec3(350.0f, 300.0f, 0.0f), glm::vec2(310.0f, 200.0f), rotation, color5);
    const glm::vec4 color6(1.0f, 1.0f, 0.6f, 0.3f);
    Renderer::drawQuad(glm::vec3(500.0f, 300.0f, 0.0f), glm::vec2(310.0f, 40.0f), -rotation, color6);
}

void TestDrawBasicShapes::OnImGuiRender()
{
    //ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 800.0f);
    //ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 800.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
