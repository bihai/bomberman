/**************************************
*                                     *
*   BOMBERMAN HRUUIIIIIIIIIIIIIIII    *
*                                     *
**************************************/

#include <vector>
#include <windows.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "settings.h"
#include "OGL.h"
#include "map.h"
#include "player.h"
#include "model.h"
#include "debugout.h"
#include "bomb.h"
#include "commons.h"
#include "sound.h"
#include "particle.h"
#include "gltext.h"
#include "ai.h"

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

using std::vector;

GL_Window*	g_window;
Keys*		g_keys;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
GLfloat angleX = 0, angleY = 0, angleZ = 0, zoom = 0;
cMap map;
cModel mdlCrate, mdlLandscape, mdlWallblock, mdlBomber[4], mdlBomb, mdlPwrBomb, mdlPwrBlow;
cPlayer player[4];
cBombContainer bombs;

cMap &getMapRef()
{
    return map;
}

cBombContainer &getBombsRef()
{
    return bombs;
}

cPlayer &getPlayerRef(int idx)
{
    return player[idx];
}

BOOL Initialize (GL_Window* window, Keys* keys)
{
	debugoutput("Initialization commencing.");

	g_window	= window;
	g_keys		= keys;

    srand ( time(NULL) );

    snd::Snd_Load();


    glEnable(GL_TEXTURE_2D);
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

	if( (!mdlCrate.loadFile("crate")) ||
        (!mdlWallblock.loadFile("rock")) ||
        (!mdlLandscape.loadFile("landscape")) ||
        (!mdlBomber[0].loadFile("bomber1")) ||
        (!mdlBomber[1].loadFile("bomber2")) ||
        (!mdlBomber[2].loadFile("bomber3")) ||
        (!mdlBomber[3].loadFile("bomber4")) ||
        (!mdlBomb.loadFile("bomb")) ||
        (!mdlPwrBomb.loadFile("powerup_bomb")) ||
        (!mdlPwrBlow.loadFile("powerup_blowlength")))
	{
        return FALSE;
	}

    bombs.init(17, 11);

    map.resize(17, 11);
    map.setBounderies(cRect(-8.5, -5.5, 8.5, 5.5));
    map.setSpotSize(cVertex2d(1.0, 1.0));

	for(int x = 0; x < 17; x ++)
	{
	    for(int y = 0; y < 11; y ++)
	    {
	        map(x, y).walkable = true;
	        map(x, y).blowable = true;
	        if(((x + 1) % 2 == 0) && ((y + 1) % 2) == 0)
	        {
                map(x, y).object = cMap::WALLBLOCK;
                map(x, y).walkable = false;
                map(x, y).blowable = false;
	        }
            else
            {
                if(getRndInt(0, 100) < 65)
                {
                    map(x, y).object = cMap::CRATE;
                    map(x, y).walkable = false;
                }
	        }
	    }
	}

	// make sure starting pos's are clear
	map(0, 0).object = cMap::NOTHING; map(0, 0).walkable = true;
	map(1, 0).object = cMap::NOTHING; map(1, 0).walkable = true;
	map(0, 1).object = cMap::NOTHING; map(0, 1).walkable = true;
	map(0, 10).object = cMap::NOTHING; map(0, 10).walkable = true;
	map(1, 10).object = cMap::NOTHING; map(1, 10).walkable = true;
	map(0, 9).object = cMap::NOTHING; map(0, 9).walkable = true;
	map(16, 10).object = cMap::NOTHING; map(16, 10).walkable = true;
	map(15, 10).object = cMap::NOTHING; map(15, 10).walkable = true;
	map(16, 9).object = cMap::NOTHING; map(16, 9).walkable = true;
	map(16, 0).object = cMap::NOTHING; map(16, 0).walkable = true;
	map(15, 0).object = cMap::NOTHING; map(15, 0).walkable = true;
	map(16, 1).object = cMap::NOTHING; map(16, 1).walkable = true;

    // Set up players
    player[0].setPlayerId(0);
    player[1].setPlayerId(1);
    player[2].setPlayerId(2);
    player[3].setPlayerId(3);

    player[0].place(-8.0, 5.0);
    player[1].place(8.0, 5.0);
    player[2].place(-8.0, -5.0);
    player[3].place(8.0, -5.0);

    player[0].setDirection(45);
    player[1].setDirection(315);
    player[2].setDirection(135);
    player[3].setDirection(225);

    player[0].setCollisionBox(cRect(-0.35, -0.35, 0.35, 0.35));
    player[1].setCollisionBox(cRect(-0.35, -0.35, 0.35, 0.35));
    player[2].setCollisionBox(cRect(-0.35, -0.35, 0.35, 0.35));
    player[3].setCollisionBox(cRect(-0.35, -0.35, 0.35, 0.35));

    player[0].setController(cPlayer::HUMAN);
    player[1].setController(cPlayer::AI);
    player[2].setController(cPlayer::AI);
    player[3].setController(cPlayer::AI);

    // Set up player controls
    player[0].ctrl_up = VK_UP;
    player[0].ctrl_down = VK_DOWN;
    player[0].ctrl_right = VK_RIGHT;
    player[0].ctrl_left = VK_LEFT;
    player[0].ctrl_bomb = VK_RETURN;

    player[1].ctrl_up = 0x57;
    player[1].ctrl_down = 0x53;
    player[1].ctrl_right = 0x44;
    player[1].ctrl_left = 0x41;
    player[1].ctrl_bomb = VK_TAB;

    player[2].ctrl_up = 0x49;
    player[2].ctrl_down = 0x4B;
    player[2].ctrl_right = 0x4C;
    player[2].ctrl_left = 0x4A;
    player[2].ctrl_bomb = VK_SPACE;

    player[3].ctrl_up = VK_NUMPAD8;
    player[3].ctrl_down = VK_NUMPAD5;
    player[3].ctrl_right = VK_NUMPAD6;
    player[3].ctrl_left = VK_NUMPAD4;
    player[3].ctrl_bomb = VK_NUMPAD0;

    // Init particles
    ptl::initParticles();

    // Init fonts
    gltext::init();

    // Init AI
    AI::init();

    debugoutput("Initialization completed.");

	return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
    snd::Snd_Unload();
    gltext::deinit();
    AI::deinit();
}

