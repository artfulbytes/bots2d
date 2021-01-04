#include "Renderer.h"
#include "GLError.h"
#include "ResourcesHelper.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "TexCoords.h"
#include "QuadCoords.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cassert>
#include <memory>

namespace {
    const unsigned int quadVertexBufferSize = 4 * 4 * sizeof(float);
    const float metersToPxScale = 1000.0f;
}

/**
 * Contains the state of the renderer. The vertex buffers are allocated once, and the renderer
 * then re-use them for each draw call.
 */
struct RendererStorage
{
    float dynamicQuadVertices[16] = {
        /* Position, texture coordinates */
        -0.5f, -0.5f, 0.0f, 0.0f, /* Bottom left */
         0.5f, -0.5f, 1.0f, 0.0f, /* Bottom right */
         0.5f,  0.5f, 1.0f, 1.0f, /* Top right */
        -0.5f,  0.5f, 0.0f, 1.0f  /* Top left */
    };
    std::unique_ptr<VertexBuffer> quadDynamicVertexBuffer;
    std::unique_ptr<VertexArray> quadDynamicVertexArray;
    std::unique_ptr<VertexArray> quadStaticVertexArray;
    std::unique_ptr<VertexBuffer> quadStaticVertexBuffer;
    std::unique_ptr<IndexBuffer> quadIndexBuffer;

    std::unique_ptr<VertexBuffer> circleVertexBuffer;
    std::unique_ptr<VertexArray> circleVertexArray;
    std::unique_ptr<IndexBuffer> circleIndexBuffer;

    std::unique_ptr<Shader> solidColorShader;
    std::unique_ptr<Shader> textureShader;
    std::unique_ptr<glm::mat4> projectionMatrix;
    std::unique_ptr<glm::mat4> viewMatrix;
};

static std::unique_ptr<RendererStorage> s_rendererData;

static glm::mat4 scale2D(const glm::vec2 &size, bool pixelScale) {
    const float sizeX = pixelScale ? (size.x * metersToPxScale) : size.x;
    const float sizeY = pixelScale ? (size.y * metersToPxScale) : size.y;
    return glm::scale(glm::mat4(1.0f), { sizeX, sizeY, 1.0f });
}

static glm::mat4 translate2D(const glm::vec2 &position, bool pixelScale)
{
    const float posX = pixelScale ? (position.x * metersToPxScale) : position.x;
    const float posY = pixelScale ? (position.y * metersToPxScale) : position.y;
    return glm::translate(glm::mat4(1.0f), { posX, posY, 0.0f });
}

static void initCircle()
{
    const float radius = 0.5f;
    const auto cornerCount = 60;
    const auto circleVerticesCount = cornerCount * 2;
    const auto circleVertexIndicesCount = cornerCount + 1;
    float circleVertices[circleVerticesCount];

    /* Sin and cos are expensive, but we only run this function once */
	for(int i = 0; i < cornerCount; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(cornerCount);
        circleVertices[2 * i] = radius * glm::cos(theta);
        circleVertices[2 * i + 1] = radius * glm::sin(theta);
	}
    unsigned int circleVertexIndices[circleVertexIndicesCount];
    for (int i = 0; i < cornerCount + 1; i++) {
        circleVertexIndices[i] = i % cornerCount;
    }
    s_rendererData->circleVertexBuffer = std::make_unique<VertexBuffer>(circleVertices, circleVerticesCount * sizeof(float), VertexBuffer::DrawType::Static);

    s_rendererData->circleIndexBuffer = std::make_unique<IndexBuffer>(circleVertexIndices, circleVertexIndicesCount);
    VertexBufferLayout layout;
    layout.push<float>(2);
    s_rendererData->circleVertexArray = std::make_unique<VertexArray>();
    s_rendererData->circleVertexArray->addBuffer(*s_rendererData->circleVertexBuffer, layout);
}

