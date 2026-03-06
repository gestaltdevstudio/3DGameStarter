#ifndef CAMERA2D_H_INCLUDED
#define CAMERA2D_H_INCLUDED
#pragma once

#include <glm/glm.hpp>

namespace GGE
{

    class Camera2D
    {
    public:
        inline Camera2D() {};

        inline void setPosition(float _x, float _y) { x = _x; y = _y; };
        inline void setZoom(float _z) { zoom = _z; };
        inline void setSize(float w, float h) { width = w; height = h;  };

        inline float getX() const { return x; };
        inline float getY() const { return y; };
        inline float getZoom() const { return zoom; };
        inline float getWidth() const { return width; };
        inline float getHeight() const { return height; };

    private:
        void recalcMatrix();

        float x;
        float y;

        float width;
        float height;


        float zoom;
    };

}


#endif // CAMERA2D_H_INCLUDED
