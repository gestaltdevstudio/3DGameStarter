#include "../include/Renderer2D.h"
#include "../include/RendererBatch2D.h"
#include "../include/Camera2D.h"
#include "../include/Shader.h"
#include "../include/GraphicsUtils.h"

namespace GGE
{

    void Renderer2D::Execute(
        RendererBatch2D& batch,
        const Camera2D& camera,
        Shader& shader)
    {
        shader.bind();
        shader.setMat4("u_ViewProj", GraphicsUtils::buildViewProj(camera));

        batch.flush();

        shader.unbind();
    }

}
