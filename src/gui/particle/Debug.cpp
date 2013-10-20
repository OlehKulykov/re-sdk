/*
 * Copyright (c) 2002-2006 Fenomen Games.  All Rights Reserved.
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

/* $ Revision 1.3.2 $ */

//#include "StdAfx.H"
#include "Log.h"
#include "Debug.h"

#ifndef ENGINE_FLASH

Engine::CException::CException(const char *pError, ...)
{
        va_list arg_list;
        va_start(arg_list, pError);

        m_Error = CString::FormatStatic(pError, arg_list);

        va_end(arg_list);
}

Engine::CException::~CException()
{
}

#endif /* ENGINE_FLASH */
