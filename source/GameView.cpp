#include "../include/GameView.h"
#include "../include/Render2DPass.h"
#include "../include/Text.h"

namespace GGE
{

    GameView::GameView(Screen* screen, GameModel* model)
        : View(screen), gameModel(model)
    {
    }

    void GameView::initView()
    {
        atlas = new TextureAtlas();
        camera.setSize(1920, 1080); // área útil do jogo
        camera.setPosition(0.0f, 0.0f);
        camera.setZoom(1.0f);

        animationsPaused = false;
        const resourceFile* atlasFile =
            Resources::getInstance()->loadCompressedFile("game.atlas");
        const resourceFile* imageFile =
            Resources::getInstance()->loadCompressedFile("game.png");

        atlas->loadTextureAtlas(atlasFile, imageFile);

        delete atlasFile;
        delete imageFile;

        // Load font
        f = new Font();
        const resourceFile* fontFile = Resources::getInstance()->loadCompressedFile("pressStart2P.fnt");
        f->loadFont(fontFile);
        delete fontFile;

        // Load text shader
        textSh = new Shader();
        const resourceFile* tvs = Resources::getInstance()->loadCompressedFile("textShader.vert");
        const resourceFile* tfs = Resources::getInstance()->loadCompressedFile("textShader.frag");
        textSh->setShaderID(GraphicsUtils::loadShaders(tvs, tfs));
        delete tvs;
        delete tfs;

        // Create text
        t = new Text();
        t->initText("testText", f, textSh, "pressStart2P", atlas, -400, 200);  // baseline at center
        t->setText("HELLO");
        t->setVisible(true);

        shader = new Shader();

        const resourceFile* vs =
            Resources::getInstance()->loadCompressedFile("shaderNew2D.vert");
        const resourceFile* fs =
            Resources::getInstance()->loadCompressedFile("shaderNew2D.frag");

        shader->setShaderID(GraphicsUtils::loadShaders(vs, fs));

        delete vs;
        delete fs;


        playerSprite = new Sprite();
        Animation *animation = new Animation();
        std::vector<std::string> frames;
        frames.push_back("dogAnim2");
        frames.push_back("dogAnim3");
        frames.push_back("dogAnim4");
        frames.push_back("dogAnim5");
        frames.push_back("dogAnim6");
        frames.push_back("dogAnim7");
        frames.push_back("dogAnim8");
        frames.push_back("dogAnim9");
        animation->loadFrames(atlas, shader, 1.0/8, frames);
        playerSprite->addAnimation("Idle", animation);
        playerSprite->setCurrentAnimationName("Idle", ANIM_LOOP_PINGPONG);
        playerSprite->setShader(shader);
        playerSprite->setIsVisible(true);
        playerSprite->setEntity(gameModel->getPlayer());
//        Graphics::getInstance()->addSprite("test", playerSprite);




//
//        quad = new Drawable();
//        quad->loadRegion("dogAnim2", atlas);
//
//        quad->setX(0);
//        quad->setY(0);
//        quad->setScaleX( 1.0f);
//        quad->setScaleY( 1.0f);
//        quad->setZ(0);
//        quad->setIsVisible(true);
//        quad->setColor(new float [4] {1.0f, 1.0f, 1.0f, 1.0f});
//        quad->setAlpha(1.0);

        GraphicsManager* gm = GraphicsManager::getInstance();
        gm->getCurrentBatch()->init(20);

        // Configura pipeline de render 2D no gerenciador. A GameView não controla o ciclo GL.
        gm->set2DPipeline(new Render2DPass(1920, 1080));
    }

    void GameView::step(float deltaTime)
    {
        if (!shader)
            return;

        // === FASE 1: Atualizar estado do sprite (do model) ===
        // (lê do model, atualiza posição/visibilidade/z)
        if (!animationsPaused && playerSprite)
        {
            if (!playerSprite->getCurrentAnimationName().empty())
            {
                playerSprite->getCurrentAnimation()->update(deltaTime);
            }
        }

        // === FASE 2: Renderizar (pipeline isolada) ===
        std::vector<Drawable*> drawList;
        Drawable* quad = nullptr;

        if (playerSprite && !playerSprite->getCurrentAnimationName().empty())
        {
            quad = playerSprite->getCurrentAnimation()->getCurrentDrawable(
                playerSprite->getAnimationPlayMode()
            );
        }
        else if (playerSprite)
        {
            quad = reinterpret_cast<Drawable*>(playerSprite);
        }

        if (quad && quad->isVisible())
            drawList.push_back(quad);

        // Add text if visible - but render separately since it uses different shader
//        if (t && t->isVisible())
//            drawList.push_back(t);

        // Enfileirar drawables no GraphicsManager.
        GraphicsManager* gm = GraphicsManager::getInstance();
        gm->clear2DDrawables();
        for (Drawable* d : drawList)
            gm->add2DDrawable(d);

        // Delegar ao GraphicsManager a renderização real (pipeline + FBO).
        gm->render2DDrawables(camera, *shader);

        // Render text separately on top
        if (t && t->isVisible())
            gm->renderText(t, &camera);
    }

    void GameView::finishView()
    {
        delete playerSprite;
        delete shader;
        delete atlas;
        delete f;
        delete textSh;
        delete t;
    }

}
