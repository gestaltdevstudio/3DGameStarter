#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include "Definitions.h"
#include "RendererBatch2D.h"
#include "TextRenderer.h"

namespace GGE
{
class Text;
class Render2DPass;
class RenderTarget;
class Camera2D;
class Shader;
class Drawable;

struct Viewport
{
    int x;
    int y;
    int width;
    int height;
};

struct RenderSurface
{
    // tamanho REAL do framebuffer (GLFW)
    Point screenSize;

    // área útil (aspect ratio fixo)
    Point renderAreaSize;

    // posição da área útil dentro da tela
    Point renderAreaPosition;

    inline int left()   const { return renderAreaPosition.x; }
    inline int bottom() const { return renderAreaPosition.y; }
    inline int width()  const { return renderAreaSize.x; }
    inline int height() const { return renderAreaSize.y; }
};

class GraphicsManager
{
public:
    static GraphicsManager* getInstance();

    void beginFrame();
    void endFrame();

    // viewport / framebuffer
    void setViewport(const Viewport& vp);
    const Viewport& getViewport() const;

    void bindFramebuffer(unsigned int fbo);

    // render batch API (claro e nítido)
    void clear2DDrawables();
    void add2DDrawable(Drawable* drawable);
    void set2DPipeline(Render2DPass* pipeline);
    void render2DDrawables(const Camera2D& camera, Shader& shader, RenderTarget* target = nullptr);
    void renderText(Text* text, Camera2D* camera);

    // resize vindo do GLFW
    void onFramebufferResize(int w, int h);

    // infos úteis
    const RenderSurface& getSurface() const;
    void submit(const InstanceData2D& i);
    RendererBatch2D* getCurrentBatch() { return &batch; }

private:
    RendererBatch2D batch;
    std::unique_ptr<Render2DPass> active2DPass;
    std::vector<Drawable*> queued2DDrawables;
    TextRenderer textRenderer;

    GraphicsManager() = default;

    static GraphicsManager* instance;

    RenderSurface surface;
    Viewport currentViewport;
    unsigned int currentFBO = 0;
};

} // namespace GGE


#endif // GRAPHICSMANAGER_H_INCLUDED
