#include "../include/GraphicsManager.h"
#include "../include/GraphicsUtils.h"
#include "../include/Render2DPass.h"
#include "../include/Render3DPass.h"
#include "../include/TextureRenderTarget.h"

#include <algorithm>

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

void GraphicsManager::add2DDrawable(Drawable2D* drawable)
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

void GraphicsManager::set3DPipeline(Render3DPass* pipeline)
{
    if (!pipeline)
        return;

    active3DPass.reset(pipeline);
}

void GraphicsManager::set3DCamera(const Camera3D& camera)
{
    camera3D = camera;
}

Camera3D& GraphicsManager::get3DCamera()
{
    return camera3D;
}

const Camera3D& GraphicsManager::get3DCamera() const
{
    return camera3D;
}

void GraphicsManager::addGraphicsObject3D(const std::string& name, Drawable3D* object)
{
    if (!object)
        return;

    graphicsObjects3D[name] = object;
}

void GraphicsManager::removeGraphicsObject3D(const std::string& name)
{
    std::map<std::string, Drawable3D*>::iterator it = graphicsObjects3D.find(name);
    if (it == graphicsObjects3D.end())
        return;

    graphicsObjects3D.erase(it);
}

void GraphicsManager::render3DFrame()
{
    if (currentViewport.width > 0 && currentViewport.height > 0)
    {
        const float aspect = (float)currentViewport.width / (float)currentViewport.height;
        camera3D.setPerspective(camera3D.getFovDegrees(), aspect, camera3D.getNearPlane(), camera3D.getFarPlane());
    }

    queued3DObjects.clear();
    for (std::map<std::string, Drawable3D*>::iterator it = graphicsObjects3D.begin();
         it != graphicsObjects3D.end();
         ++it)
    {
        if (it->second)
            queued3DObjects.push_back(it->second);
    }

    std::stable_sort(queued3DObjects.begin(), queued3DObjects.end(),
        [](Drawable3D* left, Drawable3D* right)
        {
            if (!left || !right)
                return left < right;

            return left->getRenderOrder() < right->getRenderOrder();
        });

    if (active3DPass)
    {
        active3DPass->render(camera3D, queued3DObjects);
        return;
    }

    Render3DPass localPass;
    localPass.render(camera3D, queued3DObjects);
}

const Renderer3D::FrameStats& GraphicsManager::get3DStats() const
{
    static const Renderer3D::FrameStats emptyStats;

    if (!active3DPass)
        return emptyStats;

    return active3DPass->getLastStats();
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
