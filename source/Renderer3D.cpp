#include "../include/Renderer3D.h"

#include "../include/Drawable3D.h"

#include <cstddef>

namespace GGE
{

bool Renderer3D::BatchKey3D::operator<(const BatchKey3D& other) const
{
    if (renderOrder != other.renderOrder) return renderOrder < other.renderOrder;
    if (shader != other.shader) return shader < other.shader;
    if (vertexBuffer != other.vertexBuffer) return vertexBuffer < other.vertexBuffer;
    if (elementBuffer != other.elementBuffer) return elementBuffer < other.elementBuffer;
    if (uvBuffer != other.uvBuffer) return uvBuffer < other.uvBuffer;
    if (indexCount != other.indexCount) return indexCount < other.indexCount;
    return renderStateFlags < other.renderStateFlags;
    // texture não entra na key: objetos com texturas diferentes compartilham o mesmo batch
}

void Renderer3D::begin()
{
    if (!vertexArray)
        glGenVertexArrays(1, &vertexArray);
    if (!instanceBuffer)
        glGenBuffers(1, &instanceBuffer);

    queuedBatches.clear();
    stats = FrameStats{};
}

void Renderer3D::submit(Drawable3D* object)
{
    if (!object || !object->isVisible())
        return;

    if (!object->getMesh())
        return;

    const Material3D& material = object->getMaterial();

    BatchKey3D key;
    key.renderOrder = object->getRenderOrder();
    key.shader = material.getShader();
    key.vertexBuffer = object->getVertexBuffer();
    key.elementBuffer = object->getElementBuffer();
    key.uvBuffer = object->getUVBuffer();
    key.indexCount = object->getIndexCount();
    key.renderStateFlags = material.getRenderStateFlags();
    // texture NÃO entra na key; é registrada por instância no BatchData3D

    if (!key.shader || key.indexCount <= 0)
        return;

    BatchData3D& batch = queuedBatches[key];

    InstanceData3D data;
    data.modelMatrix   = object->buildModelMatrix();
    Vector4 color      = object->getColor();
    data.color         = glm::vec4(color.r, color.g, color.b, color.a * object->getAlpha());
    data.textureIndex  = batch.getTextureSlot(material.getTextureId());

    batch.instances.push_back(data);
    stats.instances++;
}

void Renderer3D::drawBatch(const BatchKey3D& key, const BatchData3D& data,
                           const glm::mat4& projection, const glm::mat4& view)
{
    const std::vector<InstanceData3D>& instances = data.instances;
    if (instances.empty())
        return;

    const bool depthTestEnabled  = (key.renderStateFlags & (1u << 0)) != 0;
    const bool depthWriteEnabled = (key.renderStateFlags & (1u << 1)) != 0;
    const unsigned int blendMode = (key.renderStateFlags >> 2) & 0x3u;

    if (depthTestEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    glDepthMask(depthWriteEnabled ? GL_TRUE : GL_FALSE);

    if (blendMode == BLEND_3D_ALPHA)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    key.shader->bind();
    glBindVertexArray(vertexArray);

    // --- bind até 8 texturas e preenche u_Textures[0..N] ---
    const bool hasUVs = (key.uvBuffer != 0u);
    int samplers[MaxTextures3D] = { 0,1,2,3,4,5,6,7 };
    key.shader->setIntArray("u_Textures", samplers, MaxTextures3D);

    for (int i = 0; i < data.textureCount; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, data.textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    key.shader->setInt("u_UseTexture", (hasUVs && data.textureCount > 0) ? 1 : 0);

    // --- vértices e UVs ---
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, key.vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if (hasUVs)
    {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, key.uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, key.elementBuffer);

    // --- carregar instâncias no GPU ---
    const glm::mat4 viewProj = projection * view;
    key.shader->setMat4("u_ViewProj", viewProj);

    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(instances.size() * sizeof(InstanceData3D)),
        instances.data(),
        GL_STREAM_DRAW);

    const GLsizei   stride      = static_cast<GLsizei>(sizeof(InstanceData3D));
    const std::size_t modelOff  = offsetof(InstanceData3D, modelMatrix);
    const std::size_t colorOff  = offsetof(InstanceData3D, color);
    const std::size_t texIdxOff = offsetof(InstanceData3D, textureIndex);

    // model matrix (4 x vec4, locations 2-5)
    for (int col = 0; col < 4; ++col)
    {
        glEnableVertexAttribArray(2 + col);
        glVertexAttribPointer(2 + col, 4, GL_FLOAT, GL_FALSE, stride,
                              (void*)(modelOff + sizeof(glm::vec4) * col));
        glVertexAttribDivisor(2 + col, 1);
    }
    // color (location 6)
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOff);
    glVertexAttribDivisor(6, 1);
    // textureIndex (location 7) — inteiro, usa IPointer
    glEnableVertexAttribArray(7);
    glVertexAttribIPointer(7, 1, GL_INT, stride, (void*)texIdxOff);
    glVertexAttribDivisor(7, 1);

    glDrawElementsInstanced(
        GL_TRIANGLES,
        key.indexCount,
        GL_UNSIGNED_SHORT,
        (void*)0,
        static_cast<GLsizei>(instances.size()));
    stats.drawCalls++;

    // cleanup divisors
    for (int col = 0; col < 4; ++col) { glVertexAttribDivisor(2 + col, 0); glDisableVertexAttribArray(2 + col); }
    glVertexAttribDivisor(6, 0); glDisableVertexAttribArray(6);
    glVertexAttribDivisor(7, 0); glDisableVertexAttribArray(7);

    glDisableVertexAttribArray(0);
    if (hasUVs) glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    key.shader->unbind();
    glDepthMask(GL_TRUE);
}

void Renderer3D::flush(const glm::mat4& projection, const glm::mat4& view)
{
    for (auto& kv : queuedBatches)
    {
        drawBatch(kv.first, kv.second, projection, view);
        stats.batches++;
    }

    queuedBatches.clear();
}

}