#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED

#include <vector>
#include "Definitions.h"
#include "RendererBatch2D.h"

namespace GGE
{

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

    // resize vindo do GLFW
    void onFramebufferResize(int w, int h);

    // infos úteis
    const RenderSurface& getSurface() const;
    void submit(const InstanceData2D& i);
    RendererBatch2D* getCurrentBatch() { return &batch; }

private:
    RendererBatch2D batch;
    GraphicsManager() = default;

    static GraphicsManager* instance;

    RenderSurface surface;
    Viewport currentViewport;
    unsigned int currentFBO = 0;
};

} // namespace GGE


#endif // GRAPHICSMANAGER_H_INCLUDED
