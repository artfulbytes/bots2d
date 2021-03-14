#ifndef IMGUI_OVERLAY_H_
#define IMGUI_OVERLAY_H_

#include <string>

struct GLFWwindow;

/**
 * Wrapper around ImGui (lightweight GUI library).
 */
class ImGuiOverlay
{
public:
    static void init(GLFWwindow *window);
    static void destroy();

    static void render();
    static void newFrame();
    static void begin(std::string name, float x, float y);
    static void end();
    static bool button(std::string name);
    static void text(std::string text);
};

#endif /* IMGUI_OVERLAY_ */
