#include <vector>
#include "map.h"
#include "commons.h"
#include "settings.h"
#include "sound.h"
#include "debugout.h"

cMap::sSpot nullSpot;

cMap::cMap() {
    nullSpot.object = NOTHING;
    nullSpot.walkable = false;
    nullSpot.blowable = false;
}

void cMap::resize(int w, int h)
{
    this->map.resize(w * h);
    this->w = w;
    this->h = h;
}

cMap::sSpot& cMap::operator()(int x, int y) {
    if((x < 0) || (x >= this->w) || (y < 0) || (y >= this->h))
    {
        return nullSpot;
    }
    else
    {
        return this->map[y * this->w + x];
    }
}

cMap::sSpot& cMap::operator()(float x, float y) {
    if((x < 0) || (x >= this->w) || (y < 0) || (y >= this->h))
    {
        return nullSpot;
    }
    else
    {
        return this->map[(int)y * this->w + (int)x];
    }
}

cRect& cMap::getBounderies()
{
    return this->bounderies;
}

void cMap::setBounderies(cRect bounderies)
{
    this->bounderies = bounderies;
}

cVertex2d& cMap::getSpotSize()
{
    return this->spotSize;
}

void cMap::setSpotSize(cVertex2d spotSize)
{
    this->spotSize = spotSize;
}

bool cMap::createPwrUp(int x, int y)
{
    if(this->map[y * this->w + x].object == CRATE)
    {
        if (getRndInt(0, 100) < OPT_CHANCE_PWRUP)
        {
            snd::Snd_Play(snd::SND_PWRUP);

            this->map[y * this->w + x].walkable = true;
            this->map[y * this->w + x].blowable = true;

            int type = getRndInt(0, 100);
            if(type < 50)
            {
                this->map[y * this->w + x].object = cMap::PWRUP_BOMB;
            }
            else
            {
                this->map[y * this->w + x].object = cMap::PWRUP_BLOW;
            }
            return true;
        }
    }
    return false;
}