void Update (DWORD milliseconds)								// Perform Motion Updates Here
{
    static bool firstTime = true;

    if(firstTime)
    {
        firstTime = false;
        milliseconds = (DWORD)0.01;
    }

    // App control
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	// AI
	for(int p = 0; p < 4; p ++)
	{
        if(player[p].getController() == cPlayer::AI)
        {
            AI::tick(p);
        }
	}

	// Player control
	for(int p = 0; p < 4; p ++)
	{
        if((g_keys->keyDown [player[p].ctrl_up] == TRUE) || (AI::getDesire(p) == AI::AI_GOUP))
        {
            player[p].move(0, 0.004 * milliseconds);
            player[p].setDesiredDirection(180);
        }
        if((g_keys->keyDown [player[p].ctrl_down] == TRUE) || (AI::getDesire(p) == AI::AI_GODOWN))
        {
            player[p].move(0, -0.004 * milliseconds);
            player[p].setDesiredDirection(0);
        }
        if((g_keys->keyDown [player[p].ctrl_left] == TRUE) || (AI::getDesire(p) == AI::AI_GOLEFT))
        {
            player[p].move(-0.004 * milliseconds, 0);
            player[p].setDesiredDirection(270);
        }
        if((g_keys->keyDown [player[p].ctrl_right] == TRUE) || (AI::getDesire(p) == AI::AI_GORIGHT))
        {
            player[p].move(0.004 * milliseconds, 0);
            player[p].setDesiredDirection(90);
        }
        if((g_keys->keyDown [player[p].ctrl_bomb] == TRUE) || (AI::getDesire(p) == AI::AI_BOMB))
        {
            player[p].dropBomb();
        }
	}


    // Cam control
    if(g_keys->keyDown [VK_PRIOR] == TRUE)
    {
        angleX += 0.5f;
    }
    if(g_keys->keyDown [VK_NEXT] == TRUE)
    {
        angleX -= 0.5f;
    }
    if(g_keys->keyDown [VK_HOME] == TRUE)
    {
        angleY += 0.5f;
    }
    if(g_keys->keyDown [VK_END] == TRUE)
    {
        angleY -= 0.5f;
    }
    if(g_keys->keyDown [VK_INSERT] == TRUE)
    {
        angleZ += 0.5f;
    }
    if(g_keys->keyDown [VK_DELETE] == TRUE)
    {
        angleZ -= 0.5f;
    }
    if(g_keys->keyDown [VK_ADD] == TRUE)
    {
        zoom += 0.5f;

    }
    if(g_keys->keyDown [VK_SUBTRACT] == TRUE)
    {
        zoom -= 0.5f;
    }
    if(g_keys->keyDown [VK_F12] == TRUE)
    {
        zoom = 0;
        angleX = 0;
        angleY = 0;
        angleZ = 0;
    }
    if(g_keys->keyDown [VK_F11] == TRUE)
    {
        zoom = 0.5;
        angleX = 20;
        angleY = 0;
        angleZ = 0;
    }

    // Tick all players
    for(int i = 0; i < 4; i ++)
    {
        player[i].tick(milliseconds);
    }

    // Tick and check all bombs
    bombs.tick(milliseconds);


    // Tick and check all particles
    ptl::tickParticles();
}

