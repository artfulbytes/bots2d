#ifndef SHADER_H_
#define SHADER_H_

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

struct ShaderProgramSource;

/**
 * Wrapper around OpenGL shader handling.
 * Loads and compiles a shader (GPU) program.
 */
class Shader
{
public:
    Shader(const std::string& filepath);
    ~Shader();
    void bind() const;
    void unbind() const;
    void setUniform1i(const std::string& name, int value);
    void setUniform4f(const std::string& name, const glm::vec4& color);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    std::string m_filepath;
    unsigned int m_id;
    std::unordered_map<std::string, int> m_uniformLocationCache;

    ShaderProgramSource parse(const std::string& filepath);
    unsigned int compile(unsigned int type, const std::string& source);
    unsigned int create(const std::string& vertexShader, const std::string& fragmentShader);
    int getUniformLocation(const std::string& name);
};
#endif /* SHADER_H_ */
