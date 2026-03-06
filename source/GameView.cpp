#include "../include/GameView.h"
#include "../include/GraphicsManager.h"

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
    }

    void GameView::step(float deltaTime)
    {

            GraphicsManager* gm = GraphicsManager::getInstance();
            gm->beginFrame();
            gm->getCurrentBatch()->begin();
            gm->getCurrentBatch()->setShader(shader);
            shader->bind();

            Drawable *quad;
            if (!playerSprite->getCurrentAnimationName().empty())
            {
                if (!animationsPaused) {
                    playerSprite->getCurrentAnimation()->update(deltaTime);
                }
                quad = playerSprite->getCurrentAnimation()->getCurrentDrawable(playerSprite->getAnimationPlayMode());
            }
            else
            {
                quad = reinterpret_cast<Drawable*>(playerSprite);
            }


//        if (quad->isVisible()) {


            const AtlasRegion* r = quad->getAtlasRegion();

            InstanceData2D       inst;
            inst.position     = glm::vec2 { quad->getX(), quad->getY() };
            inst.size        = glm::vec2 { quad->getScaleX() * r->width, quad->getScaleY() * r->height };
            inst.size.x *= quad->isFlippedX() ? -1 : 1;
            inst.size.y *= quad->isFlippedY() ? -1 : 1;
            inst.rotation     = quad->getRotation();
            inst.z            = quad->getZ();
            inst.textureIndex = gm->getCurrentBatch()->getTextureSlot(quad->getTextureAtlas()->textureID);

            const TextureAtlas* t = quad->getTextureAtlas();
            float u0 = (float) r->x / t->width;
            float v0 = (float) r->y / t->height;

            float u1 = (float) (r->x + r->width)  / t->width;
            float v1 = (float) (r->y + r->height) / t->height;

            inst.uv       = glm::vec4 { u0, v0, u1 - u0, v1 - v0};
            float *color = quad->getColor();
            inst.color    = glm::vec4 { color[0], color[1], color[2], color[3]};
            inst.alpha    = quad->getAlpha();


            shader->setMat4("u_ViewProj", GraphicsUtils::buildViewProj(camera));

            gm->submit(inst);

            gm->getCurrentBatch()->flush();
            gm->endFrame();

GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        // Tratamento de erro (ex: imprimir no console)
        std::cout << "OpenGL error: " << err << std::endl;
    }

//        }

//        GraphicsManager::getInstance()->renderFrame(deltaTime);
    }

    void GameView::finishView()
    {
        delete playerSprite;
        delete shader;
        delete atlas;
    }

}
