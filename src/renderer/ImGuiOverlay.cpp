#include "ImGuiOverlay.h"
#include <GLFW/glfw3.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

void ImGuiOverlay::init(GLFWwindow *window)
{
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Copied from example_glfw_opengl3 */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiOverlay::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiOverlay::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiOverlay::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiOverlay::begin(std::string name, float x, float y, float width, float height)
{
    ImGui::SetNextWindowPos({x, y});
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Once);
    ImGui::Begin(name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
}

void ImGuiOverlay::end()
{
    ImGui::End();
}

bool ImGuiOverlay::button(std::string name)
{
    return ImGui::Button(name.c_str());
}

void ImGuiOverlay::text(std::string text)
{
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 230);
    ImGui::Text("%s", text.c_str());
}

void ImGuiOverlay::checkbox(std::string name, bool *set)
{
    ImGui::Checkbox(name.c_str(), set);
}

void ImGuiOverlay::sliderFloat(std::string name, float *value, float min, float max)
{
    ImGui::SliderFloat(name.c_str(), value, min, max);
}

/* TODO: Colored text: ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff"); */
