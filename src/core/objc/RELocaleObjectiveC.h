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


#ifndef __RELOCALEOBJECTIVEC_H__
#define __RELOCALEOBJECTIVEC_H__

#include "../../../include/recore/RELocale.h"

void RELocale::tryDetectLocale()
{
	NSArray * arr = [NSLocale preferredLanguages];
	if (arr)
	{
		if ([arr count] > 0)
		{
			NSString * currentLanguage = [[arr objectAtIndex:0] lowercaseString];
			if (currentLanguage)
			{
				this->setCurrentLocaleFromString([currentLanguage UTF8String]);
			}
		}
	}
}


#endif /* __RELOCALEOBJECTIVEC_H__ */


