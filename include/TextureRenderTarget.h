#ifndef TEXTURERENDERTARGET_H_INCLUDED
#define TEXTURERENDERTARGET_H_INCLUDED

#include "RenderTarget.h"
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
