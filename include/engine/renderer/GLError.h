/* TODO: Use callback instead (OpenGl > ~4.4) */

#define GLCall(x) GLClearError();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__);

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


