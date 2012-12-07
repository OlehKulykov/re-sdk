

#ifndef __ADD_LIB_PYROPARTICLES_H__
#define __ADD_LIB_PYROPARTICLES_H__



#include "pyroparticles_config.h"

#ifdef __RE_USING_ADITIONAL_PYRO_PARTICLES_LIBRARY__

#include "pyro_particles/SDK/Include/Pyro.h"
#include "pyro_particles/SDK/Include/PyroGraphics.h"
#include "pyro_particles/SDK/Include/PyroInterfaces.h"
#include "pyro_particles/SDK/Include/PyroTypes.h"
#include "pyro_particles/SDK/Samples/Common/EmitterList.h"

#if defined(__RE_USING_DIRECTX9__)
#include "pyro_particles/SDK/Samples/Common/PyroD3D9.h"
#elif defined(__RE_USING_OPENGL__)
#include "pyro_particles/SDK/Samples/Common/PyroOpenGL.h"
#elif defined(__RE_USING_OPENGL_ES__)
#include "pyro_particles/SDK/Samples/Common/PyroOpenGLES.h"
#endif



#endif


#endif

