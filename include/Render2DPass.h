#ifndef RENDER2DPASS_H_INCLUDED
#define RENDER2DPASS_H_INCLUDED

#include <vector>
#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif defined(_WIN64)
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include "OS_GLFW.h"
  #include <glad/glad.h>
 #else
  #include "OS_iOS.h"
 #endif
#else
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#endif

namespace GGE
{

class Render2DPipeline;
class TextureRenderTarget;
class RenderTarget;
class Camera2D;
class Shader;
class Drawable2D;

// Render2DPass = pipeline de renderização 2D pronto para usar
// Gerencia: FBO, pipeline, batch rendering, composição para tela
class Render2DPass
{
public:
    Render2DPass(int width = 1920, int height = 1080);
    ~Render2DPass();

    // Renderizar lista de drawables para o FBO
    void render(const Camera2D& camera, Shader& shader, 
                const std::vector<Drawable2D*>& drawables);

    // (opcional) renderizar para target customizado
    void render(const Camera2D& camera, Shader& shader,
                const std::vector<Drawable2D*>& drawables,
                RenderTarget* target);

    // Copiar FBO para tela (blit)
    void presentToScreen();

    TextureRenderTarget* getTarget() const { return target; }

private:
    int width;
    int height;
    Render2DPipeline* pipeline = nullptr;
    TextureRenderTarget* target = nullptr;
};

}

#endif // RENDER2DPASS_H_INCLUDED
