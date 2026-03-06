#include "../include/Shader.h"
#include <glad/glad.h>   // ou glew, dependendo do teu setup
#include <iostream>

namespace GGE
{

    // =====================================================
    // Helpers internos
    // =====================================================

    static GLint GetUniformLocation(
        GLuint program,
        std::unordered_map<std::string, GLint>& cache,
        const std::string& name)
    {
        // Já foi cacheado?
        auto it = cache.find(name);
        if (it != cache.end())
            return it->second;

        // Busca no OpenGL
        GLint location = glGetUniformLocation(program, name.c_str());

#ifdef _DEBUG
        if (location == -1)
        {
            std::cerr << "[Shader] Warning: uniform '"
                      << name << "' not found or not used.\n";
        }
#endif

        // Cacheia (mesmo -1, evita chamadas repetidas)
        cache[name] = location;
        return location;
    }

    // =====================================================
    // Bind / Unbind
    // =====================================================

    void Shader::bind() const
    {
        glUseProgram(shaderID);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    // =====================================================
    // Uniform setters
    // =====================================================

    void Shader::setIntArray(const std::string& name, int *v, int c)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform1iv(loc, c, v);
    }

    void Shader::setInt(const std::string& name, int v)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform1i(loc, v);
    }

    void Shader::setFloat(const std::string& name, float v)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform1f(loc, v);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& v)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform2f(loc, v.x, v.y);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& v)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform3f(loc, v.x, v.y, v.z);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& v)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniform4f(loc, v.x, v.y, v.z, v.w);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& m)
    {
        GLint loc = GetUniformLocation(shaderID, uniformCache, name);
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    }

} // namespace GGE
