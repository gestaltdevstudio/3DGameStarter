#include "../include/TextRenderer.h"
#include "../include/Text.h"
#include "../include/Camera2D.h"
#include <glm/glm.hpp>

namespace GGE
{

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::render(Text* text, Camera2D* camera)
{
    if (!text || !text->isVisible()) return;

    updateBuffers(text);

    GLuint shaderID = text->getShader()->getShaderID();
    if (shaderID == 0) return; // Shader not loaded

    glUseProgram(shaderID);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float camWidth = camera ? camera->getWidth() : SCREEN_X;
    float camHeight = camera ? camera->getHeight() : SCREEN_Y;
    float camX = camera ? camera->getX() : 0.0f;
    float camY = camera ? camera->getY() : 0.0f;
    glm::mat4 projection = glm::ortho(-camWidth/2.0f, camWidth/2.0f, -camHeight/2.0f, camHeight/2.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camX, -camY, 0.0f));
    glm::mat4 MVP = projection * view;
    unsigned int mvpID = glGetUniformLocation(shaderID, "MVP");
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

    if (!text->getTextureAtlas()) return;

    unsigned int uniformID = glGetUniformLocation(shaderID, "textTexture");
    glUniform1i(uniformID, 0);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (text->vao == 0) return;

    glBindVertexArray(text->vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, text->textVertexBufferID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, text->textUVBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glDrawArrays(GL_TRIANGLES, 0, text->vertices.size() );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void TextRenderer::updateBuffers(Text* text)
{
    if (!text || !text->isVisible()) return;
    if (!text->textureAtlas) return;
    if (text->textToPrint == text->textPrinted) return;

    text->textPrinted = text->textToPrint;
    int length = text->textToPrint.length();

    text->vertices.clear();
    text->UVs.clear();
    float letterX = text->getX();
    float baselineY = text->getY();
    float scaleFactor = text->textSize / static_cast<float>(text->font->getOriginalSize());
    const float MODEL_SCALE = 1.0f;

    AtlasRegion* atlasRegion = static_cast<AtlasRegion*>(text->textureAtlas->regions.at(text->textureRegionName));
    glBindVertexArray(text->vao);

    for (int i = 0; i < length; ++i) {
        unsigned char character = static_cast<unsigned char>(text->textToPrint[i]);
        FontChar fontChar = text->font->fontChars[character];

        if (fontChar.width == 0 || fontChar.height == 0) {
            letterX += fontChar.xadvance * scaleFactor * MODEL_SCALE * text->scaleX;
            continue;
        }

        float uv_x = (atlasRegion->x + fontChar.x) / static_cast<float>(text->textureAtlas->width);
        float uv_y = (atlasRegion->y + fontChar.y) / static_cast<float>(text->textureAtlas->height);
        float uv_w = fontChar.width / static_cast<float>(text->textureAtlas->width);
        float uv_h = fontChar.height / static_cast<float>(text->textureAtlas->height);

        float charXoffset = fontChar.xoffset * scaleFactor * MODEL_SCALE * text->scaleX;
        float charYOffset = fontChar.yoffset * scaleFactor * MODEL_SCALE * text->scaleY;
        float charWidth = fontChar.width * scaleFactor * MODEL_SCALE * text->scaleX;
        float charHeight = fontChar.height * scaleFactor * MODEL_SCALE * text->scaleY;
        float charAdvance = fontChar.xadvance * scaleFactor * MODEL_SCALE * text->scaleX;

        float x0 = letterX + charXoffset;
        float x1 = x0 + charWidth;
        float y0 = baselineY - (charYOffset + charHeight);
        float y1 = baselineY - charYOffset;

        glm::vec2 vertex_up_left    = glm::vec2(x0, y1);
        glm::vec2 vertex_up_right   = glm::vec2(x1, y1);
        glm::vec2 vertex_down_right = glm::vec2(x1, y0);
        glm::vec2 vertex_down_left  = glm::vec2(x0, y0);

        text->vertices.push_back(vertex_up_left);
        text->vertices.push_back(vertex_down_left);
        text->vertices.push_back(vertex_up_right);
        text->vertices.push_back(vertex_down_right);
        text->vertices.push_back(vertex_up_right);
        text->vertices.push_back(vertex_down_left);

        glm::vec2 uv_up_left    = glm::vec2(uv_x, uv_y);
        glm::vec2 uv_up_right   = glm::vec2(uv_x + uv_w, uv_y);
        glm::vec2 uv_down_right = glm::vec2(uv_x + uv_w, uv_y + uv_h);
        glm::vec2 uv_down_left  = glm::vec2(uv_x, uv_y + uv_h);

        text->UVs.push_back(uv_up_left);
        text->UVs.push_back(uv_down_left);
        text->UVs.push_back(uv_up_right);
        text->UVs.push_back(uv_down_right);
        text->UVs.push_back(uv_up_right);
        text->UVs.push_back(uv_down_left);

        letterX += charAdvance;
    }

    glBindBuffer(GL_ARRAY_BUFFER, text->textVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, text->vertices.size() * sizeof(glm::vec2), text->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, text->textUVBufferID);
    glBufferData(GL_ARRAY_BUFFER, text->UVs.size() * sizeof(glm::vec2), text->UVs.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

}
