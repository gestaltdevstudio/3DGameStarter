#ifndef RENDERTARGET_H_INCLUDED
#define RENDERTARGET_H_INCLUDED

namespace GGE
{

class RenderTarget
{
public:
    virtual ~RenderTarget() = default;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual unsigned int getFramebuffer() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};

}

#endif // RENDERTARGET_H_INCLUDED
