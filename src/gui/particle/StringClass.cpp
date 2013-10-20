/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
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

/* $ Revision 1.4 $ */

//#include "StdAfx.H"

#ifndef ENGINE_FLASH

#define __ISO_C_VISIBLE 1999
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "StringClass.h"

#ifdef _WIN32
#define vsnprintf _vsnprintf
#ifdef WINCE
#define stricmp _stricmp
#endif /* WINCE */
#else
#define stricmp strcasecmp
#endif /* _WIN32 */

#endif /* ENGINE_FLASH */
