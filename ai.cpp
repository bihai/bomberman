#include <vector>
#include <string>
#include "ai.h"
#include "settings.h"
#include "debugout.h"
#include "commons.h"
#include "bomberman.h"
#include "map.h"
#include "player.h"
#include "geometry.h"
#include "math.h"

namespace AI
{

using std::string;

lua_State* luaVMs[4];
eDesires AIDesire[4];
int activeAI;

bool init()
{
    for(int i = 0; i < 4; i ++)
    {
        luaVMs[i] = lua_open();

        if(luaVMs[i] == NULL)
        {
            return false;
        }

        lua_register(luaVMs[i], "print", l_debugout);
        lua_register(luaVMs[i], "setDesire", l_setDesire);
        lua_register(luaVMs[i], "getRandomInt", l_getRandomInt);
        lua_register(luaVMs[i], "getCollideState", l_getCollideState);
        lua_register(luaVMs[i], "isAtJunction", l_isAtJunction);
        lua_register(luaVMs[i], "l_canWalk", l_canWalkD);

        string filename = OPT_PATH_SCRIPTS + (string)"npc" + OPT_PATHEXT_SCRIPTS;
        luaL_dofile(luaVMs[i], filename.c_str());

        activeAI = i;

        luaL_dostring(luaVMs[i], "init()");
    }

    return true;
}

void deinit()
{
    for(int i = 0; i < 4; i ++)
    {
        if(luaVMs[i] != NULL)
        {
            lua_close(luaVMs[i]);
        }
    }
}

void tick(int idx)
{
    if(luaVMs[idx] != NULL)
    {
        activeAI = idx;
        luaL_dostring(luaVMs[idx], "tick()");
    }
}

eDesires getDesire(int idx)
{
    return AIDesire[idx];
}


int l_debugout(lua_State * luaVM)
{
    string msg = "Script(AI" + intToStr(activeAI) + "): " + lua_tostring(luaVM, -1);
    debugoutput(msg);
    lua_pushnumber(luaVM, 0);
    return 1;
}

int l_setDesire(lua_State * luaVM)
{
    switch(lua_tointeger(luaVM, -1))
    {
        case 0: AIDesire[activeAI] = AI_NOTHING; break;
        case 1: AIDesire[activeAI] = AI_GOUP; break;
        case 2: AIDesire[activeAI] = AI_GODOWN; break;
        case 3: AIDesire[activeAI] = AI_GOLEFT; break;
        case 4: AIDesire[activeAI] = AI_GORIGHT; break;
        case 5: AIDesire[activeAI] = AI_BOMB; break;
        default: break;
    }
    lua_pushnumber(luaVM, 0);
    return 1;
}

int l_getRandomInt(lua_State * luaVM)
{
    lua_pushnumber(luaVM, getRndInt(lua_tointeger(luaVM, -2), lua_tointeger(luaVM, -1)));
    return 1;
}

int l_getCollideState(lua_State * luaVM)
{
    int result = 0;

    if(getPlayerRef(activeAI).getCollideHappened())
    {
        result = 1;
    }

    lua_pushnumber(luaVM, result);
    return 1;
}

int l_isAtJunction(lua_State * luaVM)
{
    int result = 0;

    cMap map = getMapRef();
    cRect &mapBounds = map.getBounderies();
    cPlayer player = getPlayerRef(activeAI);

    float nx = player.getX() - mapBounds.x1;
    float ny = player.getY() - mapBounds.y1;


    // Check if there are more than two openings around
    int openingCount = 0;
    if(map(nx - 1, ny).walkable)
    {
        openingCount ++;
    }
    if(map(nx + 1, ny).walkable)
    {
        openingCount ++;
    }
    if(map(nx, ny + 1).walkable)
    {
        openingCount ++;
    }
    if(map(nx, ny - 1).walkable)
    {
        openingCount ++;
    }

    if(openingCount > 2)
    {
        result = 1;
    }

    lua_pushnumber(luaVM, result);
    return 1;
}

int l_canWalkD(lua_State * luaVM)
{
    int result = 0;

    cMap map = getMapRef();
    cRect &mapBounds = map.getBounderies();
    cPlayer player = getPlayerRef(activeAI);

    float nx = player.getX() - mapBounds.x1;
    float ny = player.getY() - mapBounds.y1;

    switch(lua_tointeger(luaVM, -1))
    {
        case AI_UP:
            if(map(nx, ny + 1).walkable)
            {
                result = 1;
            }
            break;
        case AI_DOWN:
            if(map(nx, ny - 1).walkable)
            {
                result = 1;
            }
            break;
        case AI_LEFT:
            if(map(nx - 1, ny).walkable)
            {
                result = 1;
            }
            break;
        case AI_RIGHT:
            if(map(nx + 1, ny).walkable)
            {
                result = 1;
            }
            break;
        default:
            break;
    }

    lua_pushnumber(luaVM, result);
    return 1;
}

} /* namespace */
