#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include "Definitions.h"
#include "GLPlatform.h"
#include "Camera3D.h"
#include "Drawable3D.h"
#include "RendererBatch2D.h"
#include "Renderer3D.h"
#include "TextRenderer.h"

namespace GGE
{
class Text;
class Render2DPass;
class Render3DPass;
class RenderTarget;
class Camera2D;
class Shader;
class Drawable2D;

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
    void add2DDrawable(Drawable2D* drawable);
    void set2DPipeline(Render2DPass* pipeline);
    void render2DDrawables(const Camera2D& camera, Shader& shader, RenderTarget* target = nullptr);
    void renderText(Text* text, Camera2D* camera);
    void render3DFrame();
    void set3DPipeline(Render3DPass* pipeline);
    void set3DCamera(const Camera3D& camera);
    Camera3D& get3DCamera();
    const Camera3D& get3DCamera() const;
    void addGraphicsObject3D(const std::string& name, Drawable3D* object);
    void removeGraphicsObject3D(const std::string& name);
    const Renderer3D::FrameStats& get3DStats() const;

    // resize vindo do GLFW
    void onFramebufferResize(int w, int h);

    // infos úteis
    const RenderSurface& getSurface() const;
    void submit(const InstanceData2D& i);
    RendererBatch2D* getCurrentBatch() { return &batch; }

private:
    RendererBatch2D batch;
    Render2DPass* active2DPass = nullptr;
    Render3DPass* active3DPass = nullptr;
    std::vector<Drawable2D*> queued2DDrawables;
    std::vector<Drawable3D*> queued3DObjects;
    TextRenderer textRenderer;
    Camera3D camera3D;
    std::map<std::string, Drawable3D*> registered3DObjects;

    GraphicsManager() = default;

    static GraphicsManager* instance;

    RenderSurface surface;
    Viewport currentViewport;
    unsigned int currentFBO = 0;
};

} // namespace GGE


#endif // GRAPHICSMANAGER_H_INCLUDED
