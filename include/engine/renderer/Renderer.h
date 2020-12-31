#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>

class VertexArray;
class IndexBuffer;
class Shader;
class Texture;
struct TexCoords;
struct QuadCoords;

class Renderer
{
public:
    static void init();
    static void destroy();
    static void clear(const glm::vec4 &color);
    static void setViewport(int x, int y, int width, int height);
    static void setCameraPosition(const glm::vec2 &position, float zoomFactor);
    static float getPixelScaleFactor();
    static void drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
    static void drawRect(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
    static void drawRect(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Texture &texture,
                         const TexCoords *texCoords = nullptr);
    static void drawQuad(const QuadCoords &quadCoords, const glm::vec4 &color);
    static void drawQuad(glm::vec2 start, glm::vec2 end);
    static void drawCircle(const glm::vec2 &position, float radius, const glm::vec4 &color);
};

#endif /* RENDERER_H_ */
