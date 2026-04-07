#include "../include/Render2DPipeline.h"
#include "../include/GraphicsManager.h"
#include "../include/RendererBatch2D.h"
#include "../include/Camera2D.h"
#include "../include/Shader.h"
#include "../include/Drawable2D.h"
#include "../include/GraphicsUtils.h"

namespace GGE
{

void Render2DPipeline::beginFrame(const Camera2D& camera, Shader& shader, RenderTarget* target)
{
    GraphicsManager* gm = GraphicsManager::getInstance();

    // Notificar GraphicsManager qual framebuffer usar antes de beginFrame()
    if (target)
    {
        gm->bindFramebuffer(target->getFramebuffer());
    }
    else
    {
        gm->bindFramebuffer(0);
    }

    RendererBatch2D* batch = gm->getCurrentBatch();

    gm->beginFrame();
    batch->begin();
    batch->setShader(&shader);

    shader.bind();
    shader.setMat4("u_ViewProj", GraphicsUtils::buildViewProj(camera));
}

void Render2DPipeline::draw(Drawable2D* drawable)
{
    if (!drawable || !drawable->isVisible())
        return;

    GraphicsManager* gm = GraphicsManager::getInstance();
    RendererBatch2D* batch = gm->getCurrentBatch();
    const AtlasRegion* region = drawable->getAtlasRegion();
    const TextureAtlas* atlas = drawable->getTextureAtlas();

    if (!region || !atlas)
        return;

    InstanceData2D inst;
    inst.position = glm::vec2{ drawable->getX(), drawable->getY() };
    inst.size = glm::vec2{
        drawable->getScaleX() * region->width,
        drawable->getScaleY() * region->height
    };
    inst.size.x *= drawable->isFlippedX() ? -1.0f : 1.0f;
    inst.size.y *= drawable->isFlippedY() ? -1.0f : 1.0f;
    inst.rotation = drawable->getRotation();
    inst.z = drawable->getZ();
    inst.textureIndex = batch->getTextureSlot(drawable->getTextureAtlas()->textureID);

    float u0 = static_cast<float>(region->x) / atlas->width;
    float v0 = static_cast<float>(region->y) / atlas->height;
    float u1 = static_cast<float>(region->x + region->width) / atlas->width;
    float v1 = static_cast<float>(region->y + region->height) / atlas->height;
    inst.uv = glm::vec4{ u0, v0, u1 - u0, v1 - v0 };

    float* color = drawable->getColor();
    inst.color = glm::vec4{ color[0], color[1], color[2], color[3] };
    inst.alpha = drawable->getAlpha();

    gm->submit(inst);
}

void Render2DPipeline::draw(const std::vector<Drawable2D*>& drawables)
{
    for (Drawable2D* d : drawables)
        draw(d);
}

void Render2DPipeline::endFrame()
{
    GraphicsManager* gm = GraphicsManager::getInstance();
    RendererBatch2D* batch = gm->getCurrentBatch();

    batch->flush();
    gm->endFrame();
}

}
