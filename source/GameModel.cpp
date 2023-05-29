#include "../include/GameModel.h"
#if defined(__WIN32__)
#include <algorithm>
#endif

namespace GGE
{

    GameModel::~GameModel()
    {

    }

    void GameModel::initModel()
    {

        // player = new Entity("Player");
        // player->setDimension(80, 170);
        // player->setPosition(-SCREEN_X/2 + 200, 300);
        // Graphics::getInstance()->addEntity(player);


        // floor = new Entity("Floor");
        // floor->setDimension(1920, 150);
        // floor->setPosition(0, -SCREEN_Y/2 + floor->getDimension()->y/2);
//        Graphics::getInstance()->addEntity(floor);


        playerVelocity.x = playerVelocity.y = 0;

        jumpingTimer = 0;
        jumping = false;

    }

    void GameModel::step(float deltaTime)
    {

    }

    void GameModel::interpolate(float alpha)
    {

    }

    void GameModel::checkCollision()
    {
        
    }


    void GameModel::finishModel()
    {

        delete player;
        delete floor;

    }

}
