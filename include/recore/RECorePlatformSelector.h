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


#ifndef __RECOREPLATFORMSELECTOR_H__
#define __RECOREPLATFORMSELECTOR_H__



/*   Detect building platform 32bit/64bit  */
#if ( (!defined(__RE_32BIT_PLATFORM__)) && (!defined(__RE_64BIT_PLATFORM__)) )

#if ( defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_AMD64_) || defined(_M_AMD64) )
#define __RE_64BIT_PLATFORM__
#endif /* Detect 64bit AMD64 */

#ifndef __RE_64BIT_PLATFORM__
#if ( defined(__LP64__) || defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) )
#define __RE_64BIT_PLATFORM__
#endif /* Detect 64bit Intel Architecture-64 */
#endif 

#ifndef __RE_64BIT_PLATFORM__
#if ( defined(_WIN64) || defined(__X86_64__) || defined(WIN64) || defined(_LP64) || defined(ppc64) || defined(x86_64) )
#define __RE_64BIT_PLATFORM__
#endif /* Detect 64bit common defines */
#endif 

#ifndef __RE_64BIT_PLATFORM__
#if ( defined(__x86_64__) || defined(__ppc64__) )
#define __RE_64BIT_PLATFORM__
#endif /* Detect 64bit common defines */
#endif 


#ifndef __RE_64BIT_PLATFORM__
#define __RE_32BIT_PLATFORM__
#endif /* if not 64bit than select 32bit */

#endif



#endif /* __RECOREPLATFORMSELECTOR_H__ */


