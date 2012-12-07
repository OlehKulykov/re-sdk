

#ifndef __OPENAL_WRAP2_C__
#define __OPENAL_WRAP2_C__

#include "openal_config.h"


#if ( defined(__RE_OS_ANDROID__) )

#include "openal_soft/Alc/ALc.c"
#include "openal_soft/Alc/alcConfig.c"
#include "openal_soft/Alc/alcEcho.c"
#include "openal_soft/Alc/alcModulator.c"
#include "openal_soft/Alc/alcReverb.c"
#include "openal_soft/Alc/alcRing.c"
#include "openal_soft/Alc/alcThread.c"
#include "openal_soft/Alc/ALu.c"
#include "openal_soft/Alc/bs2b.c"
#include "openal_soft/Alc/panning.c"
#include "openal_soft/Alc/mixer.c"
#include "openal_soft/Alc/null.c"

#ifdef HAVE_WAVE
#include "openal_soft/Alc/wave.c"
#endif

#ifdef HAVE_ANDROID
#include "openal_soft/Alc/android.c"
#endif

//alsa.c		
//pulseaudio.c
//oss.c		
//solaris.c
//dsound.c	
//portaudio.c	
//winmm.c

#endif 



#endif

