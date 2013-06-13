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


#ifndef __RECORESETTINGS_H__
#define __RECORESETTINGS_H__


#include "RECoreOSSelector.h"
#include "../reconfig/REConfigSelector.h"



#if defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) && defined(__RE_OS_WINDOWS__) 
#	if defined(_MSC_VER) 
#		define __RE_PUBLIC_CLASS_API__ __declspec(dllexport) 
#	elif defined(__GNUC__) 
#		define __RE_PUBLIC_CLASS_API__ __attribute__((dllexport)) 
#	endif 
#elif defined(__USING_RECORE_DYNAMIC_LIBRARY__) && defined(__RE_OS_WINDOWS__)
#	if defined(_MSC_VER) 
#		define __RE_PUBLIC_CLASS_API__ __declspec(dllimport) 
#	elif defined(__GNUC__) 
#		define __RE_PUBLIC_CLASS_API__ __attribute__((dllimport))  
#	endif 
#endif 


#if __GNUC__ >= 4 
#	define __RE_PUBLIC_CLASS_API__ __attribute__ ((visibility("default"))) 
#endif 


#ifndef __RE_PUBLIC_CLASS_API__
#define __RE_PUBLIC_CLASS_API__
#endif


#if 0
#ifdef __RE_OS_WINDOWS__ 
#  ifdef __BUILDING_RECORE_LIBRARY__ 
#    ifdef __GNUC__ 
#      ifdef __RECORE_STATIC_LIBRARY__ 
#        define __RE_PUBLIC_CLASS_API__ 
#      else
#        define __RE_PUBLIC_CLASS_API__ __attribute__((dllexport))
#      endif /* __RECORE_STATIC_LIBRARY__ */
#    else 
#      ifdef __RECORE_STATIC_LIBRARY__ 
#        define __RE_PUBLIC_CLASS_API__
#      else
#        define __RE_PUBLIC_CLASS_API__ __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#      endif /* __RECORE_STATIC_LIBRARY__ */
#    endif /* __GNUC__ */
#  else 
#    ifdef __GNUC__ 
#      ifdef __RECORE_STATIC_LIBRARY__ 
#        define __RE_PUBLIC_CLASS_API__ 
#      else
#        define __RE_PUBLIC_CLASS_API__ __attribute__((dllimport)) 
#      endif /* __RECORE_STATIC_LIBRARY__ */
#    else 
#      ifdef __RECORE_STATIC_LIBRARY__ 
#        define __RE_PUBLIC_CLASS_API__ 
#      else
#       define __RE_PUBLIC_CLASS_API__ __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#      endif /* __RECORE_STATIC_LIBRARY__ */
#    endif /* __GNUC__ */ 
#  endif /* __BUILDING_RECORE_LIBRARY__ */ 
#  define __RE_LOCAL_CLASS_API__ 
#else 
#  if __GNUC__ >= 4 
#    define __RE_PUBLIC_CLASS_API__ __attribute__ ((visibility("default"))) 
#    define __RE_LOCAL_CLASS_API__  __attribute__ ((visibility("hidden"))) 
#  else 
#    define __RE_PUBLIC_CLASS_API__ 
#    define __RE_LOCAL_CLASS_API__ 
#  endif 
#endif 
#endif


#ifdef _MSC_VER
// http://msdn.microsoft.com/en-us/library/83ythb65%28v=vs.100%29.aspx
#define __RE_BEFORE_DECLARATOR_ALIGNMENT__(a) __declspec(align(a))
#define __RE_AFTER_DECLARATOR_ALIGNMENT__(a) 
#else
#  ifdef __GNUC__
// http://www.delorie.com/gnu/docs/gcc/gcc_62.html
#    define __RE_BEFORE_DECLARATOR_ALIGNMENT__(a) 
#    define __RE_AFTER_DECLARATOR_ALIGNMENT__(a) __attribute__((aligned(a)))
#  else
#    if defined(__IBMC__) || defined(__IBMCPP__)
// http://publib.boulder.ibm.com/infocenter/comphelp/v7v91/index.jsp?topic=%2Fcom.ibm.vacpp7a.doc%2Flanguage%2Fref%2Fclrc03type_attr_aligned.htm
#      define __RE_BEFORE_DECLARATOR_ALIGNMENT__(a) 
#      define __RE_AFTER_DECLARATOR_ALIGNMENT__(a) __attribute__((__aligned__(a)))
#    else
#      error "__RE_BEFORE_DECLARATOR_ALIGNMENT__ and __RE_AFTER_DECLARATOR_ALIGNMENT__ must be declared for current compiller."
#    endif
#  endif
#endif


/// _GLIBCXX_DEBUG -> Undefined by default. When defined, compiles user code using the debug mode. 

/// _GLIBCXX_DEBUG_PEDANTIC -> Undefined by default. When defined while compiling with the debug mode, 
/// makes the debug mode extremely picky by making the use of libstdc++ extensions and libstdc++-specific behavior into errors. 

#if ( (defined(DEBUG) && (DEBUG != 0)) || defined(_DEBUG) || defined(_GLIBCXX_DEBUG) || defined(_GLIBCXX_DEBUG_PEDANTIC) )
/* DEBUG MODE */
#ifndef __RE_DEBUG_MODE__
#define __RE_DEBUG_MODE__
//#warning "DEBUG MODE"
#endif /* Define __RE_DEBUG_MODE__ */

#ifdef __RE_RELEASE_MODE__
#undef __RE_RELEASE_MODE__
#endif /* UNDefine __RE_RELEASE_MODE__ if defined somewhere else */

#else 
/* NOT DEBUG MODE */
#if ( defined(NDEBUG) || defined(_NDEBUG) || (defined(DEBUG) && (DEBUG == 0)) )

#ifndef __RE_RELEASE_MODE__
#define __RE_RELEASE_MODE__
//#warning "RELEASE MODE"
#endif /* Define __RE_RELEASE_MODE__ */

#ifdef __RE_DEBUG_MODE__
#undef __RE_DEBUG_MODE__
#endif /* UNDefine __RE_DEBUG_MODE__ if defined somewhere else */

#endif /* RELEASE MODE */
#endif /* Checking DEBUG/RELEASE MODE */


#ifdef __RE_DEBUG_MODE__
#define __RE_ASSERT__(expr) if(!(expr)){printf("\nASSERT FAILED '%s' in func '%s' line %d file '%s'",#expr,__PRETTY_FUNCTION__,__LINE__,__FILE__);abort();}
#else
#define __RE_ASSERT__(expr)    
#endif

#endif /* __RECORESETTINGS_H__ */

