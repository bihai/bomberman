#include <sstream>
#include "sound.h"
#include "FMOD/fmod.h"
#include "debugout.h"

namespace snd {


FSOUND_SAMPLE* snds[SND__COUNT];

bool Snd_Load()
{
    if (FSOUND_GetVersion() < FMOD_VERSION) {
		std::ostringstream streamTemp;
		streamTemp << "Wrong FMOD dll version. Expected version " << FMOD_VERSION << ".";
        output(streamTemp.str());
        return false;
    }

    if (!FSOUND_Init(32000, 48, 0)) {
        output("Internal sound sub-system error.");
		return false;
    }

    snds[SND_EXPLOSION] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/explode.wav", 0,0,0);
    snds[SND_TICKTACK] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/ticktack.wav", 0,0,0);
    snds[SND_PWRUP] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/powerup.wav", 0,0,0);
    snds[SND_PWRUPTAKEN] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/poweruptaken.wav", 0,0,0);

    return true;
}

void Snd_Unload()
{
    FSOUND_Sample_Free(snds[SND_EXPLOSION]);
    FSOUND_Sample_Free(snds[SND_TICKTACK]);
    FSOUND_Sample_Free(snds[SND_PWRUP]);
    FSOUND_Sample_Free(snds[SND_PWRUPTAKEN]);
}

void Snd_Play(int sound)
{
    //FSOUND_PlaySound(FSOUND_FREE, snds[sound]);
}

}