void Draw (void)
{
	static float fpst;
	static int fps, fpsc;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
    glDisable(GL_BLEND);

	glTranslatef (0.0f, 0.0f, -19.0f + zoom);
	glRotatef (70+angleX, 1.0f, 0.0f, 0.0f);
	glRotatef (angleY, 0.0f, 1.0f, 0.0f);
	glRotatef (angleZ, 0.0f, 0.0f, 1.0f);

    glPushMatrix();
    mdlLandscape.draw();

    glTranslatef(-8.0f, 0.0f, 5.0f);
    for(int x = 0; x < 17; x ++) {
        for(int y = 0; y < 11; y ++) {
            switch(map(x, y).object) {
                case cMap::WALLBLOCK:
                    mdlWallblock.draw();
                    break;
                case cMap::CRATE:
                    mdlCrate.draw();
                    break;
                case cMap::BOMB:
                    mdlBomb.draw();
                    break;
                case cMap::PWRUP_BOMB:
                    mdlPwrBomb.draw();
                    break;
                case cMap::PWRUP_BLOW:
                    mdlPwrBlow.draw();
                    break;
                case cMap::NOTHING:
                default:
                    break;
            }
            glTranslatef(0.0f, 0.0f, -1.0f);
        }
        glTranslatef (0.0f, 0.0f, 1.0f * 11);
        glTranslatef (1.0f, 0.0f, 0.0f);
    }

    glPopMatrix();

    for(int i = 0; i < 4; i ++)
    {
        glPushMatrix();
        glTranslatef (player[i].getX(), 0.0f, - player[i].getY());
        glRotatef(player[i].getDirection(), 0.0, 1.0, 0.0);
        mdlBomber[i].draw();
        glPopMatrix();
    }

    glEnable(GL_BLEND);

    gltext::beginOrtho();

    string text;

    glColor3f(1.0, 0.0, 0.0);
    text = "Red: " + intToStr(player[0].getWinCount());
    gltext::print(0, 460, text.c_str());

    glColor3f(0.0, 0.0, 1.0);
    text = "Blue: " + intToStr(player[1].getWinCount());
    gltext::print(110, 460, text.c_str());

    glColor3f(0.5, 0.5, 0.5);
    text = "[TI:ME]";
    gltext::print(270, 460, text.c_str());

    glColor3f(0.0, 1.0, 0.0);
    text = "Green: " + intToStr(player[2].getWinCount());
    gltext::print(405, 460, text.c_str());

    glColor3f(1.0, 1.0, 0.0);
    text = "Yellow: " + intToStr(player[3].getWinCount());
    gltext::print(525, 460, text.c_str());

    glColor3f(1.0, 1.0, 1.0);
    text = intToStr(ptl::getLiveParticles());
    gltext::print(0, 0, text.c_str());

    glColor3f(1.0, 1.0, 1.0);
    text = intToStr(fps);
    gltext::print(270, 0, text.c_str());


    gltext::endOrtho();

    ptl::renderParticles();

	glFlush ();

	fpsc ++;
	if(GetTickCount() > fpst + 1000)
	{
        fps = fpsc;
        fpsc = 0;
        fpst = GetTickCount();
	}
}

