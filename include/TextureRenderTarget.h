#ifndef TEXTURERENDERTARGET_H_INCLUDED
#define TEXTURERENDERTARGET_H_INCLUDED

#include "RenderTarget.h"

namespace GGE
{

class TextureRenderTarget : public RenderTarget
{
public:
    TextureRenderTarget(int width, int height);
    ~TextureRenderTarget() override;

    void bind() override;
    void unbind() override;

    unsigned int getFramebuffer() const override { return fbo; }
    unsigned int getTexture() const { return texture; }
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }

private:
    unsigned int fbo = 0;
    unsigned int texture = 0;
    unsigned int rbo = 0;
    int width;
    int height;
};

}

#endif // TEXTURERENDERTARGET_H_INCLUDED
