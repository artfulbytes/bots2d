#ifndef RENDERER_H_
#define RENDERER_H_
/* TODO: Should we include GL some other way? */
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VertexArray;
class IndexBuffer;
class Shader;

/* TODO: Make it singleton? */
class Renderer
{
public:
    static void init();
    static void destroy();

    static void clear();
    static void setWindowSize(int width, int height);
    static void setCamera(const glm::vec3 camera);
    static void drawLine(const glm::vec2& start, const glm::vec2& end, float width, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void drawCircle(float radius, const glm::vec3& position, const glm::vec4& color);
};

#endif /* RENDERER_H_ */
