#include "../include/RendererBatch2D.h"

#include <glad/glad.h>
#include <iostream>

namespace GGE
{

    RendererBatch2D::RendererBatch2D()
    {
    }

    RendererBatch2D::~RendererBatch2D()
    {
        finish();
    }

    void RendererBatch2D::init(uint32_t maxInstances)
    {
        m_MaxInstances = maxInstances;
        m_CPUBuffer.reserve(maxInstances);

        createBuffers();
    }

    void RendererBatch2D::finish()
    {
        if (m_EBO)         glDeleteBuffers(1, &m_EBO);
        if (m_InstanceVBO) glDeleteBuffers(1, &m_InstanceVBO);
        if (m_QuadVBO)     glDeleteBuffers(1, &m_QuadVBO);
        if (m_VAO)         glDeleteVertexArrays(1, &m_VAO);

        m_VAO = m_QuadVBO = m_InstanceVBO = m_EBO = 0;
    }

    void RendererBatch2D::begin()
    {
        m_InstanceCount = 0;
        m_CPUBuffer.clear();
        textureCount = 0;   // IMPORTANTE
    }

    void RendererBatch2D::submit(const InstanceData2D& instance)
    {
        if (m_InstanceCount >= m_MaxInstances)
            return; // ou Flush + continue, decisão sua depois

        m_CPUBuffer.push_back(instance);
        m_InstanceCount++;
    }

    void RendererBatch2D::flush()
    {

        if (m_InstanceCount == 0)
            return;

        glUseProgram(activeShader->getShaderID());

        int samplers[8] = { 0,1,2,3,4,5,6,7 };
        activeShader->setIntArray("u_Textures", samplers, 8);


        for (int i = 0; i < textureCount; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }


        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
        // orphaning: diz ao driver que não precisamos do conteúdo antigo
        glBufferData(
            GL_ARRAY_BUFFER,
            m_MaxInstances * sizeof(InstanceData2D),
            nullptr,
            GL_DYNAMIC_DRAW
        );

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            m_InstanceCount * sizeof(InstanceData2D),
            m_CPUBuffer.data()
        );

        glBindVertexArray(m_VAO);

        glDrawElementsInstanced(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr,
            m_InstanceCount
        );

//        glDrawElements(
//            GL_TRIANGLES,
//            6,
//            GL_UNSIGNED_INT,0
//        );

        glBindVertexArray(0);

    }

    void RendererBatch2D::createBuffers()
    {
        // Quad base (unitário, centrado)
//        float quadVertices[] = {
//            -0.5f, -0.5f,
//             0.5f, -0.5f,
//             0.5f,  0.5f,
//            -0.5f,  0.5f
//        };

        float quadVertices[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
};


        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_QuadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            (void*)0
        );

        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &m_InstanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_MaxInstances * sizeof(InstanceData2D),
            nullptr,
            GL_DYNAMIC_DRAW
        );

        std::size_t offset = 0;

        // position
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, position));
        glVertexAttribDivisor(1, 1);

        // size
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, size));
        glVertexAttribDivisor(2, 1);

        // rotation
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, rotation));
        glVertexAttribDivisor(3, 1);

        // uv
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, uv));
        glVertexAttribDivisor(4, 1);

        // color
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, color));
        glVertexAttribDivisor(5, 1);

        // z
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, z));
        glVertexAttribDivisor(6, 1);

        // Alpha
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, alpha));
        glVertexAttribDivisor(7, 1);

        // texture index
        glEnableVertexAttribArray(8);
        glVertexAttribIPointer(8, 1, GL_INT, sizeof(InstanceData2D), (void*)offsetof(InstanceData2D, textureIndex));
        glVertexAttribDivisor(8, 1);

        glBindVertexArray(0);
    }

    int RendererBatch2D::getTextureSlot(GLuint textureID)
    {
        for (int i = 0; i < textureCount; i++)
            if (textures[i] == textureID)
                return i;

        textures[textureCount] = textureID;
        return textureCount++;
    }

}
