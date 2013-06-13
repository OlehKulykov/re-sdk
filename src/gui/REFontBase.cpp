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


#include "../../include/regui/private/REFontBase.h"
#include "../../include/regui/private/REFontsCache.h"

REBOOL REFontBase::IsBasesApproximatelyEqual(REFontBase * firstBase, REFontBase * secondBase)
{
	const REInt32 range = (REInt32)firstBase->heightPX - (REInt32)secondBase->heightPX;
	if (firstBase->heightPX < 10)
	{
		if (range != 0) { return false; }
	}
	else if ((firstBase->heightPX >= 10) && (firstBase->heightPX < 18))
	{
		if ((range < -1) || (range > 1)) { return false; }
	}
	else if ((firstBase->heightPX >= 18) && (firstBase->heightPX < 34))
	{
		if ((range < -2) || (range > 2)) { return false; }
	}
	else if (firstBase->heightPX != secondBase->heightPX)
	{
		return false;
	}
	
	if ( !firstBase->path.IsEqual(secondBase->path) )
	{
		return false;
	}
	
	return true;
}

REBOOL REFontBase::IsApproximatelyEqual(REFontBase * base)
{
	if (base) 
	{
		return REFontBase::IsBasesApproximatelyEqual(this, base);
	}
	return false;
}

REBOOL REFontBase::IsLoaded() const
{
	return (charsArray != NULL);
}

REFontBase::REFontBase() : REObject(),
	charsArray(NULL),
	height(0.0f),
	gammaCorectionValue(0.0f),
	heightPX(0),
	isUseGammaCorection(false),
	isCached(false)
{

}

void REFontBase::OnReleased()
{
	if (isCached) 
	{
		REFontsCache c;
		c.Deleted(this);
		isCached = false;
	}
	
	if (charsArray) 
	{
		for (REUInt32 i = 0; i < charsArray->Count(); i++) 
		{
			RETTFFontChar * loadedChar = (*charsArray)[i];
			delete loadedChar;
		}
		delete charsArray;
		charsArray = NULL;
	}
	
	REObject::OnReleased();
}

REFontBase::~REFontBase()
{
	
}

REFontBase * REFontBase::Create()
{
	REFontBase * newBase = new REFontBase();
	return newBase;
}


