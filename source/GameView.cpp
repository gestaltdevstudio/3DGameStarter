#include "../include/GameView.h"
#include "../include/Render2DPass.h"
#include "../include/Render3DPass.h"
#include "../include/Text.h"
#include "../include/Camera3D.h"
#include "../include/GraphicsManager.h"
#include "../include/GraphicsUtils.h"

namespace GGE
{

    GameView::GameView(Screen* screen, GameModel* model)
        : View(screen), gameModel(model)
    {
    }

    // Computa as UVs normalizadas das 6 faces do cubo a partir de uma região do atlas.
    // Cada face usa 4 vértices ordenados BL, BR, TR, TL conforme o VBO do cubo.
    std::vector<glm::vec2> GameView::buildCubeFaceUVs(
        const AtlasRegion* region, float atlasW, float atlasH)
    {
        const float u0 = region->x / atlasW;
        const float u1 = (region->x + region->width) / atlasW;
        const float v0 = region->y / atlasH;
        const float v1 = (region->y + region->height) / atlasH;

        const glm::vec2 bl(u0, v1), br(u1, v1), tr(u1, v0), tl(u0, v0);
        std::vector<glm::vec2> uvs;
        uvs.reserve(24);
        for (int face = 0; face < 6; ++face)
        {
            uvs.push_back(bl);
            uvs.push_back(br);
            uvs.push_back(tr);
            uvs.push_back(tl);
        }
        return uvs;
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
//        quad = new Drawable2D();
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
        gm->setViewport({0, 0, SCREEN_X, SCREEN_Y});
        gm->getCurrentBatch()->init(20);

        // Configura pipeline de render 2D no gerenciador. A GameView não controla o ciclo GL.
        gm->set2DPipeline(new Render2DPass(1920, 1080));
        gm->set3DPipeline(new Render3DPass());

        // === 3D: câmera + shader + cubo girando ===
        sh3D = new Shader();
        const resourceFile* v3s = Resources::getInstance()->loadCompressedFile("3DShader.vert");
        const resourceFile* f3s = Resources::getInstance()->loadCompressedFile("3DShader.frag");
        sh3D->setShaderID(GraphicsUtils::loadShaders(v3s, f3s));
        delete v3s;
        delete f3s;

        Camera3D cam3D;
        cam3D.setPosition(glm::vec3(0.0f, 1.5f, 4.0f));
        cam3D.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
        cam3D.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
        cam3D.setPerspective(45.0f, (float)SCREEN_X / (float)SCREEN_Y, 0.1f, 100.0f);
        gm->set3DCamera(cam3D);

        std::vector<Vector3> cubeVerts = {
            // Front (+Z)
            Vector3(-0.5f, -0.5f,  0.5f), Vector3( 0.5f, -0.5f,  0.5f),
            Vector3( 0.5f,  0.5f,  0.5f), Vector3(-0.5f,  0.5f,  0.5f),
            // Back (-Z)
            Vector3( 0.5f, -0.5f, -0.5f), Vector3(-0.5f, -0.5f, -0.5f),
            Vector3(-0.5f,  0.5f, -0.5f), Vector3( 0.5f,  0.5f, -0.5f),
            // Left (-X)
            Vector3(-0.5f, -0.5f, -0.5f), Vector3(-0.5f, -0.5f,  0.5f),
            Vector3(-0.5f,  0.5f,  0.5f), Vector3(-0.5f,  0.5f, -0.5f),
            // Right (+X)
            Vector3( 0.5f, -0.5f,  0.5f), Vector3( 0.5f, -0.5f, -0.5f),
            Vector3( 0.5f,  0.5f, -0.5f), Vector3( 0.5f,  0.5f,  0.5f),
            // Bottom (-Y)
            Vector3(-0.5f, -0.5f, -0.5f), Vector3( 0.5f, -0.5f, -0.5f),
            Vector3( 0.5f, -0.5f,  0.5f), Vector3(-0.5f, -0.5f,  0.5f),
            // Top (+Y)
            Vector3(-0.5f,  0.5f,  0.5f), Vector3( 0.5f,  0.5f,  0.5f),
            Vector3( 0.5f,  0.5f, -0.5f), Vector3(-0.5f,  0.5f, -0.5f)
        };
        AtlasRegion* cubeRegion = atlas->regions.at("dogAnim2");
        float u0 = static_cast<float>(cubeRegion->x) / atlas->width;
        float u1 = static_cast<float>(cubeRegion->x + cubeRegion->width) / atlas->width;
        // stbi_set_flip_vertically_on_load(false): v cresce para baixo igual ao y do atlas.
        // v0 = borda superior do sprite, v1 = borda inferior.
        float v0 = static_cast<float>(cubeRegion->y) / atlas->height;
        float v1 = static_cast<float>(cubeRegion->y + cubeRegion->height) / atlas->height;
        // Por face: vértices y=-0.5 (fundo) → v1; vértices y=+0.5 (topo) → v0
        // Ordem por face: BL, BR, TR, TL
        std::vector<glm::vec2> cubeUVs = {
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Front
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Back
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Left
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Right
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Bottom
            glm::vec2(u0, v1), glm::vec2(u1, v1), glm::vec2(u1, v0), glm::vec2(u0, v0), // Top
        };
        std::vector<unsigned short> cubeIdx = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9,10, 8,10,11,
           12,13,14,12,14,15,
           16,17,18,16,18,19,
           20,21,22,20,22,23
        };

