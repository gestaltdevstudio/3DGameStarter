#include "../include/Render3DPipeline.h"

#include "../include/Drawable3D.h"
#include "../include/GraphicsManager.h"
#include "../include/RenderTarget.h"


namespace GGE
{

void Render3DPipeline::beginFrame(const Camera3D& camera, RenderTarget* target)
{
    GraphicsManager* gm = GraphicsManager::getInstance();

    if (target)
    {
        gm->bindFramebuffer(target->getFramebuffer());
        glViewport(0, 0, target->getWidth(), target->getHeight());
    }
    else
    {
        gm->bindFramebuffer(0);
        const Viewport& vp = gm->getViewport();
        glViewport(vp.x, vp.y, vp.width, vp.height);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    // Só limpa depth: a cor do frame 2D já está no backbuffer e serve de fundo.
    glClear(GL_DEPTH_BUFFER_BIT);

    projection = camera.getProjectionMatrix();
    view = camera.getViewMatrix();
    renderer.begin();
}

void Render3DPipeline::draw(Drawable3D* object)
{
    renderer.submit(object);
}

void Render3DPipeline::draw(const std::vector<Drawable3D*>& objects)
{
    for (size_t i = 0; i < objects.size(); ++i)
    {
        draw(objects[i]);
    }
}

void Render3DPipeline::endFrame()
{
    renderer.flush(projection, view);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
}

const Renderer3D::FrameStats& Render3DPipeline::getLastStats() const
{
    return renderer.getLastStats();
}

}