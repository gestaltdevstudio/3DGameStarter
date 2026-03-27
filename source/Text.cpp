#include "../include/Text.h"

namespace GGE
{

    Text::Text()
    {
		textToPrint = "";
		textPrinted = "";
    }


    Text::~Text()
    {

		if (textUVBufferID)
			glDeleteBuffers(1, &textUVBufferID);

		if (textVertexBufferID)
			glDeleteTextures(1, &textVertexBufferID);
    }

    void Text::initText(const char * _textName, Font *_textFont, Shader *_shader, std::string _textureRegionName, TextureAtlas* _textureAtlas, int _x, int _y)
    {
        if (_textureAtlas->regions.find(_textureRegionName) == _textureAtlas->regions.end())
        {
//            OS::getInstance()->alert("Can't initiate Text object", _textName);
        }
        else
        {
            textName = _textName;
            shader = _shader;
            textureAtlas = _textureAtlas;
            textureRegionName = _textureRegionName;
            x = _x;
            y = _y;
            font = _textFont;
            textSize = font->getOriginalSize(); // use font defined size


            glGenVertexArrays(1, &vao);

            glGenBuffers(1, &textVertexBufferID);
            glGenBuffers(1, &textUVBufferID);
        }
    }

    void Text::printText(Camera2D* camera)
    {

        AtlasRegion *atlasRegion= static_cast<AtlasRegion*>(textureAtlas->regions.at(textureRegionName));
        glBindVertexArray(vao);

        if (textToPrint.compare(textPrinted) != 0) {
            textPrinted = textToPrint;
            int length = textPrinted.length();

            vertices.clear();
            UVs.clear();
            float letterX = x;
            float baselineY = y;
            float scaleFactor = textSize / static_cast<float>(font->getOriginalSize());
            const float MODEL_SCALE = 1.0f; // test without 0.075

            for (int i = 0; i < length; ++i) {
                unsigned char character = static_cast<unsigned char>(textPrinted[i]);
                FontChar fontChar = font->fontChars[character];

                if (fontChar.width == 0 || fontChar.height == 0) {
                    letterX += fontChar.xadvance * scaleFactor * MODEL_SCALE * scaleX;
                    continue;
                }

                float uv_x = (atlasRegion->x + fontChar.x) / static_cast<float>(textureAtlas->width);
                float uv_y = (atlasRegion->y + fontChar.y) / static_cast<float>(textureAtlas->height);
                float uv_w = fontChar.width / static_cast<float>(textureAtlas->width);
                float uv_h = fontChar.height / static_cast<float>(textureAtlas->height);

                float charXoffset = fontChar.xoffset * scaleFactor * MODEL_SCALE * scaleX;
                float charYOffset = fontChar.yoffset * scaleFactor * MODEL_SCALE * scaleY;
                float charWidth = fontChar.width * scaleFactor * MODEL_SCALE * scaleX;
                float charHeight = fontChar.height * scaleFactor * MODEL_SCALE * scaleY;
                float charAdvance = fontChar.xadvance * scaleFactor * MODEL_SCALE * scaleX;

                float x0 = letterX + charXoffset;
                float x1 = x0 + charWidth;
                float y0 = baselineY - (charYOffset + charHeight);
                float y1 = baselineY - charYOffset;

                glm::vec2 vertex_up_left    = glm::vec2(x0, y1);
                glm::vec2 vertex_up_right   = glm::vec2(x1, y1);
                glm::vec2 vertex_down_right = glm::vec2(x1, y0);
                glm::vec2 vertex_down_left  = glm::vec2(x0, y0);

                vertices.push_back(vertex_up_left);
                vertices.push_back(vertex_down_left);
                vertices.push_back(vertex_up_right);
                vertices.push_back(vertex_down_right);
                vertices.push_back(vertex_up_right);
                vertices.push_back(vertex_down_left);

                glm::vec2 uv_up_left    = glm::vec2(uv_x, uv_y);
                glm::vec2 uv_up_right   = glm::vec2(uv_x + uv_w, uv_y);
                glm::vec2 uv_down_right = glm::vec2(uv_x + uv_w, uv_y + uv_h);
                glm::vec2 uv_down_left  = glm::vec2(uv_x, uv_y + uv_h);

                UVs.push_back(uv_up_left);
                UVs.push_back(uv_down_left);
                UVs.push_back(uv_up_right);
                UVs.push_back(uv_down_right);
                UVs.push_back(uv_up_right);
                UVs.push_back(uv_down_left);

                letterX += charAdvance;
            }

            glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, textUVBufferID);
            glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), UVs.data(), GL_STATIC_DRAW);
        }
        glUseProgram(shader->getShaderID());

        // Set GL state for 2D text rendering
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set MVP based on camera or default
        float camWidth = camera ? camera->getWidth() : 1920.0f;
        float camHeight = camera ? camera->getHeight() : 1080.0f;
        float camX = camera ? camera->getX() : 0.0f;
        float camY = camera ? camera->getY() : 0.0f;
        glm::mat4 projection = glm::ortho(-camWidth/2.0f, camWidth/2.0f, -camHeight/2.0f, camHeight/2.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-camX, -camY, 0.0f));
        glm::mat4 MVP = projection * view;
        unsigned int mvpID = glGetUniformLocation(shader->getShaderID(), "MVP");
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlas->textureID);

        unsigned int uniformID = glGetUniformLocation(shader->getShaderID(), "textTexture");
        glUniform1i(uniformID, 0);


        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, textUVBufferID);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        glDrawArrays(GL_TRIANGLES, 0, vertices.size() );


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    }

}
