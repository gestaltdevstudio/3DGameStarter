#ifndef GAMEVIEW_H_INCLUDED
#define GAMEVIEW_H_INCLUDED

#include "View.h"
#include "TextureAtlas.h"
#include "Sprite.h"
#include "Text.h"
#include "Button.h"
#include "GameModel.h"
#include "GraphicsObjectGL.h"

namespace GGE
{

    class GameView : public View
    {

        public:
            GameView(Screen *_screen, GameModel *_gameModel);
            void finishView();
            void initView();
            void step(float deltaTime);

        protected:
            GameModel *gameModel;
            GraphicsObject *go;
            TextureAtlas *ta;
            Shader *sh, *sh3D;
            Sprite *playerSprite;
            Sprite *s2;
            Shader *textSh;
            Font *f;
            Text *t;
            Button *exitButton;
            float w;

#if !defined(GGE_DESKTOP)
            Button *leftButton;
            Button *rightButton;
            Button *jumpButton;
#endif

    };

}

#endif // GAMEVIEW_H_INCLUDED
