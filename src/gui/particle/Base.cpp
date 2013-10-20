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


/* Revision 1.2.2 */

//#include "StdAfx.H"
#include "Base.h"

#ifndef ENGINE_FLASH

#ifndef ENGINE_WIN32

LONG InterlockedIncrement(LONG *pValue)
{
        (*pValue) ++;
        return *pValue;
}

LONG InterlockedDecrement(LONG *pValue)
{
        (*pValue) --;
        return *pValue;
}

#endif /* _WIN32 */

#ifdef _WIN32

#include <windows.h>

#ifndef WINCE

LPVOID GetProcAddressW(HINSTANCE hInstance, LPWSTR lpProcName)
{
        char ProcName[512];

        int nChars = WideCharToMultiByte(CP_ACP, 0, lpProcName, wcslen(lpProcName), ProcName, sizeof(ProcName), NULL, FALSE);

        if (nChars < 512)
                ProcName[nChars] = '\0';

        return (LPVOID)GetProcAddress(hInstance, ProcName);
}

#else

long __cdecl clock()
{
        return 0;
}

time_t __cdecl time(time_t *)
{
        return 0;
}

extern "C"
{

int errno = 0;

void perror()
{
}

char *getenv()
{
        return NULL;
}

void abort(int)
{
}

int strcoll(const char *s0, const char *s1) { return strcmp(s0, s1); }


const char *strerror() { return "unknown error"; }

}

#endif /* WINCE */

#endif /* _WIN32 */

#endif /* ENGINE_FLASH */
