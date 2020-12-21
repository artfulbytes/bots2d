#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Event.h"
#include <string>

class GLFWwindow;

class Application
{
public:
    Application(std::string name);
    ~Application();
    void run();

    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onFixedUpdate(double stepTime) = 0;

private:
    GLFWwindow *m_window;
};

#endif /* APPLICATION_H_ */
