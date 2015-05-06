#include <gl/gl.h>
#include <string>
#include "model.h"
#include "settings.h"
#include "gltext.h"

namespace gltext
{

using std::string;

GLuint displist[FONT__COUNT];
GLuint texture[FONT__COUNT];

void init()
{
    cModel dummyModel;
    texture[FONT_GENERIC] = dummyModel.loadTexture(OPT_PATH_FONTS + (string)"generic" + OPT_PATHEXT_FONTS);

    float cx;
    float cy;

    displist[FONT_GENERIC] = glGenLists(256);
    glBindTexture(GL_TEXTURE_2D, texture[FONT_GENERIC]);

    for (int loop=0; loop<256; loop++)
	{
		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;

		glNewList(displist[FONT_GENERIC]+loop,GL_COMPILE);
            glBegin(GL_QUADS);
				glTexCoord2f(cx,1-cy-0.0625f);
				glVertex2i(0,0);
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);
				glVertex2i(16,0);
				glTexCoord2f(cx+0.0625f,1-cy);
				glVertex2i(16,16);
				glTexCoord2f(cx,1-cy);
				glVertex2i(0,16);
			glEnd();
			glTranslated(10,0,0);
        glEndList();
	}
}

void deinit()
{
    glDeleteLists(displist[FONT_GENERIC],256);
}

void beginOrtho()
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,640,0,480,-1,1);

	glDisable(GL_LIGHTING);
}

void print(GLint x, GLint y, const char *string, int set)
{
	if (set>1)
	{
		set=1;
	}
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslated(x,y,0);
	glListBase(displist[FONT_GENERIC]-32+(128*set));
	glCallLists(strlen(string),GL_UNSIGNED_BYTE,string);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void endOrtho()
{
    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

}
