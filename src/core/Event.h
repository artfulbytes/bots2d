#ifndef EVENT_H_
#define EVENT_H_

/**
 * Wrapper class around GLFW window, key and scroll events.
 */
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
        R,
        Escape,
        Space,
        Up,
        Down,
        Left,
        Right,
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
