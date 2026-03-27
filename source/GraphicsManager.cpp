#include "../include/GraphicsManager.h"
#include "../include/GraphicsUtils.h"
#include "../include/Render2DPass.h"
#include "../include/TextureRenderTarget.h"
#include "../include/RenderTarget.h"
#include "../include/Camera2D.h"
#include "../include/Shader.h"
#include "../include/Drawable.h"

#include <glad/glad.h>

namespace GGE
{

GraphicsManager* GraphicsManager::instance = nullptr;

GraphicsManager* GraphicsManager::getInstance()
{
    if (!instance)
        instance = new GraphicsManager();
    return instance;
}

void GraphicsManager::beginFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);

    glViewport(
        currentViewport.x,
        currentViewport.y,
        currentViewport.width,
        currentViewport.height
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsManager::endFrame()
{
    // por enquanto vazio
    // futuramente: renderpasses, debug, stats
}

void GraphicsManager::setViewport(const Viewport& vp)
{
    currentViewport = vp;

    glViewport(
        vp.x,
        vp.y,
        vp.width,
        vp.height
    );
}

void GraphicsManager::submit(const InstanceData2D& i)
{
    batch.submit(i);//, currentViewport);
}

const Viewport& GraphicsManager::getViewport() const
{
    return currentViewport;
}

void GraphicsManager::bindFramebuffer(unsigned int fbo)
{
    currentFBO = fbo;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void GraphicsManager::clear2DDrawables()
{
    queued2DDrawables.clear();
}

void GraphicsManager::add2DDrawable(Drawable* drawable)
{
    if (!drawable)
        return;
    queued2DDrawables.push_back(drawable);
}

void GraphicsManager::set2DPipeline(Render2DPass* pipeline)
{
    if (!pipeline)
        return;
    active2DPass.reset(pipeline);
}

void GraphicsManager::render2DDrawables(const Camera2D& camera, Shader& shader, RenderTarget* target)
{
    if (active2DPass)
    {
        RenderTarget* resolvedTarget = target;
        if (!resolvedTarget)
            resolvedTarget = active2DPass->getTarget();

        active2DPass->render(camera, shader, queued2DDrawables, resolvedTarget);
        active2DPass->presentToScreen();
        return;
    }

    // fallback sem pipeline injetado
    Render2DPass localPass(currentViewport.width, currentViewport.height);
    RenderTarget* resolvedTarget = target;
    if (!resolvedTarget)
        resolvedTarget = localPass.getTarget();

    localPass.render(camera, shader, queued2DDrawables, resolvedTarget);
    localPass.presentToScreen();
}

void GraphicsManager::renderText(Text* text, Camera2D* camera)
{
    textRenderer.render(text, camera);
}

void GraphicsManager::onFramebufferResize(int w, int h)
{
    surface.screenSize = { w, h };
    surface.renderAreaSize = surface.screenSize;

    // mantém aspect ratio fixo
    GraphicsUtils::calculateViewportSize(surface.renderAreaSize);

    surface.renderAreaPosition.x =
        (surface.screenSize.x - surface.renderAreaSize.x) / 2;

    surface.renderAreaPosition.y =
        (surface.screenSize.y - surface.renderAreaSize.y) / 2;

    // viewport default = área útil
    currentViewport.x = surface.renderAreaPosition.x;
    currentViewport.y = surface.renderAreaPosition.y;
    currentViewport.width  = surface.renderAreaSize.x;
    currentViewport.height = surface.renderAreaSize.y;

    glViewport(
        currentViewport.x,
        currentViewport.y,
        currentViewport.width,
        currentViewport.height
    );
}

const RenderSurface& GraphicsManager::getSurface() const
{
    return surface;
}

} // namespace GGE
