#ifndef RENDERER2D_H_INCLUDED
#define RENDERER2D_H_INCLUDED

namespace GGE
{

class RendererBatch2D;
class Camera2D;
class Shader;

class Renderer2D
{
public:
    static void Execute(
        RendererBatch2D& batch,
        const Camera2D& camera,
        Shader& shader
    );
};

}

#endif // RENDERER2D_H_INCLUDED
