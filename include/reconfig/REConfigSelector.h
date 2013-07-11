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


#ifndef __RECONFIGSELECTOR_H__
#define __RECONFIGSELECTOR_H__

#include <recore_sdk_config.h>

#ifdef __RE_OS_WINDOWS__  
#include "REConfigWindows.h"
#endif /* __RE_OS_WINDOWS__ */


#ifdef __RE_OS_IPHONE__  
#include "REConfigIPhone.h"
#endif /* __RE_OS_IPHONE__ */ 


#ifdef __RE_OS_MACOSX__  
#include "REConfigMacosx.h"
#endif /* __RE_OS_MACOSX__ */


#ifdef __RE_OS_LINUX__  
#include "REConfigLinux.h"
#endif /* __RE_OS_LINUX__ */ 


#ifdef __RE_OS_ANDROID__  
#include "REConfigAndroid.h"
#endif /* __RE_OS_ANDROID__ */ 


#ifdef __RE_OS_BADA__
#include "REConfigBada.h"
#endif /* __RE_OS_BADA__  */



#endif /* __RECONFIGSELECTOR_H__ */


