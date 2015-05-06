#include <deque>
#include "bomb.h"
#include "settings.h"
#include "map.h"
#include "bomberman.h"
#include "debugout.h"
#include "commons.h"
#include "sound.h"
#include "particle.h"
#include "math.h"

cBombContainer::cBombContainer()
{
}

void cBombContainer::init(int w, int h)
{
    this->w = w;
    this->h = h;
    this->bombs.resize(w * h);

    vector<cBomb>::iterator bombIter;
    for( bombIter = this->bombs.begin(); bombIter != this->bombs.end(); bombIter++ ) {
        bombIter->free = true;
        bombIter->playerId = -1;
    }
}

void cBombContainer::create(int x, int y, int blowLength, int playerId)
{
    if(this->bombs[y * this->w + x].free)
    {
        this->bombs[y * this->w + x].free = false;
        this->bombs[y * this->w + x].blowLength = blowLength;
        this->bombs[y * this->w + x].fuse = OPT_BOMB_FUSETIME;
        this->bombs[y * this->w + x].playerId = playerId;

        cMap &map = getMapRef();

        map(x, y).walkable = false;
        map(x, y).object = cMap::BOMB;

        snd::Snd_Play(snd::SND_TICKTACK);
    }
}


void cBombContainer::tick(double milliseconds)
{
    for(int x = 0; x < this->w; x++)
    {
        for(int y = 0; y < this->h; y++)
        {
            if(! this->bombs[y * this->w + x].free)
            {
                this->bombs[y * this->w + x].fuse -= milliseconds;

                if(this->bombs[y * this->w + x].fuse <= 0)
                {
                    this->blow(x, y);
                }
            }
        }
    }
}

void cBombContainer::blow(int x, int y)
{
    cMap &map = getMapRef();
    cRect &mapBounds = map.getBounderies();

    // Allow player to drop one more bomb now
    this->rejuvenateCount[this->bombs[y * this->w + x].playerId] ++;

    // Play sounds
    snd::Snd_Play(snd::SND_EXPLOSION);

    // Spawn particles
    float mx = x + mapBounds.x1;
    float my = y + mapBounds.y1;
    int blowLength = this->bombs[y * this->w + x].blowLength;
    std::deque<ptl::cParticle> newPtls;

    // Out going
    for(int i = 0; i < OPT_NUM_EXPLO_PTLS / (12/2); i ++)
    {
        ptl::cParticle newPtl(  getRndFloat(0.3, 0.7),
                                1.0, getRndFloat(0.0, 0.5), 0.0,
                                cVertex(mx + 0.5, 0.3, -my - 0.5),
                                cVertex(getRndFloat(-cos(i), cos(i)), getRndFloat(4.0, 10.0), getRndFloat(-sin(i), sin(i))),
                                cVertex(0, -0.1, 0)
                            );
        newPtls.push_back(newPtl);
    }

    // Left/Right going
    for(int i = 0; i < OPT_NUM_EXPLO_PTLS / (12/5); i ++)
    {
        ptl::cParticle newPtl(  getRndFloat(6.0 / blowLength, 6.0 / blowLength),
                                getRndFloat(0.5, 1.0), getRndFloat(0.0, 0.7), 0.0,
                                cVertex(mx + 0.5, 0.3, -my - 0.5),
                                cVertex(getRndFloat(-8.0, 8.0), getRndFloat(-0.1, 0.1), getRndFloat(-0.1, 0.1)),
                                cVertex(0, -0.1, 0)
                            );
        newPtls.push_back(newPtl);
    }

    // Up/Down going
    for(int i = 0; i < OPT_NUM_EXPLO_PTLS / (12/5); i ++)
    {
        ptl::cParticle newPtl(  getRndFloat(6.0 / blowLength, 6.0 / blowLength),
                                getRndFloat(0.5, 1.0), getRndFloat(0.0, 0.7), 0.0,
                                cVertex(mx + 0.5, 0.3, -my - 0.5),
                                cVertex(getRndFloat(-0.1, 0.1), getRndFloat(-0.1, 0.1), getRndFloat(-8.0, 8.0)),
                                cVertex(0, -0.1, 0)
                            );
        newPtls.push_back(newPtl);
    }


    // Remove stuff (crates, aso)
    this->bombs[y * this->w + x].free = true;
    this->bombs[y * this->w + x].playerId = -1;

    map(x, y).walkable = true;
    map(x, y).object = cMap::NOTHING;

    for(int i = x + 1; i <= x + this->bombs[y * this->w + x].blowLength; i ++)
    {
        if(blowCheckSpot(i, y, newPtls) == false)
        {
            break;
        }
    }

    for(int i = x - 1; i >= x - this->bombs[y * this->w + x].blowLength; i --)
    {
        if(blowCheckSpot(i, y, newPtls) == false)
        {
            break;
        }
    }

    for(int j = y + 1; j <= y + this->bombs[y * this->w + x].blowLength; j ++)
    {
        if(blowCheckSpot(x, j, newPtls) == false)
        {
            break;
        }
    }

    for(int j = y - 1; j >= y - this->bombs[y * this->w + x].blowLength; j --)
    {
        if(blowCheckSpot(x, j, newPtls) == false)
        {
            break;
        }
    }


    // Now create them all the particles
    ptl::createParticles(newPtls);
}

cBombContainer::cBomb& cBombContainer::operator()(int x, int y) {
    return this->bombs[y * this->w + x];
}

cBombContainer::cBomb& cBombContainer::operator()(float x, float y) {
    return this->bombs[(int)y * this->w + (int)x];
}

int cBombContainer::getRejuvenateCount(int playerId)
{
    int count = this->rejuvenateCount[playerId];
    this->rejuvenateCount[playerId] = 0;
    return count;
}

bool cBombContainer::blowCheckSpot(int x, int y, std::deque<ptl::cParticle> &newPtls)
{
    cMap &map = getMapRef();
    cRect &mapBounds = map.getBounderies();

    if((x < 0) || (x >= this->w) || (y < 0) || (y >= this->h))
    {
        return false;
    }
    if(map(x, y).object == cMap::BOMB)
    {
        blow(x, y);
    }
    if(map(x, y).blowable)
    {
        int mx = (int)(x + mapBounds.x1);
        int my = (int)(y + mapBounds.y1);
        for(int p = 0; p < OPT_NUM_EXPLO_PTLS / (12/2); p ++)
        {
            ptl::cParticle newPtl(  getRndFloat(0.5, 1.0),
                                    getRndFloat(0.5, 1.0), getRndFloat(0.0, 0.5), getRndFloat(0.0, 0.2),
                                    cVertex(getRndFloat(mx, mx + 1), 0.3, getRndFloat(-my, -my - 1)),
                                    cVertex(getRndFloat(-cos(p), cos(p)), getRndFloat(4.0, 10.0), getRndFloat(-sin(p), sin(p))),
                                    cVertex(0, -0.1, 0)
                                );
            newPtls.push_back(newPtl);
        }
        if(map(x, y).object != cMap::NOTHING)
        {
            if (! map.createPwrUp(x, y))
            {
                map(x, y).walkable = true;
                map(x, y).object = cMap::NOTHING;
            }
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}
