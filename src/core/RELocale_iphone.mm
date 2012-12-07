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


#include "../../include/recore/RELocale.h"

#ifdef __RE_OS_IPHONE__

#include <Foundation/Foundation.h>

void RELocale::TryDetectLocale()
{
	NSArray * arr = [NSLocale preferredLanguages];
	if (arr)
	{
		if ([arr count] > 0)
		{
			NSString * currentLanguage = [[arr objectAtIndex:0] lowercaseString];
			if (currentLanguage)
			{
				this->SetCurrentLocaleFromString([currentLanguage UTF8String]);
			}
		}
	}
}

#endif


