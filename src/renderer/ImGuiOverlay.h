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
    static void begin(std::string name, float x, float y, float width, float height);
    static void end();
    static bool button(std::string name);
    static void text(std::string text);
    static void checkbox(std::string name, bool *set);
    static void sliderFloat(std::string name, float *value, float min, float max);
};

#endif /* IMGUI_OVERLAY_ */
