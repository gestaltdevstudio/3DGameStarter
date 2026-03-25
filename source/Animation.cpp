#include "../include/Animation.h"
#if defined(__WIN32__)
#include <algorithm>
#endif

namespace GGE
{


    Animation::Animation()
    {
        elapsedTime = 0;
    }

    Animation::~Animation()
    {
        for (unsigned int j=0;j<frames.size();j++)
        {
                Drawable* drawable = frames[j];
                frames[j] = 0;
                delete drawable;
        }
    }

    void Animation::loadFrames(TextureAtlas *_textureAtlas, Shader *_shader, float _frameDuration, std::vector<std::string> _framesNames)
    {

        shader = _shader;
        textureAtlas = _textureAtlas;
        frameDuration = _frameDuration;
        for (const std::string& name : _framesNames) {
            Drawable* drawable = new Drawable();
            drawable->loadRegion(name, textureAtlas);
            drawable->setShader(shader);
            drawable->setIsVisible(true); // frames individuais devem ser visíveis
            drawable->setX(0.0f);
            drawable->setY(0.0f);
            drawable->setScaleX(1.0f);
            drawable->setScaleY(1.0f);
            drawable->setAlpha(1.0f);
            frames.push_back(drawable);
        }

    }

    Drawable* Animation::getCurrentDrawable(AnimationPlayMode playMode)
    {
        unsigned int frameIndex = (int) (elapsedTime / frameDuration);
        switch(playMode)
        {
            case ANIM_NORMAL:

                frameIndex = std::min((int)frames.size() - 1, (int) frameIndex);

            break;
            case ANIM_LOOP:

                frameIndex = frameIndex % frames.size();

            break;
            case ANIM_LOOP_PINGPONG:

                frameIndex = frameIndex % ((frames.size() * 2) - 2);
                if (frameIndex >= frames.size()) frameIndex = frames.size() - 2 - (frameIndex - frames.size());

            break;
        }
        return frames[frameIndex];
    }

}
