#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Event.h"
#include <string>
#include <memory>
#include "Scalebar.h"

struct GLFWwindow;

class Application
{
public:
    Application(std::string name);
    ~Application();
    void run();

    virtual void onKeyEvent(const Event::Key &keyEvent) = 0;
    virtual void onFixedUpdate(double stepTime) = 0;
    unsigned int getAvgFps() const;

private:
    GLFWwindow *m_window = nullptr;
    std::unique_ptr<Scalebar> m_scalebar = nullptr;
    float m_avgFps = 0.0f;
};

#endif /* APPLICATION_H_ */
