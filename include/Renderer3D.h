#ifndef RENDERER3D_H_INCLUDED
#define RENDERER3D_H_INCLUDED

#include "Shader.h"
#include "Material3D.h"
#include "GLPlatform.h"

#include <glm/glm.hpp>

#include <map>
#include <vector>

namespace GGE
{

class Drawable3D;

class Renderer3D
{
public:
    static constexpr int MaxTextures3D = 8;

    struct FrameStats
    {
        unsigned int drawCalls = 0;
        unsigned int batches = 0;
        unsigned int instances = 0;
    };

    void begin();
    void submit(Drawable3D* object);
    void flush(const glm::mat4& projection, const glm::mat4& view);

    const FrameStats& getLastStats() const { return stats; }

private:
    struct BatchKey3D
    {
        unsigned int renderOrder = 0;
        Shader* shader = nullptr;
        GLuint vertexBuffer = 0;
        GLuint elementBuffer = 0;
        GLuint uvBuffer = 0;
        GLsizei indexCount = 0;
        unsigned int renderStateFlags = 0;
        // texture removida da key: diferentes texturas acumulam no mesmo batch

        bool operator<(const BatchKey3D& other) const;
    };

    struct InstanceData3D
    {
        glm::mat4 modelMatrix  = glm::mat4(1.0f);
        glm::vec4 color        = glm::vec4(1.0f);
        int       textureIndex = 0;
        // 3 floats de padding → sizeof = 96 bytes (múltiplo de 16)
        float _pad0 = 0.f, _pad1 = 0.f, _pad2 = 0.f;
    };

    // Estado completo de um batch: instâncias + tabela de until 8 texturas
    struct BatchData3D
    {
        std::vector<InstanceData3D> instances;
        GLuint textures[MaxTextures3D] = {};
        int    textureCount = 0;

        int getTextureSlot(GLuint textureID)
        {
            if (textureID == 0) return 0;
            for (int i = 0; i < textureCount; ++i)
                if (textures[i] == textureID) return i;
            if (textureCount < MaxTextures3D)
            {
                textures[textureCount] = textureID;
                return textureCount++;
            }
            return 0; // fallback: batch cheio reutiliza slot 0
        }
    };

    void drawBatch(const BatchKey3D& key, const BatchData3D& data,
                   const glm::mat4& projection, const glm::mat4& view);

    std::map<BatchKey3D, BatchData3D> queuedBatches;
    FrameStats stats;
    GLuint vertexArray = 0;
    GLuint instanceBuffer = 0;
};

}

#endif // RENDERER3D_H_INCLUDED