#ifndef RENDER2DPIPELINE_H_INCLUDED
#define RENDER2DPIPELINE_H_INCLUDED

#include "RenderPipeline.h"
#include "RenderTarget.h"

namespace GGE
{

class Render2DPipeline : public RenderPipeline
{
public:
    Render2DPipeline() = default;
    ~Render2DPipeline() override = default;

    void beginFrame(const Camera2D& camera, Shader& shader, RenderTarget* target = nullptr) override;
    void draw(Drawable* drawable) override;
    void draw(const std::vector<Drawable*>& drawables) override;
    void endFrame() override;
};

}

#endif // RENDER2DPIPELINE_H_INCLUDED
