#ifndef RENDER3DPASS_H_INCLUDED
#define RENDER3DPASS_H_INCLUDED

#include "Drawable3D.h"
#include <memory>
#include <vector>

#include "Renderer3D.h"

namespace GGE
{

class Camera3D;
class Render3DPipeline;
class RenderTarget;

class Render3DPass
{
public:
    Render3DPass();
    ~Render3DPass();

    void render(const Camera3D& camera, const std::vector<Drawable3D*>& objects);
    void render(const Camera3D& camera, const std::vector<Drawable3D*>& objects, RenderTarget* target);

    const Renderer3D::FrameStats& getLastStats() const;

private:
    std::unique_ptr<Render3DPipeline> pipeline;
};

}

#endif // RENDER3DPASS_H_INCLUDED