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


#ifndef __REFONTOBJECT_H__
#define __REFONTOBJECT_H__

#include "../recore/RECommonHeader.h"
#include "REGUIObject.h"
#include "RETTFFontChar.h"

/// Class of font.
class __RE_PUBLIC_CLASS_API__ REFontObject : public REGUIObject
{
private:
	REBOOL PrepareForSetParams();
    REBOOL Load();
	void RecalculateScaleRatio(const REFloat32 needHeight, const REFloat32 loadedHeight);
	static REBOOL FillArrayWithCharsForTextFromFont(REArray<RETTFFontChar*> * charsArray, const REString & text, REArray<RETTFFontChar*> * fontChars);
	
protected:
	void * _base;
	RESize _charScaleRatio;
    REFloat32 _height;

    void SetPath(const REString & newPath);
    void SetHeight(const REFloat32 newHeight);
	
	REFontObject();
	virtual ~REFontObject();
public:	
    /* REObject */
    virtual const REUInt32 GetClassIdentifier() const;
    static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/// Returns height of font.
	const REFloat32 GetHeight() const { return _height; }
	
	/// Returns characters space ratio.
	const RESize & GetCharsScaleRatio() const { return _charScaleRatio; }
	
	/// Returns pointer to array with characters.
	REArray<RETTFFontChar*> * GetChars();
	
	/// Fills array for chars by text.
	REBOOL FillArrayWithCharsForText(REArray<RETTFFontChar*> * charsArray, const REString & text);

	/* REGUIObject */
	virtual void OnPrepareGUIObjectForSetuping();
	virtual void OnSetupingGUIObjectFinished(const REBOOL isAcceptedByParent);
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	
	/// Creates and returns new font object.
	static REFontObject * Create();
	
	/// Creates and returns new font object loaded from path using with height.
	static REFontObject * CreateWithPath(const REString & path, const REFloat32 height);
	
	/// Returns XML key string for font file path.
    static const char * GetXMLPathKeyString();
	
	/// Returns XML key string for font file path.
    static const char * GetXMLHeightKeyString();
	
	/// Returns XML format string for font height.
    static const char * GetXMLHeightFormatString();
};


#endif /* __REFONTOBJECT_H__ */


