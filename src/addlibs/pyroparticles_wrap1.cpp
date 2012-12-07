

#ifndef __PYROPARTICLES_WRAP1_CPP__
#define __PYROPARTICLES_WRAP1_CPP__



#include "pyroparticles_config.h"

#ifdef __RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__

#ifdef __RE_OS_WINDOWS__
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif

#include "pyro_particles/SDK/Samples/Common/EmitterList.cpp"

#if defined(__RE_USING_DIRECTX9__)
#include "pyro_particles/SDK/Samples/Common/PyroD3D9.cpp"
#elif defined(__RE_USING_OPENGL__)
#include "pyro_particles/SDK/Samples/Common/PyroOpenGL.cpp"
#elif defined(__RE_USING_OPENGL_ES__)
#include "pyro_particles/SDK/Samples/Common/PyroOpenGLES.cpp"
#endif


#endif


#endif

