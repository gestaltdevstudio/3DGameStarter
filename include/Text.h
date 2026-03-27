#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "Font.h"
#include "TextureAtlas.h"
#include "Shader.h"
#include "Drawable.h"

namespace GGE
{
    class Font;

    class Text : public Drawable
    {
        friend class TextRenderer;

        public:
            Text();
            ~Text();
            void initText(const char * _textName, Font *_textFont, Shader *shader, std::string _textureRegionName, TextureAtlas* _textureAtlas, int _x, int _y);

            inline void setText(std::string _text)
            {
				if (_text.length() == 0 || _text.compare("") == 0)
				{
					_text = " ";
				}
				textToPrint = _text;
            };

            void printText(Camera2D* camera = nullptr);

            inline void setPosition(int _x, int _y) {x = _x; y = _y; };
            inline void setScaleX(float _x) { scaleX = _x; }
            inline void setScaleY(float _y) { scaleY = _y; }
            inline void setVisible(bool _visible) { visible = _visible; }
            inline bool isVisible() { return visible; }
            inline std::string getName() { return textName; }

            inline Point getPosition() { return {(int)x, (int)y}; }
            inline float getScaleX() { return scaleX; }
            inline float getScaleY() { return scaleY; }
            inline Shader* getShader() { return shader; }
            inline Font* getFont() { return font; }


        protected:
            std::string             textName;
            float                   textSize;
            Font                    *font;
			std::string             textToPrint;
			std::string             textPrinted;
            GLuint                  vao;
            GLuint                  textVertexBufferID;
            GLuint                  textUVBufferID;
            std::vector<glm::vec2>  vertices;
            std::vector<glm::vec2>  UVs;
            std::string             textureRegionName;
            Shader                  *shader;


    };

}

#endif // TEXT_H_INCLUDED
