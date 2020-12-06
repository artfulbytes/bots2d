#ifndef RENDERER_H_
#define RENDERER_H_
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VertexArray;
class IndexBuffer;
class Shader;
class Texture;
class TexCoords;

class Renderer
{
public:
    static void init();
    static void destroy();
    static void clear(const glm::vec4 &color);
    static void setViewport(int x, int y, int width, int height);
    static void setCameraPosition(const glm::vec3 &position, float zoomFactor);
    static void drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
    static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
    static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Texture &texture, const TexCoords *texCoords);
    static void drawCircle(const glm::vec3 &position, float radius, const glm::vec4 &color);
};

#endif /* RENDERER_H_ */
