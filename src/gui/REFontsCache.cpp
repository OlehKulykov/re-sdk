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

void REFontsCache::deleted(REFontBase * base)
{
	for (REUInt32 i = 0; i < _cache.count(); i++) 
	{
		if (((REFontBase *)_cache[i])->isEqual(base)) 
		{
			_cache.removeAt(i);
			return;
		}
	}
}

REFontBase * REFontsCache::get(REFontBase * base)
{
	if (base == NULL) { return NULL; }
	
	if (base->heightPX == 0) 
	{
		base->heightPX = RETTFFontLoader::getFontHeightInPixels(base->height);
	}
	
	for (REUInt32 i = 0; i < _cache.count(); i++) 
	{
		REFontBase * b = (REFontBase *)_cache[i];
		if (b->isApproximatelyEqual(base)) { return b; }
	}
	
	REData data;
	if (!data.initFromPath(base->path)) { return NULL; }
	
	RETTFFontLoader loader(data, base->height);
	if ( !loader.isCanLoad() ) { return NULL; }
	
	REArray<RETTFFontChar*> * newChars = new REArray<RETTFFontChar*>(loader.getGlyphsCount() + 2);
	if (newChars) 
	{
		if (loader.loadChars(newChars)) 
		{
			base->charsArray = newChars;
			base->heightPX = loader.getFontHeightInPixels();
			base->familyName = loader.getFamilyName();
			base->styleName = loader.getStyleName();
			base->isUseGammaCorection = loader.isUseGammaCorection();
			base->gammaCorectionValue = loader.getGammaCorectionValue();
			if (_cache.add(base))
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

