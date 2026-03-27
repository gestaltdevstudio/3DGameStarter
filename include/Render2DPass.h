#ifndef RENDER2DPASS_H_INCLUDED
#define RENDER2DPASS_H_INCLUDED

#include <vector>
#include <memory>

namespace GGE
{

class Render2DPipeline;
class TextureRenderTarget;
class RenderTarget;
class Camera2D;
class Shader;
class Drawable;

// Render2DPass = pipeline de renderização 2D pronto para usar
// Gerencia: FBO, pipeline, batch rendering, composição para tela
class Render2DPass
{
public:
    Render2DPass(int width = 1920, int height = 1080);
    ~Render2DPass();

    // Renderizar lista de drawables para o FBO
    void render(const Camera2D& camera, Shader& shader, 
                const std::vector<Drawable*>& drawables);

    // (opcional) renderizar para target customizado
    void render(const Camera2D& camera, Shader& shader,
                const std::vector<Drawable*>& drawables,
                RenderTarget* target);

    // Copiar FBO para tela (blit)
    void presentToScreen();

    TextureRenderTarget* getTarget() const { return target.get(); }

private:
    std::unique_ptr<Render2DPipeline> pipeline;
    std::unique_ptr<TextureRenderTarget> target;
    int width;
    int height;
};

}

#endif // RENDER2DPASS_H_INCLUDED