static void initQuad()
{
    const float defaultQuadVertices[16] = {
        /* Position, texture coordinates */
        -0.5f, -0.5f, 0.0f, 0.0f, /* Bottom left */
         0.5f, -0.5f, 1.0f, 0.0f, /* Bottom right */
         0.5f,  0.5f, 1.0f, 1.0f, /* Top right */
        -0.5f,  0.5f, 0.0f, 1.0f  /* Top left */
    };
    unsigned int quadVertexIndices[] = {
        0, 1, 2, /* First triangle */
        0, 2, 3  /* Second triangle */
    };
    s_rendererData->quadIndexBuffer = std::make_unique<IndexBuffer>(quadVertexIndices, 6);
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    s_rendererData->quadStaticVertexBuffer = std::make_unique<VertexBuffer>(defaultQuadVertices, quadVertexBufferSize, VertexBuffer::DrawType::Static);
    s_rendererData->quadStaticVertexArray = std::make_unique<VertexArray>();
    s_rendererData->quadStaticVertexArray->addBuffer(*(s_rendererData->quadStaticVertexBuffer), layout);

    s_rendererData->quadDynamicVertexBuffer = std::make_unique<VertexBuffer>(defaultQuadVertices, quadVertexBufferSize, VertexBuffer::DrawType::Dynamic);

    s_rendererData->quadDynamicVertexArray = std::make_unique<VertexArray>();
    s_rendererData->quadDynamicVertexArray->addBuffer(*(s_rendererData->quadDynamicVertexBuffer), layout);
}

static void enableBlending()
{
    /* Specify how colors should be blended together */
    /* alpha * src + (1 - alpha) * target */
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    glEnable(GL_MULTISAMPLE);
    //int samples;
    ///* Can be used to see if anti-aliasing is enabled */
    //glGetInteger(GL_SAMPLES, &samples);
    //std::cout << samples << std::endl;
}

void Renderer::init()
{
    ResourcesHelper::init();
    s_rendererData = std::make_unique<RendererStorage>();
    enableBlending();

    s_rendererData->projectionMatrix = std::make_unique<glm::mat4>(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f));
    s_rendererData->viewMatrix = std::make_unique<glm::mat4>(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));

    s_rendererData->solidColorShader = std::make_unique<Shader>("solid_color.shader");
    s_rendererData->textureShader = std::make_unique<Shader>("texture.shader");
    initCircle();
    initQuad();
}

void Renderer::destroy()
{
    /* Frees all smart pointers */
    s_rendererData = nullptr;
}

float Renderer::getPixelScaleFactor()
{
    return metersToPxScale;
}

void Renderer::setCameraPosition(const glm::vec2 &position, float zoomFactor)
{
    *s_rendererData->viewMatrix = translate2D(position, false) * glm::scale(glm::mat4(1.0f), { zoomFactor, zoomFactor, 1.0f });
}

void Renderer::setViewport(int x, int y, int width, int height)
{
    *(s_rendererData->projectionMatrix) = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    glViewport(x, y, width, height);
}

