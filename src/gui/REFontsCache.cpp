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


#include "../../include/regui/private/REFontsCache.h"
#include "../../include/regui/RETTFFontLoader.h"

__RE_PUBLIC_CLASS_API__ REArray<REFontBase *> REFontsCache::_cache;

void REFontsCache::Deleted(REFontBase * base)
{
	for (REUInt32 i = 0; i < _cache.Count(); i++) 
	{
		if (((REFontBase *)_cache[i])->IsEqual(base)) 
		{
			_cache.RemoveAt(i);
			return;
		}
	}
}

REFontBase * REFontsCache::Get(REFontBase * base)
{
	if (base == NULL) { return NULL; }
	
	if (base->heightPX == 0) 
	{
		base->heightPX = RETTFFontLoader::GetFontHeightInPixels(base->height);
	}
	
	for (REUInt32 i = 0; i < _cache.Count(); i++) 
	{
		REFontBase * b = (REFontBase *)_cache[i];
		if (b->IsApproximatelyEqual(base)) { return b; }
	}
	
	REData data;
	if (!data.InitFromPath(base->path)) { return NULL; }
		
	RETTFFontLoader loader(data, base->height);
	if ( !loader.IsCanLoad() ) { return NULL; }
	
	REArray<RETTFFontChar*> * newChars = new REArray<RETTFFontChar*>(loader.GetGlyphsCount() + 2);
	if (newChars) 
	{
		if (loader.LoadChars(newChars)) 
		{
			base->charsArray = newChars;
			base->heightPX = loader.GetFontHeightInPixels();
			base->familyName.Set(loader.GetFamilyName());
			base->styleName.Set(loader.GetStyleName());
			base->isUseGammaCorection = loader.IsUseGammaCorection();
			base->gammaCorectionValue = loader.GetGammaCorectionValue();
			if (_cache.Add(base))
			{
				base->isCached = true;
				return base;
			}
		}
		else { delete newChars; }
	}
	
	return NULL;
}

REFontsCache::REFontsCache()
{
	
}

REFontsCache::~REFontsCache()
{
	
}

