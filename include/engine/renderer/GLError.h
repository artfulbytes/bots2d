/**
 * Declares a macro for OpenGL calls to check for errors and report the
 * the failing line. This is a remnant from earlier OpenGL versions. In
 * later OpenGL a callback can be used instead.
 *
 * TODO: Replace with callback
 */

#define GLCall(x) GLClearError();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__);

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


