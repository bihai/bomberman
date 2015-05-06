#ifndef __BOMB_H
#define __BOMB_H

#include <deque>
#include "geometry.h"
#include "particle.h"

using std::vector;

class cBombContainer
{
    public:
        cBombContainer();

        struct cBomb
        {
            float fuse;
            int blowLength;
            bool free;
            int playerId;
        };

        void init(int w, int h);
        void create(int x, int y, int blowLength, int playerId);
        void tick(double milliseconds);
        void blow(int x, int y);
        bool blowCheckSpot(int x, int y, std::deque<ptl::cParticle> &newPtls);
        int getRejuvenateCount(int playerId);

        cBomb& operator()(int x, int y);
        cBomb& operator()(float x, float y);

    private:
        int rejuvenateCount[4];
        vector<cBomb> bombs;
        int w;
        int h;
};

#endif /* __BOMB_H */
