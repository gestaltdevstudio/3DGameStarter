#ifndef GAMEVIEW_H_INCLUDED
#define GAMEVIEW_H_INCLUDED

#include "View.h"
#include "TextureAtlas.h"
#include "Sprite.h"
#include "Text.h"
#include "Button.h"
#include "GameModel.h"
#include "Camera2D.h"
#include "Drawable3D.h"

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
            //GraphicsObject *go;
            TextureAtlas *atlas;
            Shader *shader, *sh3D;
            Sprite *playerSprite;
            Sprite *s2;
            Shader *textSh;
            Font *f;
            Text *t;
            Button *exitButton;
//            Drawable *quad;
            float w;
            bool animationsPaused;

            Camera2D camera;

            Drawable3D* cube3D = nullptr;
            float cubeAngle = 0.0f;

            // Computa as UVs para as 6 faces do cubo a partir de uma região do atlas.
            static std::vector<glm::vec2> buildCubeFaceUVs(
                const AtlasRegion* region, float atlasW, float atlasH);

#if !defined(GGE_DESKTOP)
            Button *leftButton;
            Button *rightButton;
            Button *jumpButton;
#endif

    };

}

#endif // GAMEVIEW_H_INCLUDED
