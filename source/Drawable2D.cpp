#include "../include/Drawable2D.h"

namespace GGE
{

    Drawable2D::Drawable2D()
    {
        visible = false;
        scaleX = scaleY = 1.0f;
        x = y = z= 0.0f;
        layer = 0;
        uvbo = 0;
        color = new float[4] {1., 1., 1., 1.};
        flippedX = flippedY = false;
		rotation = 0;
		alpha=1.0;
    }

    Drawable2D::~Drawable2D()
    {
        if (uvbo)
			glDeleteBuffers(1, &uvbo);
    }

    void Drawable2D::loadRegion(std::string regionName, TextureAtlas *_textureAtlas)
    {
        if (_textureAtlas->regions.find(regionName) != _textureAtlas->regions.end())
        {
            textureAtlas = _textureAtlas;
            atlasRegion = static_cast<AtlasRegion*>(textureAtlas->regions.at(regionName));

//            if (uvbo)
//                glDeleteBuffers(1, &uvbo);
//
//            glGenBuffers(1, &uvbo);
//
//            float uv[] = {
//                        (float) (atlasRegion->x + atlasRegion->width) / textureAtlas->width,  1.0f - ((float) atlasRegion->y / textureAtlas->height),
//                        (float) (atlasRegion->x + atlasRegion->width) / textureAtlas->width,  1.0f - (float) (atlasRegion->y + atlasRegion->height) / textureAtlas->height,
//                        (float) atlasRegion->x / textureAtlas->width,                         1.0f - (float) (atlasRegion->y + atlasRegion->height) / textureAtlas->height,
//                        (float) atlasRegion->x / textureAtlas->width,                         1.0f - ((float) atlasRegion->y / textureAtlas->height)
//                    };
//
//            glBindBuffer(GL_ARRAY_BUFFER, uvbo);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
        }

    }

}
