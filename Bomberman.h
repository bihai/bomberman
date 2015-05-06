#ifndef __BOMBERMAN_H
#define __BOMBERMAN_H

#include "map.h"
#include "bomb.h"
#include "player.h"


cMap &getMapRef();
cBombContainer &getBombsRef();
cPlayer &getPlayerRef(int idx);

#endif /* ifndef __BOMBERMAN_H */
