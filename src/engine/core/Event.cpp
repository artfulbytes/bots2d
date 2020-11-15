#include "Event.h"

#include "GLFW/glfw3.h"

Event::KeyCode Event::GLFWKeyToKeyCode(int GLFWKey) {
    switch (GLFWKey)
    {
        case GLFW_KEY_A: return Event::KeyCode::A;
        case GLFW_KEY_S: return Event::KeyCode::S;
        case GLFW_KEY_D: return Event::KeyCode::D;
        case GLFW_KEY_W: return Event::KeyCode::W;
        case GLFW_KEY_UP: return Event::KeyCode::Up;
        case GLFW_KEY_DOWN: return Event::KeyCode::Down;
        case GLFW_KEY_LEFT: return Event::KeyCode::Left;
        case GLFW_KEY_RIGHT: return Event::KeyCode::Right;
        case GLFW_KEY_SPACE: return Event::KeyCode::Space;
        case GLFW_KEY_ESCAPE: return Event::KeyCode::Escape;
    }
    return Event::KeyCode::Unhandled;
}

Event::KeyAction Event::GLFWActionToKeyAction(int GLFWAction) {
    switch (GLFWAction)
    {
        case GLFW_RELEASE: return Event::KeyAction::Release;
        case GLFW_PRESS:   return Event::KeyAction::Press;
        case GLFW_REPEAT:  return Event::KeyAction::Repeat;
    }
    return Event::KeyAction::Unhandled;
}
