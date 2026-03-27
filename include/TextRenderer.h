#ifndef TEXTRENDERER_H_INCLUDED
#define TEXTRENDERER_H_INCLUDED

#include "Camera2D.h"
#include <vector>
#include <glm/glm.hpp>

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