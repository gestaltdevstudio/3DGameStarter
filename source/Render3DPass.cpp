#include "../include/Render3DPass.h"

#include "../include/Camera3D.h"
#include "../include/Drawable3D.h"
#include "../include/Render3DPipeline.h"
#include "../include/RenderTarget.h"

namespace GGE
{

Render3DPass::Render3DPass()
    : pipeline(std::make_unique<Render3DPipeline>())
{
}

Render3DPass::~Render3DPass() = default;

void Render3DPass::render(const Camera3D& camera, const std::vector<Drawable3D*>& objects)
{
    render(camera, objects, nullptr);
}

void Render3DPass::render(const Camera3D& camera, const std::vector<Drawable3D*>& objects, RenderTarget* target)
{
    if (!pipeline)
        return;

    pipeline->beginFrame(camera, target);
    pipeline->draw(objects);
    pipeline->endFrame();
}

const Renderer3D::FrameStats& Render3DPass::getLastStats() const
{
    return pipeline->getLastStats();
}

}