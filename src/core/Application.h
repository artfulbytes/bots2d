#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Event.h"
#include <string>
#include <memory>

#include "Scalebar.h"

class Scene;
class SceneMenu;
struct GLFWwindow;

/**
 * Application base class. Sets up OpenGL and GLFW (window and event handling),
 * and implements the main loop.
 *
 * You inherit this when you create your own Application, look at SimulatorTestApp for
 * an example.
 */
class Application
{
public:
    Application(std::string name);
    virtual ~Application();
    /** Starts the main loop */
    void run();
    void onKeyCallback(const Event::Key &keyEvent);
    virtual void onKeyEvent(const Event::Key &keyEvent);

    /** Runs after every physics step */
    virtual void onFixedUpdate();

protected:
    std::unique_ptr<SceneMenu> m_sceneMenu;
    void setCurrentScene();

private:
    bool isStepTimeTooSmall() const;
    void updatePhysics(float stepTime);
    void updateLogic(float stepTime);
    void updateAndRenderSceneMenu();
    void render();

    GLFWwindow *m_window = nullptr;
    std::unique_ptr<Scalebar> m_scalebar = nullptr;
    float m_fps = 0.0f;
    float m_avgPhysicsSteps = 0.0f;
    Scene *m_currentScene = nullptr;
};

#endif /* APPLICATION_H_ */
