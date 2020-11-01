#include <glad/gl.h>
#include <iostream>
#include <cassert>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << file << " " << line << " " << function << " [OpenGL errer]: " << error << std::endl;
        assert(false);
    }
    return true;
}
