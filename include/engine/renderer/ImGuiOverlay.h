#ifndef IMGUI_OVERLAY_H_
#define IMGUI_OVERLAY_H_

#include <string>

struct GLFWwindow;

class ImGuiOverlay
{
public:
    static void init(GLFWwindow *window);
    static void destroy();

    static void render();
    static void newFrame();
    static void begin(const char *name);
    static void end();
    static bool button(const char *name);
    static void text(const char *text);
};

#endif /* IMGUI_OVERLAY_ */
