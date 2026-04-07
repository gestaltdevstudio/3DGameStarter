#ifndef RENDERERBATCH2D_H_INCLUDED
#define RENDERERBATCH2D_H_INCLUDED

#include "Shader.h"
#include <vector>
#include <cstdint>
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

// Estrutura exatamente no formato que vai para o VBO de instâncias
struct InstanceData2D
{
    glm::vec2 position;
    glm::vec2 size;
    float rotation;

    glm::vec4 uv;
    glm::vec4 color;

    float z;

    float alpha;
    int textureIndex;
};

class RendererBatch2D
{
public:
    RendererBatch2D();
    ~RendererBatch2D();

    static const int MaxTextures = 8;
    void init(uint32_t maxInstances);
    void finish();

    // chamado todo frame
    void begin();
    void submit(const InstanceData2D& instance);
    void flush();

    uint32_t getCount() const { return m_InstanceCount; }
    inline void setShader(Shader *_shader) { activeShader = _shader; }
    int getTextureSlot(GLuint textureID);

private:
    void createBuffers();
    GLuint textures[MaxTextures];
    int textureCount = 0;

    Shader *activeShader;
    uint32_t m_MaxInstances = 0;
    uint32_t m_InstanceCount = 0;

    std::vector<InstanceData2D> m_CPUBuffer;

    unsigned int m_VAO = 0;
    unsigned int m_QuadVBO = 0;     // vértices do quad
    unsigned int m_InstanceVBO = 0; // dados instanciados
    unsigned int m_EBO = 0;
};

}

#endif // RENDERERBATCH2D_H_INCLUDED
