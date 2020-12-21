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

void ImGuiOverlay::begin(const char *name)
{
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::Begin(name);
}

void ImGuiOverlay::end()
{
    ImGui::End();
}

bool ImGuiOverlay::button(const char *name)
{
    return ImGui::Button(name);
}
