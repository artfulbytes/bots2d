#include "Application.h"
#define GLFW_INCLUDE_NONE
/* TODO: Glad must be included before any openGL stuff */
#include "Renderer.h"
#include "ImGuiOverlay.h"
#include "GLError.h"
#include "Event.h"
#include "Camera.h"

#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

/* TODO: Is this the callback we can use in place of GLError.h? */
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const Event::Key keyEvent = { Event::GLFWKeyToKeyCode(key),
                                  Event::GLFWActionToKeyAction(action) };
    if (keyEvent.code == Event::KeyCode::Unhandled || keyEvent.action == Event::KeyAction::Unhandled) {
        return;
    }

    if (keyEvent.code == Event::KeyCode::Escape) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (Camera::onKeyEvent(keyEvent)) {
        /* Camera swallowed it */
        return;
    }

    Application *app = (Application *)glfwGetWindowUserPointer(window);
    app->onKeyEvent(keyEvent);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
     Camera::onScrollEvent({ xoffset, yoffset });
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Camera::onWindowEvent({ width, height });
}

static void enable_vsync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}

static int init_opengl(GLFWwindow* window) {
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } else {
        std::cout << "Created GLFW window" << std::endl;
    }

    /* Set OpenGL version
     * Seems to not have an effect on my system. I need to export
     * MESA_GL_VERSION_OVERRIDE=3.3 for some reason
     * TODO: Make this choose core profile */
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    glfwMakeContextCurrent(window);

    /* Control rendering time yourself instead */
    enable_vsync(false);

    /* Initialize extension loader library (GLAD in this case)
     * GLAD (alternatively you can use GLEW or GL3W)
     * This is required to call functions beyond OpenGL 1.1
     * (i.e. to use the functions that are part of the GPU driver) */
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCall(glViewport(0, 0, 800, 600));

    /* TODO: Define callback function as lambda right here instead? */
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return 0;
}


Application::Application()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        assert(false);
    }

    /* TODO: Don't hard code name and size */
    m_window = glfwCreateWindow(800, 600, "Sumobot simulator", NULL, NULL);
    /* To make Application object accessible from C-callback */
    glfwSetWindowUserPointer(m_window, this);
    if (init_opengl(m_window) == -1) {
        assert(false);
    }
    assert(m_window != nullptr);

    ImGuiOverlay::init(m_window);
    Renderer::init();
}

Application::~Application()
{
    Renderer::destroy();
    ImGuiOverlay::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

namespace {
    const double framesPerSecond = 60.0;
    const double stepTime = 1.0 / framesPerSecond;
    double lastUpdateTime = 0;
}

/* TODO: Separate rendering freq from physics update freq. */
void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        const double currentTime = glfwGetTime();
        if ((currentTime - lastUpdateTime) < stepTime) {
            continue;
        }

        glfwPollEvents();
        Renderer::clear();
        ImGuiOverlay::newFrame();
        onFixedUpdate(stepTime);
        ImGuiOverlay::render();
        glfwSwapBuffers(m_window);
        lastUpdateTime = currentTime;
    }
}
