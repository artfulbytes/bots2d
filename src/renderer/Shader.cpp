#include "Shader.h"
#include <glad/gl.h>
#include "GLError.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 * Store the shaders in the executable instead of separate files to avoid
 * locating them at runtime.
 */
namespace {
const char textureVertexShader[] = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_mvpMatrix;

void main()
{
   gl_Position = u_mvpMatrix * position;
   v_texCoord = texCoord;
};
)glsl";

const char textureFragmentShader[] = R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_texCoord);
    color = texColor;
};
)glsl";

const char solidColorVertexShader[] = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_mvpMatrix;

void main()
{
   gl_Position = u_mvpMatrix * position;
};
)glsl";

const char solidColorFragmentShader[] = R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
   color = u_Color;
};
)glsl";
}

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

Shader::Shader(Shader::Program shaderProgram)
{
    ShaderProgramSource source;
    switch (shaderProgram)
    {
    case Shader::Program::SolidColor:
        source = { solidColorVertexShader, solidColorFragmentShader };
        break;
    case Shader::Program::Texture:
        source = { textureVertexShader, textureFragmentShader };
        break;
    }
    m_id = create(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_id));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_id));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string &name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string &name, const glm::vec4 &color)
{
    GLCall(glUniform4f(getUniformLocation(name), color[0], color[1], color[2], color[3]));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string &name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_id, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: Uniform doesn't exist (" << name << ")" << std::endl;
    }
    m_uniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::compile(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::create(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    /* Vertex shader specifiy where the vertex should be rendered (runs for each vertex) */
    unsigned int vs = compile(GL_VERTEX_SHADER, vertexShader);
    /* Fragment shader runs for each pixel */
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}
