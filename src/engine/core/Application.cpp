#include "Application.h"
#include "Renderer.h"
#include "ImGuiOverlay.h"
#include "GLError.h"
#include "Event.h"
#include "Camera.h"

/* Glad must be included before any OpenGL stuff */
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %d %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)window;
    (void)scancode;
    (void)mods;
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
     (void)window;
     Camera::onScrollEvent({ xoffset, yoffset });
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    Camera::onWindowEvent({ width, height });
}

static void enable_vsync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}

static const int defaultWidth = 1280;
static const int defaultHeight = 960;

static int init_opengl(GLFWwindow* window) {
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /* Sync rendering with the monitor's frame rate */
    enable_vsync(true);

    /* Initialize extension loader library (GLAD in this case)
     * This is required to call functions beyond OpenGL 1.1
     * (i.e. to use the functions that are part of the GPU driver) */
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCall(glViewport(0, 0, defaultWidth, defaultHeight));

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return 0;
}


Application::Application(std::string name)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        assert(false);
    }

    /* Set OpenGL version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Enable anti aliasing */
    glfwWindowHint(GLFW_SAMPLES, 16);

    /* NOTE: glfwWindowHint-calls must come before glfwCreateWindow */
    m_window = glfwCreateWindow(defaultWidth, defaultHeight, name.c_str(), NULL, NULL);

    /* Makes Application object accessible from C-callback */
    glfwSetWindowUserPointer(m_window, this);
    if (init_opengl(m_window) == -1) {
        assert(false);
    }
    assert(m_window != nullptr);

    ImGuiOverlay::init(m_window);
    Renderer::init();
    m_scalebar = std::make_unique<Scalebar>();
}

Application::~Application()
{
    Renderer::destroy();
    m_scalebar = nullptr;
    ImGuiOverlay::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

namespace {
    const glm::vec4 defaultBgColor(0.3f, 0.3f, 0.3f, 1.0f);
    const unsigned int fpsSampleCount = 10;
}

unsigned int Application::getAvgFps() const
{
    return static_cast<unsigned int>(m_avgFps);
}

void Application::run()
{
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        Renderer::clear(defaultBgColor);
        ImGuiOverlay::newFrame();
        const float stepTime = 1.0 / m_avgFps;
        onFixedUpdate(stepTime);
        ImGuiOverlay::render();
        m_scalebar->render();
        glfwSwapBuffers(m_window);

        const float currentTime = glfwGetTime();
        const float newFps = 1.0f / (currentTime - lastTime);
        m_avgFps = m_avgFps + (newFps - m_avgFps) / fpsSampleCount;
        lastTime = currentTime;
    }
}
