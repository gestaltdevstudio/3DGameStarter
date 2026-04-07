#include "../include/TextureRenderTarget.h"

namespace GGE
{

TextureRenderTarget::TextureRenderTarget(int width, int height)
    : width(width), height(height)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // fallback: no framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureRenderTarget::~TextureRenderTarget()
{
    if (rbo) glDeleteRenderbuffers(1, &rbo);
    if (texture) glDeleteTextures(1, &texture);
    if (fbo) glDeleteFramebuffers(1, &fbo);
}

void TextureRenderTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void TextureRenderTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
