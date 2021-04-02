#include "Application.h"
#include "Renderer.h"
#include "Scene.h"
#include "ImGuiOverlay.h"
#include "GLError.h"
#include "Event.h"
#include "Camera.h"
#include "SceneMenu.h"

/* Glad must be included before any OpenGL stuff */
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

namespace {
    const glm::vec4 defaultBgColor(0.3f, 0.3f, 0.3f, 1.0f);
    const unsigned int sampleCount = 10;
    const int defaultWidth = 1280;
    const int defaultHeight = 960;
}

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
    app->onKeyCallback(keyEvent);
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
    m_sceneMenu = std::make_unique<SceneMenu>(m_currentScene);
}

Application::~Application()
{
    Renderer::destroy();
    m_scalebar = nullptr;
    ImGuiOverlay::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::onKeyCallback(const Event::Key &keyEvent)
{
    if (m_currentScene) {
        m_currentScene->onKeyEvent(keyEvent);
    }
    onKeyEvent(keyEvent);
}

void Application::onKeyEvent(const Event::Key &keyEvent)
{
    (void)keyEvent;
}

void Application::onFixedUpdate()
{
}

void Application::updatePhysics(float stepTime)
{
    if (m_currentScene) {
        m_currentScene->updatePhysics(stepTime);
    }
}

void Application::updateLogic(float stepTime)
{
    if (m_currentScene) {
        m_currentScene->onFixedUpdate();
        m_currentScene->updateControllers(stepTime);
        m_currentScene->sceneObjectsOnFixedUpdate();
    }
}

/**
 * If it takes a longer time to process the physics+logic update than what the
 * configured time step is, then the physics will be completely messed up because
 * we will lag behind. We should detect this and warn the user.
 */
bool Application::isStepTimeTooSmall() const
{
    /* Let average stabilize after a new scene */
    if (m_currentScene == nullptr && (m_currentScene->getSecondsSinceStart() < 2)) {
        return false;
    }

    const float expectedAvgPhysicsSteps = 1.0f / m_currentScene->getPhysicsStepTime();
    const float maxGap = 50.0f;
    return expectedAvgPhysicsSteps - m_avgPhysicsSteps > maxGap;
}

void Application::updateAndRenderSceneMenu()
{
    static unsigned int lastUpdateSeconds = 0;
    /* Avoid flickering updates */
    if (m_currentScene && m_currentScene->getSecondsSinceStart()) {
        const auto secondsNow = m_currentScene->getSecondsSinceStart();
        if (lastUpdateSeconds != secondsNow) {
            m_sceneMenu->setFps(m_fps);
            m_sceneMenu->setAvgPhysicsSteps(m_avgPhysicsSteps);
            if (isStepTimeTooSmall()) {
                m_sceneMenu->setWarningMessage("Physics step time too small!");
            } else if ((1 / m_currentScene->getPhysicsStepTime()) < 2 * m_fps) {
                m_sceneMenu->setWarningMessage("Reduce physics step time to avoid jittery rendering");
            } else {
                m_sceneMenu->setWarningMessage("None");
            }
            lastUpdateSeconds = secondsNow;
        }
    } else if (m_currentScene && m_currentScene->getSecondsSinceStart() == 0) {
        m_sceneMenu->setFps(0);
        m_sceneMenu->setAvgPhysicsSteps(0);
        m_sceneMenu->setWarningMessage("None");
    }
    m_sceneMenu->render();
}

void Application::render()
{
    Renderer::clear(defaultBgColor);
    ImGuiOverlay::newFrame();
    if (m_currentScene) {
        m_currentScene->render();
    }
    m_scalebar->render();
    updateAndRenderSceneMenu();
    ImGuiOverlay::render();
    glfwSwapBuffers(m_window);
}

/**
 * The simulator main loop. The physics/logic has a separate update rate from the
 * rendering. The physics/logic is determined by the physics step time (set inside
 * scene) while rendering is capped by VSync. Physics is updated by a fixed step
 * time as per the general recommendation. The technique used here is inspired by
 * the well-known blog post: https://gafferongames.com/post/fix_your_timestep/
 *
 * We do no interpolation for the rendering here, so the rendering will be jittery
 * if the physics update rate is close to the rendering rate. Make the physics update
 * rate at least twice as large to avoid this.
 */
void Application::run()
{
    double currentTime = glfwGetTime();
    double accumulator = 0.0;
    unsigned int stepsTaken = 0;
    unsigned int skipPhysicsUpdate = 5;
    unsigned int elapsedTime = 0;
    unsigned int lastElapsedTime = 0;
    while (!glfwWindowShouldClose(m_window))
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (stepsTaken > 0) {
            m_fps = 1.0f / frameTime;
            m_avgPhysicsSteps = m_avgPhysicsSteps + ((stepsTaken / frameTime) - m_avgPhysicsSteps) / sampleCount;
        }
        currentTime = newTime;

        /* The frame time spikes every time we change the scene. Since we use the frame time
         * to determine how many physics steps we take, it means that the number of physics
         * steps also spikes. To counter this, detect when the scene changes and skip updating
         * the physics for a couple of frames. */
        if (m_currentScene) {
            elapsedTime = m_currentScene->getMillisecondsSinceStart();
            bool changedScene = elapsedTime < lastElapsedTime || lastElapsedTime == 0;
            lastElapsedTime = elapsedTime;
            if (changedScene) {
                skipPhysicsUpdate = 5;
                accumulator = 0;
            }
        }

        stepsTaken = 0;
        if (m_currentScene != nullptr && !skipPhysicsUpdate) {
            while (accumulator >= m_currentScene->getPhysicsStepTime())
            {
                stepsTaken++;
                updatePhysics(m_currentScene->getPhysicsStepTime());
                updateLogic(m_currentScene->getPhysicsStepTime());
                accumulator -= m_currentScene->getPhysicsStepTime();
            }
            accumulator += frameTime;
        }
        if (skipPhysicsUpdate > 0) {
            skipPhysicsUpdate--;
        }
        glfwPollEvents();
        render();
    }
}
