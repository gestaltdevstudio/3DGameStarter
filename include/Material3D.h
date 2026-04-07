#ifndef MATERIAL3D_H_INCLUDED
#define MATERIAL3D_H_INCLUDED

#include "Definitions.h"

namespace GGE
{

class Shader;

enum BlendMode3D
{
    BLEND_3D_DISABLED = 0,
    BLEND_3D_ALPHA = 1,
};

struct RenderState3D
{
    bool depthTestEnabled = true;
    bool depthWriteEnabled = true;
    BlendMode3D blendMode = BLEND_3D_DISABLED;

    unsigned int toFlags() const
    {
        unsigned int flags = 0;

        if (depthTestEnabled)
            flags |= 1u << 0;
        if (depthWriteEnabled)
            flags |= 1u << 1;

        flags |= (static_cast<unsigned int>(blendMode) & 0x3u) << 2;
        return flags;
    }
};

class Material3D
{
public:
    void setShader(Shader* value) { shader = value; }
    Shader* getShader() const { return shader; }

    void setTextureId(GLuint value)
    {
        textureId = value;
        textured = (value != 0);
    }

    GLuint getTextureId() const { return textureId; }
    bool isTextured() const { return textured; }

    void setRenderState(const RenderState3D& value) { renderState = value; }
    RenderState3D& getRenderState() { return renderState; }
    const RenderState3D& getRenderState() const { return renderState; }

    unsigned int getRenderStateFlags() const { return renderState.toFlags(); }

private:
    Shader* shader = nullptr;
    GLuint textureId = 0;
    bool textured = false;
    RenderState3D renderState;
};

}

#endif // MATERIAL3D_H_INCLUDED