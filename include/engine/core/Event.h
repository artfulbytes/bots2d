#ifndef EVENT_H_
#define EVENT_H_

class Event
{
public:
    struct Window
    {
        int width;
        int height;
    };

    struct Scroll
    {
        double xoffset;
        double yoffset;
    };

    /* Create wrappers around GLFW */
    enum class KeyCode
    {
        A,
        S,
        D,
        W,
        Escape,
        Space,
        Up,
        Down,
        Unhandled
    };
    enum class KeyAction
    {
        Release,
        Press,
        Repeat,
        Unhandled
    };
    struct Key
    {
        KeyCode code;
        KeyAction action;
    };

    static KeyCode GLFWKeyToKeyCode(int GLFWAction);
    static KeyAction GLFWActionToKeyAction(int GLFWKey);
};

#endif /* EVENT_H_ */
