#include "../include/Render2DPass.h"
#include "../include/Render2DPipeline.h"
#include "../include/TextureRenderTarget.h"
#include "../include/Camera2D.h"
#include "../include/Shader.h"
#include "../include/Drawable2D.h"

namespace GGE
{

Render2DPass::Render2DPass(int width, int height)
        : width(width), height(height),
            pipeline(new Render2DPipeline()),
            target(new TextureRenderTarget(width, height))
{
}

Render2DPass::~Render2DPass()
{
        delete pipeline;
        delete target;
}

void Render2DPass::render(const Camera2D& camera, Shader& shader,
                          const std::vector<Drawable2D*>& drawables)
{
    render(camera, shader, drawables, target);
}

void Render2DPass::render(const Camera2D& camera, Shader& shader,
                          const std::vector<Drawable2D*>& drawables,
                          RenderTarget* renderTarget)
{
    if (!pipeline)
        return;

    pipeline->beginFrame(camera, shader, renderTarget);
    pipeline->draw(drawables);
    pipeline->endFrame();
}

void Render2DPass::presentToScreen()
{
    if (!target)
        return;

    // Blit FBO conteúdo para tela
    glBindFramebuffer(GL_READ_FRAMEBUFFER, target->getFramebuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR
    );

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

}