        cube3D = new Drawable3D(sh3D);
        cube3D->loadGraphics(cubeVerts, cubeIdx, cubeUVs);
        cube3D->setTexture(atlas->textureID);
        cube3D->setVisible(true);
        Vector4 cubeColor = {1.0f, 1.0f, 1.0f, 1.0f};
        cube3D->setColor(cubeColor);
        cube3D->setPosition({0.0f, 0.0f, 0.0f});
        cube3D->setAlpha(0.5f);
        cube3D->setScale({1.5f, 1.5f, 1.5f});
        cubeAngle = 0.0f;
        gm->addGraphicsObject3D("cube", cube3D);
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
        std::vector<Drawable2D*> drawList;
        Drawable2D* quad = nullptr;

        if (playerSprite && !playerSprite->getCurrentAnimationName().empty())
        {
            quad = playerSprite->getCurrentAnimation()->getCurrentDrawable(
                playerSprite->getAnimationPlayMode()
            );
        }
        else if (playerSprite)
        {
            quad = reinterpret_cast<Drawable2D*>(playerSprite);
        }

        if (quad && quad->isVisible())
            drawList.push_back(quad);

        // Sincroniza o frame atual da animação com as UVs do cubo.
        // buildCubeFaceUVs() é barato (24 vec2) e updateUV() usa GL_DYNAMIC_DRAW.
        if (cube3D && quad)
        {
            AtlasRegion* region  = quad->getAtlasRegion();
            TextureAtlas* ta     = quad->getTextureAtlas();
            if (region && ta)
            {
                cube3D->updateUV(buildCubeFaceUVs(
                    region,
                    static_cast<float>(ta->width),
                    static_cast<float>(ta->height)
                ));
            }
        }

        // Add text if visible - but render separately since it uses different shader
//        if (t && t->isVisible())
//            drawList.push_back(t);

        // Enfileirar drawables no GraphicsManager.
        GraphicsManager* gm = GraphicsManager::getInstance();

        // === Rotacionar cubo ===
        if (cube3D)
        {
            cubeAngle += deltaTime * 4.2f;
            cube3D->setOrientation(Quaternion(0.0f, 1.0f, 0.0f, glm::radians(cubeAngle)));
        }

        // 1. Renderizar camada 2D primeiro (sprite dog + texto) no backbuffer via FBO
        gm->clear2DDrawables();
        for (Drawable2D* d : drawList)
            gm->add2DDrawable(d);
        gm->render2DDrawables(camera, *shader);
        if (t && t->isVisible())
            gm->renderText(t, &camera);

        // 2. Renderizar cubo 3D por cima (só limpa depth, preserva cor 2D)
        gm->render3DFrame();
    }

    void GameView::finishView()
    {
        GraphicsManager::getInstance()->removeGraphicsObject3D("cube");
        delete cube3D;
        delete sh3D;
        delete playerSprite;
        delete shader;
        delete atlas;
        delete f;
        delete textSh;
        delete t;
    }

}
