#ifndef __MAP_H
#define __MAP_H

#include <vector>
#include "geometry.h"

using std::vector;

class cMap {
    public:
        cMap();

        enum eObjs {
            NOTHING, WALLBLOCK, CRATE, BOMB, PWRUP_BOMB, PWRUP_BLOW
        };

        struct sSpot {
            eObjs object;
            bool walkable;
            bool blowable;
        };

        void resize(int w, int h);
        sSpot& operator()(int x, int y);
        sSpot& operator()(float x, float y);

        cRect& getBounderies();
        void setBounderies(cRect bounderies);

        cVertex2d& getSpotSize();
        void setSpotSize(cVertex2d spotSize);

        bool createPwrUp(int x, int y);

    private:
        vector<sSpot> map;
        int w;
        int h;

        cRect bounderies;
        cVertex2d spotSize;
};

#endif /* ifndef __MAP_H */
