#ifndef __PLAYER_H
#define __PLAYER_H

#include <lua.hpp>
#include "geometry.h"

class cPlayer {
    public:
        enum eController
        {
            HUMAN, AI
        };

        cPlayer();

        void move(float relativeX, float relativeY, bool recursive = false);
        void place(float absoluteX, float absoluteY);

        void tick(double milliseconds);

        float getX();
        float getY();

        void dropBomb();

        void setDirection(float direction);
        void setDesiredDirection(float direction);
        void changeDirection(float directionChange);
        float getDirection();

        void setCollisionBox(cRect collisionBox);
        cRect getCollisionBox();
        bool getCollideHappened();

        void setPlayerId(int playerId);
        eController getController();
        void setController(eController controller);

        int getWinCount();
        void setWinCount(int count);

        int ctrl_up;
        int ctrl_down;
        int ctrl_left;
        int ctrl_right;
        int ctrl_bomb;

    private:
        bool collisionAt(float x, float y);

        float x;
        float y;

        float direction;
        float desiredDirection;

        cRect collisionBox;
        bool collideHappened;

        int bombBlowLength;
        int bombsLeft;

        int playerId;
        eController controller;

        int winCount;


};

#endif /* ifndef __PLAYER_H */
