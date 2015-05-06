#include <vector>
#include <lua.hpp>
#include "player.h"
#include "math.h"
#include "bomberman.h"
#include "commons.h"
#include "debugout.h"
#include "bomb.h"
#include "sound.h"

cPlayer::cPlayer()
:   direction(0),
    desiredDirection(0),
    collideHappened(0),
    bombBlowLength(1),
    bombsLeft(1),
    controller(HUMAN),
    winCount(0)
{
}

void cPlayer::tick(double milliseconds)
{
    this->bombsLeft += getBombsRef().getRejuvenateCount(this->playerId);

    cMap &map = getMapRef();
    cRect &mapBounds = map.getBounderies();

    int nx = int(this->x - mapBounds.x1);
    int ny = int(this->y - mapBounds.y1);

    if(map(nx, ny).object == cMap::PWRUP_BOMB)
    {
        snd::Snd_Play(snd::SND_PWRUPTAKEN);
        map(nx, ny).object = cMap::NOTHING;
        this->bombsLeft++;
    }
    else if(map(nx, ny).object == cMap::PWRUP_BLOW)
    {
        snd::Snd_Play(snd::SND_PWRUPTAKEN);
        map(nx, ny).object = cMap::NOTHING;
        this->bombBlowLength++;
    }

    if(this->direction != this->desiredDirection)
    {
        while(this->desiredDirection < 0)
        {
            this->desiredDirection += 360;
        }
        while(this->desiredDirection > 360)
        {
            this->desiredDirection -= 360;
        }

        float gap = this->desiredDirection - this->direction;

        if(gap < -180)
        {
            gap = 360 + gap;
        }
        if(gap > 180)
        {
            gap = -360 + gap;
        }

        if((gap < - 1.0) || (gap > 1.0))
        {
            this->direction += gap/300 * milliseconds;
        }
        else
        {
            this->direction = this->desiredDirection;
        }
    }

    if(this->controller == AI)
    {

    }
}

void cPlayer::dropBomb()
{
    if(this->bombsLeft > 0)
    {

        cMap &map = getMapRef();
        cRect &mapBounds = map.getBounderies();

        float nx = this->x - mapBounds.x1;
        float ny = this->y - mapBounds.y1;

        if(map((int)nx, (int)ny).walkable)
        {
            this->bombsLeft --;
            cBombContainer &bombs = getBombsRef();
            bombs.create((int)nx, (int)ny, this->bombBlowLength, playerId);
        }
    }
}

void cPlayer::move(float relativeX, float relativeY, bool recursive)
{
    float nx = this->x + relativeX;
    float ny = this->y + relativeY;
    float moveAmount = (relativeX != 0 ? fabs(relativeX) : fabs(relativeY));

    // Collision testing
    bool colTL = false, colTR = false, colBL = false, colBR = false, colMap = false;
    this->collideHappened = false;

    // ... with map objects

    if(collisionAt(nx + this->collisionBox.x1, ny - this->collisionBox.y1))  // Top left
    {
        colTL = true;
    }
    if(collisionAt(nx + this->collisionBox.x2, ny - this->collisionBox.y1))  // Top right
    {
        colTR = true;
    }
    if(collisionAt(nx + this->collisionBox.x1, ny - this->collisionBox.y2))  // Bottom left
    {
        colBL = true;
    }
    if(collisionAt(nx + this->collisionBox.x2, ny - this->collisionBox.y2))  // Bottom right
    {
        colBR = true;
    }


    // ... with map bounds
    cMap &map = getMapRef();
    cRect &mapBounds = map.getBounderies();

    if(nx + this->collisionBox.x1 / 2 < mapBounds.x1)
    {
        colMap = true;
    }
    else if(nx + this->collisionBox.x2 / 2 > mapBounds.x2)
    {
        colMap = true;
    }
    if(ny + this->collisionBox.y1 / 2 < mapBounds.y1)
    {
        colMap = true;
    }
    else if(ny + this->collisionBox.y2 / 2 > mapBounds.y2)
    {
        colMap = true;
    }

    if(colTL || colTR || colBL || colBR || colMap)
    {
        this->collideHappened = true;

        if((! recursive) && (! colMap))
        {
            if(relativeY != 0)
            {
                if((colBR && ! colBL) || (colTR && ! colTL))
                {
                    move(-moveAmount, 0, true);
                }
                else if((colBL && ! colBR) || (colTL && ! colTR))
                {
                    move(moveAmount, 0, true);
                }
            }
            else
            {
                if((colTL && ! colBL) || (colTR && ! colBR))
                {
                    move(0, -moveAmount, true);
                }
                else if((colBL && ! colTL) || (colBR && ! colTR))
                {
                    move(0, moveAmount, true);
                }
            }
        }
    }
    else
    {
        this->x += relativeX;
        this->y += relativeY;
    }
}

void cPlayer::place(float absoluteX, float absoluteY)
{
    this->x = absoluteX;
    this->y = absoluteY;
}

float cPlayer::getX()
{
    return this->x;
}

float cPlayer::getY()
{
    return this->y;
}

void cPlayer::setDirection(float direction)
{
    this->direction = direction;
    this->desiredDirection = direction;
}

void cPlayer::setDesiredDirection(float direction)
{
    this->desiredDirection = direction;
}

void cPlayer::changeDirection(float directionChange)
{
    this->desiredDirection += directionChange;
}

float cPlayer::getDirection()
{
    return this->direction;
}

void cPlayer::setCollisionBox(cRect collisionBox)
{
    this->collisionBox = collisionBox;
}

cRect cPlayer::getCollisionBox()
{
    return this->collisionBox;
}

int cPlayer::getWinCount()
{
    return this->winCount;
}

void cPlayer::setWinCount(int count)
{
    this->winCount = count;
}

void cPlayer::setPlayerId(int playerId)
{
    this->playerId = playerId;
}

cPlayer::eController cPlayer::getController()
{
    return this->controller;
}

void cPlayer::setController(eController controller)
{
    this->controller = controller;
}

bool cPlayer::getCollideHappened()
{
    return this->collideHappened;
}

bool cPlayer::collisionAt(float x, float y)
{
    bool collision = false;

    cMap &map = getMapRef();
    cRect &mapBounds = map.getBounderies();
    cBombContainer &bombs = getBombsRef();

    float nx = x - mapBounds.x1;
    float ny = y - mapBounds.y1;

    if(! map(nx, ny).walkable)
    {
        if(bombs(nx, ny).playerId != this->playerId)
        {
            collision = true;
        }
    }

    return collision;
}
