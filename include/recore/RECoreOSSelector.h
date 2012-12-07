/*
 *   Copyright 2012 Kulykov Oleh
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

/*
 
Posible values for OS: 
 
 __RE_OS_MOBILE__
 __RE_OS_DESKTOP__
 
 __RE_OS_WINDOWS__ 
 __RE_OS_IPHONE__
 __RE_OS_MACOSX__
 __RE_OS_LINUX__
 __RE_OS_ANDROID__
 __RE_OS_BADA__
 
 */

#ifndef __RECOREOSSELECTOR_H__
#define __RECOREOSSELECTOR_H__


#if ( (!defined(__RE_OS_WINDOWS__)) && (!defined(__RE_OS_IPHONE__)) && (!defined(__RE_OS_MACOSX__)) && (!defined(__RE_OS_LINUX__)) && (!defined(__RE_OS_ANDROID__)) && (!defined(__RE_OS_BADA__)) )
/* No manualy selected, try to auto select */

#if (defined(WIN32) || defined(_WIN32) || defined(WIN32_LEAN_AND_MEAN) || defined(_WIN64) || defined(WIN64))

#define __RE_OS_WINDOWS__


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */
#endif /* WIN32_LEAN_AND_MEAN */


#endif /* END CHECKING WINDOWS PLATFORM  */
/***********************************************************************************/



#if (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))

#define __RE_OS_IPHONE__

#endif /* END CHECKING IPHONE PLATFORM */
/***********************************************************************************/



#if (defined(__linux__) || defined(__linux) || defined(linux))

#define __RE_OS_LINUX__


#endif /* END CHECKING LINUX PLATFORM */
/***********************************************************************************/



#if ( defined(ANDROID_NDK) || defined(__ANDROID__) ) 

#define __RE_OS_ANDROID__

#endif /* END CHECKING ANDROID PLATFORM */
/***********************************************************************************/



#if (defined(MACOS_X) || defined(__APPLE__) || defined(__MACOSX__) || defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__))

#ifndef __RE_OS_IPHONE__
#define __RE_OS_MACOSX__
#endif /* NOT IPHONE OS */

#endif	/* END CHECKING MACOSX PLATFORM */
/***********************************************************************************/

#else /* END AUTO SELECTING OS */



#endif




#if (defined(__arm__) || defined(__thumb__) || defined(_ARM)) 
#define __RE_ARCHITECTURE_ARM__
#endif /* ARM architecture detection */




#endif /* __RECOREOSSELECTOR_H__ */


