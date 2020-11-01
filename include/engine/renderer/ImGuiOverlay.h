#ifndef IMGUI_OVERLAY_H_
#define IMGUI_OVERLAY_H_

#include <string>

class GLFWwindow;

class ImGuiOverlay
{
public:
    static void init(GLFWwindow *window);
    static void destroy();

    static void render();
    static void newFrame();
    static bool button(const char *name);
    static void begin(const char *name);
    static void end();
};

#endif /* IMGUI_OVERLAY_ */
