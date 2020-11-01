#ifndef APPLICATION_H_
#define APPLICATION_H_

class GLFWwindow;

class Application
{
public:
    Application();
    ~Application();
    void run();

    virtual void onUpdate() = 0;

private:
    GLFWwindow *m_window;
};

#endif /* APPLICATION_H_ */
