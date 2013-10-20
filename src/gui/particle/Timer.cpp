/*
* Copyright (c) 2002-2008 Fenomen Games.  All Rights Reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
* PURPOSE.
*
* REPRODUCTION IN WHOLE OR IN PART IS PROHIBITED WITHOUT THE WRITTEN
* CONSENT OF THE COPYRIGHT OWNER.
*
*/

/* $ Revision 1.1 $ */

//#include "StdAfx.H"
#include "Timer.h"

#ifdef ENGINE_UNIX
#ifndef __RE_OS_WINDOWS__
#include <sys/time.h>
#endif
#endif /* ENGINE_UNIX */


#ifdef ENGINE_MACOSX
#include <CoreAudio/CoreAudio.h>
#endif /* ENGINE_MACOSX */

double Engine::CTimer::GetTime()
{
        return ((double) GetExactTime()) * 0.000001;
}

LONGLONG Engine::CTimer::GetExactTime()
{
        if (!m_bInit)
                return 0;

#ifdef __RE_OS_WINDOWS__
return ((1000 * (LONGLONG)timeGetTime()) - m_Delayed);
#endif

#ifndef __RE_OS_WINDOWS__
#ifdef ENGINE_WIN32
        return 1000 * (LONGLONG) timeGetTime() - m_Delayed;
#elif defined(ENGINE_MACOSX)
        Float64 FTime = (Float64) AudioGetCurrentHostTime();
        FTime /= AudioGetHostClockFrequency();
        FTime *= 1000000.0;
        return ((LONGLONG) FTime) - m_Delayed;
#else
        struct timeval TimeVal;
        gettimeofday(&TimeVal, NULL);

        LONGLONG Time = TimeVal.tv_sec * 1000000 + TimeVal.tv_usec;
        return Time - m_Delayed;
#endif
#endif
}
