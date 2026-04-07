#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif defined(__WIN32__)
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include "OS_GLFW.h"
  #include <glad/glad.h>
 #else
  #include "OS_iOS.h"
 #endif
#else
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#endif

namespace GGE
{

    class Shader
    {
        public:
            virtual inline ~Shader()
            {
                if (shaderID)
                {
                    glDeleteProgram(shaderID);
                }
            }

            inline GLuint getShaderID() { return shaderID; }
            inline void setShaderID(GLuint _shaderID) { shaderID = _shaderID; }

            void bind() const;
            void unbind() const;

            void setIntArray(const std::string& name, int *v, int c);
            void setInt(const std::string& name, int v);
            void setFloat(const std::string& name, float v);
            void setVec2(const std::string& name, const glm::vec2& v);
            void setVec3(const std::string& name, const glm::vec3& v);
            void setVec4(const std::string& name, const glm::vec4& v);
            void setMat4(const std::string& name, const glm::mat4& m);

        protected:

            std::unordered_map<std::string, GLint> uniformCache;
            GLuint shaderID;

    };

}

#endif // SHADER_H_INCLUDED
