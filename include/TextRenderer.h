#ifndef TEXTRENDERER_H_INCLUDED
#define TEXTRENDERER_H_INCLUDED

#include "Camera2D.h"
#include <vector>
#include <glm/glm.hpp>
#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif defined(__WIN32__)
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
    class Text;

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();

    void render(Text* text, Camera2D* camera);

private:
    void updateBuffers(Text* text);
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> UVs;
};

}

#endif // TEXTRENDERER_H_INCLUDED