void Renderer::clear(const glm::vec4 &color)
{
    GLCall(glClearColor(color[0], color[1], color[2], color[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

static float angleToXAxis(const glm::vec2& start, const glm::vec2& end) {
    const float x = end.x - start.x;
    const float y = end.y - start.y;
    float angle = glm::atan(y, x);
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

void Renderer::drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color)
{
    /* Represent a line as a thin quad */
    const float angle = angleToXAxis(start, end);
    const glm::vec2 location((start.x + end.x) / 2, (start.y + end.y) / 2);
    const glm::vec2 size(glm::distance(start, end), width);
    drawRect(location, size, angle, color);
}

static glm::mat4 getQuadMvpMatrix(const glm::vec2 &position, const glm::vec2& size, float angle)
{
    glm::mat4 scale = scale2D(size, true);
    glm::mat4 translate = translate2D(position, true);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });
    return *s_rendererData->projectionMatrix * *s_rendererData->viewMatrix * translate * rotation * scale;
}

void Renderer::drawRect(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
{
    s_rendererData->quadStaticVertexArray->bind();
    s_rendererData->quadIndexBuffer->bind();
    glm::mat4 mvpMatrix = getQuadMvpMatrix(position, size, rotation);

    s_rendererData->solidColorShader->bind();
    s_rendererData->solidColorShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    s_rendererData->solidColorShader->setUniform4f("u_Color", color);
    GLCall(glDrawElements(GL_TRIANGLES, s_rendererData->quadIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}

static void setTexCoords(const TexCoords &texCoords)
{
    float quadVertices[16] = {
        /* Position, texture coordinates */
        -0.5f, -0.5f, 0.0f, 0.0f, /* Bottom left */
         0.5f, -0.5f, 1.0f, 0.0f, /* Bottom right */
         0.5f,  0.5f, 1.0f, 1.0f, /* Top right */
        -0.5f,  0.5f, 0.0f, 1.0f  /* Top left */
    };
    texCoords.assertLimits();
    quadVertices[2] = texCoords.BottomLeft.x;
    quadVertices[3] = texCoords.BottomLeft.y;
    quadVertices[6] = texCoords.BottomRight.x;
    quadVertices[7] = texCoords.BottomRight.y;
    quadVertices[10] = texCoords.TopRight.x;
    quadVertices[11] = texCoords.TopRight.y;
    quadVertices[14] = texCoords.TopLeft.x;
    quadVertices[15] = texCoords.TopLeft.y;
    s_rendererData->quadDynamicVertexBuffer->updateData(quadVertices, quadVertexBufferSize);
}

static void setQuadCoords(const QuadCoords &quadCoords)
{
    float quadVertices[16] = {};
    quadVertices[0] = quadCoords.BottomLeft.x;
    quadVertices[1] = quadCoords.BottomLeft.y;
    quadVertices[4] = quadCoords.BottomRight.x;
    quadVertices[5] = quadCoords.BottomRight.y;
    quadVertices[8] = quadCoords.TopRight.x;
    quadVertices[9] = quadCoords.TopRight.y;
    quadVertices[12] = quadCoords.TopLeft.x;
    quadVertices[13] = quadCoords.TopLeft.y;
    s_rendererData->quadDynamicVertexBuffer->updateData(quadVertices, quadVertexBufferSize);
}

void Renderer::drawQuad(const QuadCoords &quadCoords, const glm::vec4 &color)
{
    QuadCoords quadCoordsScaled = quadCoords;
    quadCoordsScaled *= metersToPxScale;
    setQuadCoords(quadCoordsScaled);

    s_rendererData->quadDynamicVertexArray->bind();
    s_rendererData->quadIndexBuffer->bind();
    glm::mat4 mvpMatrix = *s_rendererData->projectionMatrix * *s_rendererData->viewMatrix;

    s_rendererData->solidColorShader->bind();
    s_rendererData->solidColorShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    s_rendererData->solidColorShader->setUniform4f("u_Color", color);
    GLCall(glDrawElements(GL_TRIANGLES, s_rendererData->quadIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawRect(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Texture &texture,
                        const TexCoords *texCoords)
{
    if (texCoords != nullptr) {
        setTexCoords(*texCoords);
        s_rendererData->quadDynamicVertexArray->bind();
    } else {
        s_rendererData->quadStaticVertexArray->bind();
    }
    s_rendererData->quadIndexBuffer->bind();
    glm::mat4 mvpMatrix = getQuadMvpMatrix(position, size, rotation);

    texture.bind(0);
    s_rendererData->textureShader->bind();
    s_rendererData->textureShader->setUniform1i("u_Texture", 0);
    s_rendererData->textureShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    GLCall(glDrawElements(GL_TRIANGLES, s_rendererData->quadIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawCircle(const glm::vec2 &position, float radius, const glm::vec4 &color)
{
    s_rendererData->solidColorShader->bind();
    s_rendererData->circleVertexArray->bind();
    s_rendererData->circleIndexBuffer->bind();
    glm::mat4 scale = scale2D({2 * radius, 2 * radius }, true);
    glm::mat4 translate = translate2D(position, true);
    glm::mat4 mvpMatrix = *s_rendererData->projectionMatrix * *s_rendererData->viewMatrix * translate * scale;
    s_rendererData->solidColorShader->setUniformMat4f("u_mvpMatrix", mvpMatrix);
    s_rendererData->solidColorShader->setUniform4f("u_Color", color);
    GLCall(glDrawElements(GL_TRIANGLE_FAN, s_rendererData->circleIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
}
