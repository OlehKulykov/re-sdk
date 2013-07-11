/*
 *   Copyright 2012 - 2013 Kulykov Oleh
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#ifndef __REMEM_H__
#define __REMEM_H__

#include "RECommonHeader.h"

/// Class using for managing memory.
class __RE_PUBLIC_CLASS_API__ REMem 
{
public:
	/// Locates aligned 'size' bytes of memory with 'alignment' param.
	static void * mallocAlign(const size_t size, const size_t alignment)
	{
#if defined(HAVE_FUNCTION_POSIX_MEMALIGN)
		void * m = NULL;
		if (posix_memalign((void**)&m, alignment, size) == 0) 
		{
			return m;
		}
#endif
		return malloc(size);		
	}
	
	/// Alocates 'size' bytes of memory and settes this new memory with zeros.
	static void * mallocZeros(const size_t size)
	{
		void * m = malloc(size);
		if (m) 
		{
			memset(m, 0, size);
		}
		return m;
	}
	
	/// Locates aligned 'size' bytes of memory with 'alignment' param and settes this new memory with zeros.
	static void * mallocAlignZeros(const size_t size, const size_t alignment)
	{
#if defined(HAVE_FUNCTION_POSIX_MEMALIGN)
		void * m = NULL;
		if (posix_memalign((void**)&m, alignment, size) == 0) 
		{
			memset(m, 0, size);
			return m;
		}
#endif
		return mallocZeros(size);		
	}
};


#endif /* __REMEM_H__ */

