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


#ifndef __REFONTBASE_H__
#define __REFONTBASE_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REObject.h"
#include "../../recore/REArray.h"
#include "../RETTFFontChar.h"

class __RE_PUBLIC_CLASS_API__ REFontBase : public REObject
{
private:
	static REBOOL IsBasesApproximatelyEqual(REFontBase * firstBase, REFontBase * secondBase);
	
protected:
	REFontBase();
	virtual ~REFontBase();
	
public:
	REArray<RETTFFontChar*> * charsArray;
	REString path;
	REString familyName;
	REString styleName;
	
	REFloat32 height;
	REFloat32 gammaCorectionValue;
	REUInt32 heightPX;
	REBOOL isUseGammaCorection;
	REBOOL isCached;
	
	REBOOL IsLoaded() const;
	
	REBOOL IsApproximatelyEqual(REFontBase * base);
	
	/* REObject */
	virtual void OnReleased();
	
	static REFontBase * Create();
};


#endif /* __REFONTBASE_H__ */


