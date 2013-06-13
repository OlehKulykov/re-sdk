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

/// Class using for managing memory. Like plaine C functions. 
//TODO: may be optimize speed for architectures...
class __RE_PUBLIC_CLASS_API__ REMem 
{
public:
	/// Free 'ptr' alocated memory.
	static void Free(void * ptr)
	{
		free(ptr);
	}
	
	/// Copy 'source' memory to 'dest' with 'num' size of bytes.
	static void * Memcpy(void * dest, const void * source, const size_t num)
	{
		return memcpy(dest, source, num);
	}
	
	/// Move memory from 'source0' to 'dest0' with 'length' size.
	static void * Memmove(void * dest0, const void * source0, const size_t length)
	{
		return memmove(dest0, source0, length);
		/*
		char * dest = (char *)dest0;
		const char * source = (char*)source0;
		if (source < dest)
			for (source += length, dest += length; length; --length)
				*--dest = *--source;
		else if (source != dest)
		{
			for (; length; --length)
				*dest++ = *source++;
		}
		return dest0;
		*/
	}
	
	/// Settes to 'ptr' memory values from 'value' with memory size 'num' in bytes.
	static void * Memset(void * ptr, const int value, const size_t num)
	{
		return memset(ptr, value, num);
	}
	
	/// Alocates 'size' bytes of memory.
	static void * Malloc(const size_t size)
	{
		return malloc(size);
	}
	
	/// Locates aligned 'size' bytes of memory with 'alignment' param.
	static void * MallocAlign(const size_t size, const size_t alignment)
	{
#ifdef __RE_USING_POSIX_MEMALIGN__
		void * m = NULL;
		if (posix_memalign((void**)&m, alignment, size) == 0) return m;
#endif
		return REMem::Malloc(size);
	}
	
	/// Alocates 'size' bytes of memory and settes this new memory with zeros.
	static void * MallocZeros(const size_t size)
	{
		void * m = malloc(size);
		if (m) { REMem::Memset(m, 0, size); }
		return m;
	}
	
	/// Locates aligned 'size' bytes of memory with 'alignment' param and settes this new memory with zeros.
	static void * MallocAlignZeros(const size_t size, const size_t alignment)
	{
#ifdef __RE_USING_POSIX_MEMALIGN__
		void * m = NULL;
		if (posix_memalign((void**)&m, alignment, size) == 0) 
		{
			REMem::Memset(m, 0, size);
			return m;
		}
#endif
		return REMem::MallocZeros(size);
	}
};


#endif /* __REMEM_H__ */

