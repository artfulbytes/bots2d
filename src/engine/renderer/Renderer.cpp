#include "Renderer.h"
#include "GLError.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include <iostream>
#include <cassert>
#include <memory>

/* TODO: Use smart pointer so delete removes inner objects too */
struct RendererStorage
{
    VertexBuffer* quadVertexBuffer;
    VertexArray* quadVertexArray;
    IndexBuffer* quadIndexBuffer;

    VertexBuffer* circleVertexBuffer;
    VertexArray* circleVertexArray;
    IndexBuffer* circleIndexBuffer;

    Shader* solidColorShader;
    glm::mat4* projectionMatrix;
    glm::mat4* viewMatrix;
    /* TODO: Make this depend on lengthScaleFactor */
    float centimetersToPxScale = 10.0f;
};

static RendererStorage* s_rendererData;

static void initCircle()
{
    const float radius = 0.5f;
    const int cornerCount = 60;
    float circleVertices[cornerCount * 2];

    /* Sin and cos are expensive, but we only run this function once */
	for(int i = 0; i < cornerCount; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(cornerCount);
        circleVertices[2 * i] = radius * glm::cos(theta);
        circleVertices[2 * i + 1] = radius * glm::sin(theta);
	}
    unsigned int circleVertexIndices[cornerCount + 1];
    for (int i = 0; i < cornerCount + 1; i++) {
        circleVertexIndices[i] = i % cornerCount;
    }
    s_rendererData->circleVertexBuffer = new VertexBuffer(circleVertices, sizeof(circleVertices));

    s_rendererData->circleIndexBuffer = new IndexBuffer(circleVertexIndices, sizeof(circleVertexIndices) / sizeof(unsigned int));
    VertexBufferLayout layout;
    layout.push<float>(2);
    s_rendererData->circleVertexArray = new VertexArray();
    s_rendererData->circleVertexArray->addBuffer(*s_rendererData->circleVertexBuffer, layout);
}

static void initQuad()
{
    float quadVertices[] = {
         -0.5f, -0.5f, /* Bottom left */
          0.5f, -0.5f, /* Bottom right */
          0.5f,  0.5f, /* Top right */
         -0.5f,  0.5f  /* Top left */
    };
    unsigned int quadVertexIndices[] = {
        0, 1, 2, /* First triangle */
        0, 2, 3  /* Second triangle */
    };
    s_rendererData->quadVertexBuffer = new VertexBuffer(quadVertices, 4*2*sizeof(float));
    s_rendererData->quadIndexBuffer = new IndexBuffer(quadVertexIndices, 6);
    s_rendererData->quadVertexArray = new VertexArray();

    VertexBufferLayout layout;
    layout.push<float>(2);
    s_rendererData->quadVertexArray->addBuffer(*(s_rendererData->quadVertexBuffer), layout);
}

static void enableBlending()
{
    /* Specify how colors should be blended together */
    /* alpha * src + (1 - alpha) * target */
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
}

void Renderer::init()
{
    s_rendererData = new RendererStorage();
    enableBlending();

    /* TODO: Remove default window size 800x600 */
    s_rendererData->projectionMatrix = new glm::mat4(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f));
    s_rendererData->viewMatrix = new glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
    s_rendererData->solidColorShader = new Shader("../resources/shaders/solid_color.shader");
    initCircle();
    initQuad();
}

void Renderer::setCamera(const glm::vec3 view)
{
    *s_rendererData->viewMatrix = glm::translate(glm::mat4(1.0f), view);
}

void Renderer::setWindowSize(int width, int height)
{
    *(s_rendererData->projectionMatrix) = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
}

void Renderer::destroy()
{
    delete s_rendererData;
}

void Renderer::clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

/* TODO: Improve this inefficient function */
static float angleToXAxis(const glm::vec2& start, const glm::vec2& end) {
    const float x = end.x - start.x;
    const float y = end.y - start.y;
    float angle = glm::atan(x, y);
    if (x > 0 && y > 0) {
    } else if (x < 0 && y > 0) {
        angle += glm::pi<float>();
    } else if (x < 0 && y < 0) {
        angle += 2 * glm::pi<float>();
    } else {
        angle += glm::pi<float>();
    }
    return angle;
}

void Renderer::drawLine(const glm::vec2& start, const glm::vec2& end, float width, const glm::vec4& color)
{
    /* Represent a line as a thin quad */
    const float angle = angleToXAxis(start, end);
    const glm::vec3 location((start.x + end.x) / 2, (start.y + end.y) / 2, 1.0f);
    const glm::vec2 size(glm::distance(start, end), width);
    drawQuad(location, size, angle, color);
}

/* TODO: Should this be part of the class? */
/* Scale with pixel scale */
static glm::mat4 scale2D(const glm::vec2 &size) {
    return glm::scale(glm::mat4(1.0f), { size.x * s_rendererData->centimetersToPxScale,
                                         size.y * s_rendererData->centimetersToPxScale,
                                         1.0f });
}

/* Translate with pixel scale */
static glm::mat4 translate2D(const glm::vec3 &position)
{
    return glm::translate(glm::mat4(1.0f), { position.x * s_rendererData->centimetersToPxScale,
                                             position.y * s_rendererData->centimetersToPxScale,
                                             position.z });
}

void Renderer::drawQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
{
    s_rendererData->solidColorShader->bind();
    s_rendererData->quadVertexArray->bind();
    s_rendererData->quadIndexBuffer->bind();
    glm::mat4 scale = scale2D(size);
    glm::mat4 translate = translate2D(position);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });
    glm::mat4 mvpMatrix = *s_rendererData->projectionMatrix * *s_rendererData->viewMatrix * translate * rotation * scale;
    s_rendererData->solidColorShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    s_rendererData->solidColorShader->setUniform4f("u_Color", color[0], color[1], color[2], color[3]);
    GLCall(glDrawElements(GL_TRIANGLES, s_rendererData->quadIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawCircle(const glm::vec3& position, float radius, const glm::vec4& color)
{
    s_rendererData->solidColorShader->bind();
    s_rendererData->circleVertexArray->bind();
    s_rendererData->circleIndexBuffer->bind();
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), { 2 * radius, 2 * radius, 1.0f });
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 mvpMatrix = *s_rendererData->projectionMatrix * *s_rendererData->viewMatrix * transform * scale;
    s_rendererData->solidColorShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    s_rendererData->solidColorShader->setUniform4f("u_Color", color[0], color[1], color[2], color[3]);
    GLCall(glDrawElements(GL_TRIANGLE_FAN, s_rendererData->circleIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}
