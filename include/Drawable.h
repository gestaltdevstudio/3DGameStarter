#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include "TextureAtlas.h"
#include "Shader.h"

namespace GGE
{

    class Drawable
    {

    public:
        Drawable();
        virtual ~Drawable();
        void loadRegion(std::string regionName, TextureAtlas *_textureAtlas);

        inline float getX() { return x; };
        inline float getY() { return y; };
        inline float getZ() { return z; };
        inline int getLayer() { return layer; };
        inline float getScaleX() { return scaleX; };
        inline float getScaleY() { return scaleY; };
        inline Shader *getShader() { return shader; };
        inline AtlasRegion* getAtlasRegion() { return atlasRegion; }
        inline TextureAtlas* getTextureAtlas() { return textureAtlas; }
        inline bool isVisible() const { return visible; }

        inline void setX(float _x) { x = _x; };
        inline void setY(float _y) { y = _y; };
        inline void setZ(float _z) { z = _z; };
        inline void setLayer(int _layer) { layer = _layer; };
        inline void setScaleX(float _scaleX) { scaleX = _scaleX; };
        inline void setScaleY(float _scaleY) { scaleY = _scaleY; };
        inline void setShader(Shader *_shader ) { shader = _shader; };

        inline void setIsVisible(bool _visible) { visible = _visible; }
        inline GLuint getUVBuffer() { return uvbo; }
        inline void setRotation(float r) { rotation = r;}
        inline float getRotation() { return rotation; }
        inline void setFlippedX(bool _flippedX) { flippedX = _flippedX; }
        inline bool isFlippedX() { return flippedX; }
        inline void setFlippedY(bool _flippedY) { flippedY = _flippedY; }
        inline bool isFlippedY() { return flippedY; }

        inline float getAlpha() { return alpha; };
        inline void setAlpha(float _alpha) { alpha = _alpha;};

        inline float* getColor() {return color;};
        inline void setColor(float *_color) { color = _color;};


    protected:
        float x;
        float y;
        float z;
        int layer;
        float scaleX;
        float scaleY;
        float rotation;
        Shader *shader;
        AtlasRegion *atlasRegion;
        TextureAtlas *textureAtlas;
        bool visible;
        bool flippedX;
        bool flippedY;
        float alpha;
        float *color;
        GLuint uvbo;
    };
}

#endif // DRAWABLE_H_INCLUDED
