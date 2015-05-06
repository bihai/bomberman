#ifndef __AI_H
#define __AI_H

#include <lua.hpp>

namespace AI
{

enum eDesires
{
    AI_NOTHING, AI_GOUP, AI_GODOWN, AI_GOLEFT, AI_GORIGHT, AI_BOMB
};

enum eDirections
{
    AI_NONE, AI_UP, AI_DOWN, AI_LEFT, AI_RIGHT
};

bool init();
void deinit();
void tick(int idx);
eDesires getDesire(int idx);

int l_debugout(lua_State * luaVM);
int l_setDesire(lua_State * luaVM);
int l_getRandomInt(lua_State * luaVM);
int l_getCollideState(lua_State * luaVM);
int l_isAtJunction(lua_State * luaVM);
int l_canWalkD(lua_State * luaVM);


}

#endif /* ifndef __AI_H */
