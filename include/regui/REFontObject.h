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


#ifndef __REFONTOBJECT_H__
#define __REFONTOBJECT_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "RETTFFontChar.h"

/// Class of font.
class __RE_PUBLIC_CLASS_API__ REFontObject : public REObject
{
private:
	REBOOL prepareForSetParams();
    REBOOL load();
	void recalculateScaleRatio(const REFloat32 needHeight, const REFloat32 loadedHeight);
	static REBOOL fillArrayWithCharsForTextFromFont(REArray<RETTFFontChar*> * charsArray, const REString & text, REArray<RETTFFontChar*> * fontChars);
	
protected:
	void * _base;
	RESize _charScaleRatio;
    REFloat32 _height;

    void setPath(const REString & newPath);
    void setHeight(const REFloat32 newHeight);
	
	REFontObject();
	virtual ~REFontObject();
public:	
    /* REObject */
	virtual void onReleased();
	
	/// Returns height of font.
	const REFloat32 getHeight() const { return _height; }
	
	/// Returns characters space ratio.
	const RESize & getCharsScaleRatio() const { return _charScaleRatio; }
	
	/// Returns pointer to array with characters.
	REArray<RETTFFontChar*> * getChars();
	
	/// Fills array for chars by text.
	REBOOL fillArrayWithCharsForText(REArray<RETTFFontChar*> * charsArray, const REString & text);

	/* REGUIObject */
	virtual void onPrepareGUIObjectForSetuping();
	virtual void onSetupingGUIObjectFinished(const REBOOL isAcceptedByParent);
	virtual REBOOL acceptStringParameter(const char * key, const char * value);
	
	/// Creates and returns new font object.
	static REFontObject * create();
	
	/// Creates and returns new font object loaded from path using with height.
	static REFontObject * createWithPath(const REString & path, const REFloat32 height);
	
	/// Returns XML key string for font file path.
    static const char * getXMLPathKeyString();
	
	/// Returns XML key string for font file path.
    static const char * getXMLHeightKeyString();
	
	/// Returns XML format string for font height.
    static const char * getXMLHeightFormatString();
};


#endif /* __REFONTOBJECT_H__ */


