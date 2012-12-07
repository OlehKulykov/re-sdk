/*
 * Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * Support: pyro-support@fenomen-games.com
 *
 */

#ifndef _PYRO_H_INCLUDED
#define _PYRO_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

#if defined(WIN32) && !defined(PYROLIB_STATIC)

#ifdef PYROLIB_COMPILATION
#define PYROAPI __declspec(dllimport)
#else
#define PYROAPI __declspec(dllexport)
#endif

#else
#define PYROAPI
#endif

#define PYRO_MAKE_VERSION(Major, MinorHi,  MinorLo)   (((Major) << 16) | ((MinorHi) << 8) | (MinorLo))

#define PYRO_SDK_VERSION    PYRO_MAKE_VERSION(1, 1, 15)

#include "PyroTypes.h"
#include "PyroGraphics.h"
#include "PyroInterfaces.h"

PyroParticles::IPyroParticleLibrary PYROAPI *CreateParticleLibrary(
	PyroDWord Version,
	PyroDWord Flags = 0
);

void PYROAPI DestroyParticleLibrary(PyroParticles::IPyroParticleLibrary *pParticleLibrary);

#endif /* _PYRO_H_INCLUDED */
