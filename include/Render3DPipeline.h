#ifndef RENDER3DPIPELINE_H_INCLUDED
#define RENDER3DPIPELINE_H_INCLUDED

#include "Camera3D.h"
#include "Drawable3D.h"
#include "Renderer3D.h"
#include "GLPlatform.h"

#include <vector>

namespace GGE
{
class RenderTarget;

class Render3DPipeline
{
public:
    Render3DPipeline() = default;
    ~Render3DPipeline() = default;

    void beginFrame(const Camera3D& camera, RenderTarget* target = nullptr);
    void draw(Drawable3D* object);
    void draw(const std::vector<Drawable3D*>& objects);
    void endFrame();

    const Renderer3D::FrameStats& getLastStats() const;

private:
    Renderer3D renderer;
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
};

}

#endif // RENDER3DPIPELINE_H_INCLUDED