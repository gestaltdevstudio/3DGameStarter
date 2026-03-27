#ifndef RENDERPIPELINE_H_INCLUDED
#define RENDERPIPELINE_H_INCLUDED

#include <vector>

namespace GGE
{

class Camera2D;
class Shader;
class Drawable;

class RenderTarget;

class RenderPipeline
{
public:
    virtual ~RenderPipeline() = default;

    virtual void beginFrame(const Camera2D& camera, Shader& shader, RenderTarget* target = nullptr) = 0;
    virtual void draw(Drawable* drawable) = 0;
    virtual void draw(const std::vector<Drawable*>& drawables) = 0;
    virtual void endFrame() = 0;
};

}

#endif // RENDERPIPELINE_H_INCLUDED
