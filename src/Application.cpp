#include <SumobotTest.h>

#define GLFW_INCLUDE_NONE
/* TODO: Glad must be included before any openGL stuff */
#include "Renderer.h"
#include "ImGuiOverlay.h"
#include "GLError.h"
#include <GLFW/glfw3.h>

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestDrawBasicShapes.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


/* TODO: Is this the callback we can use in place of GLError.h? */
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void camera_handling(int key, int action)
{
    static glm::vec3 vec3;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_A) {
            vec3.x += 40;
        }
        if (key == GLFW_KEY_D) {
            vec3.x -= 40;
        }
        if (key == GLFW_KEY_W) {
            vec3.y -= 40;
        }
        if (key == GLFW_KEY_S) {
            vec3.y += 40;
        }
    }
    Renderer::setCamera(vec3);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << "Key callback" << key  << " " << action << std::endl;
    camera_handling(key, action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        GLCall(glfwSetWindowShouldClose(window, GLFW_TRUE));
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Renderer::setWindowSize(width, height);
    /* TODO: Move to renderer */
    glViewport(0, 0, width, height);
}

int init_opengl(GLFWwindow* window) {
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

    /* Synch FPS with vsync (FPS consistent with display) */
    glfwSwapInterval(1);

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

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return 0;
}


int main() {
    SumobotTest sumotest;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Sumobot simulator", NULL, NULL);
    if (init_opengl(window) == -1) {
        return -1;
    }
    assert(window != nullptr);

    ImGuiOverlay::init(window);
    Renderer::init();

    Test *currentTest = nullptr;
    TestMenu *testMenu = new TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<TestClearColor>("Clear Color");
    testMenu->RegisterTest<TestDrawBasicShapes>("Draw basic shapes");

    /* Render loop */
    while (!glfwWindowShouldClose(window))
    {
        Renderer::clear();
        ImGuiOverlay::newFrame();
        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGuiOverlay::begin("Test");
            if (currentTest != testMenu && ImGuiOverlay::button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGuiOverlay::end();
        }
        ImGuiOverlay::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Clean up */
    delete currentTest;
    if (currentTest != testMenu) {
        delete testMenu;
    }
    glfwDestroyWindow(window);
    ImGuiOverlay::destroy();
    glfwTerminate();
    return 0;
}


