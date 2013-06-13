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


#ifndef __RECOMMONHEADER_H__ 
#define __RECOMMONHEADER_H__ 

#include "RECoreOSSelector.h"
#include "RECoreSettings.h"
#include "RECorePlatformSelector.h"
#include "RECoreIncludes.h"
#include "RETypes.h"

#ifndef RE_SAFE_RELEASE
/// Checks for object is not NULL, then call Release() and sets to NULL.
#define RE_SAFE_RELEASE(o) if(o){o->Release();o=NULL;}
#endif

#ifndef RE_SAFE_DELETE
/// Checks for object is not NULL, then delete's and sets to NULL.
#define RE_SAFE_DELETE(p) if(p){delete p;p=NULL;}
#endif

#ifndef RE_SAFE_FREE
/// Checks for object is not NULL, then free and sets to NULL.
#define RE_SAFE_FREE(p) if(p){free(p);p=NULL;}
#endif

#ifndef RE_SAFE_RETAIN
/// Checks for prev object is not NULL. Release()'s prev objectand sets to NULL.
/// Checks new object is not NULL, than assing and Retain().
#define RE_SAFE_RETAIN(to,from) if(to){to->Release();to=NULL;}if(from){to=from;to->Retain();}
#endif

#endif /* __RECOMMONHEADER_H__ */ 

