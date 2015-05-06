#ifndef __DEBUGOUT_H
#define __DEBUGOUT_H

#include <string>
#include "settings.h"

using std::string;

void output(string message);
#ifdef DEBUG_MODE
    #define debugoutput(msg) output(msg)
#else
    #define debugoutput(msg)
#endif

#endif /* ifndef __DEBUGOUT_H */
