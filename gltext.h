#ifndef __GLTEXT_H
#define __GLTEXT_H

namespace gltext
{

#include <gl/gl.h>

enum eFontFace
{
    FONT_GENERIC,
    FONT__COUNT
};

void init();
void deinit();
void print(GLint x, GLint y, const char *string, int set = 0);
void beginOrtho();
void endOrtho();


}

#endif /* ifndef __GLTEXT_H */
