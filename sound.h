#ifndef __SOUND_H
#define __SOUND_H

namespace snd {

#include "FMOD/fmod.h"


enum {
    SND_EXPLOSION,
    SND_TICKTACK,
    SND_PWRUP,
    SND_PWRUPTAKEN,
    SND__COUNT
};

bool Snd_Load();
void Snd_Play(int sound);
void Snd_Unload();

}

#endif /* ifndef __SOUND_H */
