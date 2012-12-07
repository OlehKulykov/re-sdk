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

#ifndef _PYRO_TYPES_H_INCLUDED
#define _PYRO_TYPES_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

typedef unsigned int PyroDWord;
typedef signed int PyroResult;
typedef int PyroBool;
typedef unsigned char PyroByte;

#define PyroFalse	0
#define PyroTrue	1

#define PyroOK		0
#define PyroFailed	-1

#define PyroIsFailed(r)		(((PyroResult) (r)) < 0)

namespace PyroParticles
{

class CPyroException
{
	char *m_pMessage;
            
public:

	PYROAPI CPyroException();

	PYROAPI CPyroException(const CPyroException &Exception);

	PYROAPI CPyroException(const char *pError, ...);

	PYROAPI ~CPyroException();
    
	PYROAPI const char *GetExceptionMessage() const;

};

}

#endif /* _PYRO_TYPES_H_INCLUDED */
