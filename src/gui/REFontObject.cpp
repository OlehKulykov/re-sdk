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


#include "../../include/regui/REFontObject.h"
#include "../../include/regui/private/REFontBase.h"
#include "../../include/regui/private/REFontsCache.h"
#include "../../include/regui/RETTFFontLoader.h"
#include "../../include/regui/RERenderDevice.h"

#include "../../include/recore/REWideString.h"

#define RE_FONT_XML_HEIGHT_KEY_STRING "heightf"
#define RE_FONT_XML_HEIGHT_FORMAT_STRING "%f"
#define RE_FONT_XML_PATH_KEY_STRING "path"

/* REObject */
const REUInt32 REFontObject::getClassIdentifier() const
{
    return REFontObject::classIdentifier();
}

const REUInt32 REFontObject::classIdentifier()
{
    static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REFontObject");
    return clasIdentif;
}

REBOOL REFontObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REFontObject::classIdentifier() == classIdentifier) ||
			REGUIObject::isImplementsClass(classIdentifier));
}

void REFontObject::recalculateScaleRatio(const REFloat32 needHeight, const REFloat32 loadedHeight)
{
	RERenderDevice * device = RERenderDevice::GetDefaultDevice();
	if (device) 
	{
		RESize deviceRatio(device->GetScreenToRenderSizeRatio());
		const REFloat32 heightRatio = needHeight / loadedHeight;
		_charScaleRatio.width = deviceRatio.width * heightRatio;
		_charScaleRatio.height = deviceRatio.height * heightRatio;
	}
	else
	{
		_charScaleRatio.width = 1.0f;
		_charScaleRatio.height = 1.0f;
	}
}

REBOOL REFontObject::fillArrayWithCharsForTextFromFont(REArray<RETTFFontChar*> * charsArray,
													   const REString & text,
													   REArray<RETTFFontChar*> * fontChars)
{
	REWideString p(text);
	charsArray->setCapacity(p.getLength() + 1);
	const REUInt32 startRight = fontChars->count() - 1;
	const wchar_t * wideText = p.getWideChars();
	wchar_t needChar;
	while ((needChar = *wideText++))
	{
		REUInt32 left = 0;
		REUInt32 right = startRight;
		while (left <= right)
		{
			const REUInt32 middle = (left + right) / 2;
			RETTFFontChar * middleValue = (*fontChars)[middle];
			const wchar_t foundChar = middleValue->getCharCode();
			if (foundChar == needChar) { charsArray->add(middleValue); break; }
			else if (foundChar > needChar)
			{
				if (middle)
				{
					right = middle - 1;
				}
				else
				{
					break;
				}
			}
			else { left = middle + 1; }
		}
	}
	return true;
}

REBOOL REFontObject::fillArrayWithCharsForText(REArray<RETTFFontChar*> * charsArray,
											   const REString & text)
{
	if (charsArray && text.getLength())
	{
		REArray<RETTFFontChar*> * fontChars = this->getChars();
		if (REArray<RETTFFontChar *>::isNotEmpty(fontChars))
		{
			return REFontObject::fillArrayWithCharsForTextFromFont(charsArray, text, fontChars);
		}
	}
	return false;
}

REArray<RETTFFontChar*> * REFontObject::getChars()
{
	if (_base) 
	{
		if ( ((REFontBase *)_base)->isLoaded() ) 
		{
			return ((REFontBase *)_base)->charsArray;
		}
		else if (this->load())
		{
			return ((REFontBase *)_base)->charsArray;
		}
	}	
	return NULL;
}

REBOOL REFontObject::load()
{
	REBOOL isLoaded = false;
	if (_base) 
	{
		REFontBase * oldBase = (REFontBase*)_base;
		_base = NULL;
		REFontsCache c;
		REFontBase * newBase = c.get(oldBase);
		if (newBase) 
		{
			_base = newBase;
			newBase->retain();
			this->recalculateScaleRatio(oldBase->height, newBase->height);
			isLoaded = true;
		}
		
		oldBase->release();
	}
	return isLoaded;
}

REBOOL REFontObject::prepareForSetParams()
{
	if (_base) 
	{
		REFontBase * base = (REFontBase*)_base;
		if (base->isLoaded()) 
		{
			base->release();
			_base = NULL;
		}
		else 
		{
			return true;
		}
	}
	
	REFontBase * newBase = REFontBase::create();
	if (newBase) 
	{
		_base = newBase;
		return true;
	}
	
	return false;
}

void REFontObject::setPath(const REString & newPath)
{
	if (this->prepareForSetParams()) 
	{
		((REFontBase*)_base)->path = newPath;
	}
}

void REFontObject::setHeight(const REFloat32 newHeight)
{
	if (this->prepareForSetParams()) 
	{
		_height = newHeight;
		((REFontBase*)_base)->height = newHeight;
	}
}

void REFontObject::onPrepareGUIObjectForSetuping()
{
	this->prepareForSetParams();
}

void REFontObject::onSetupingGUIObjectFinished(const REBOOL isAcceptedByParent)
{
	if (isAcceptedByParent) 
	{
		this->load();
	}
}

REBOOL REFontObject::acceptStringParameter(const char * key, const char * value)
{
	if (key && value) 
	{
        if (strcmp(key, RE_FONT_XML_HEIGHT_KEY_STRING) == 0)
		{
			float h = 0.0f;
            if (sscanf(value, RE_FONT_XML_HEIGHT_FORMAT_STRING, &h) == 1)
			{
				this->setHeight((REFloat32)h);
				return true;
			}
		}
        else if (strcmp(key, RE_FONT_XML_PATH_KEY_STRING) == 0)
		{
			this->setPath(REString(value));
			return true;
		}
	}
	return false;
}

REFontObject::REFontObject() : REGUIObject(),
	_base(NULL),
	_height(0.0f)
{
	
}

void REFontObject::onReleased()
{
	if (_base) 
	{
		REFontBase * base = (REFontBase*)_base;
		_base = NULL;
		
		base->release();
	}
	
	REGUIObject::onReleased();
}

REFontObject::~REFontObject()
{
	
}

REFontObject * REFontObject::create()
{
	REFontObject * newFont = new REFontObject();
	return newFont;
}

REFontObject * REFontObject::createWithPath(const REString & path, const REFloat32 height)
{
	REFontObject * newFont = new REFontObject();
	if (newFont) 
	{
		newFont->setPath(path);
		newFont->setHeight(height);
		if (newFont->load())
		{
			return newFont;
		}
		delete newFont;
	}
	return NULL;
}


const char * REFontObject::getXMLPathKeyString() { return RE_FONT_XML_PATH_KEY_STRING; }
const char * REFontObject::getXMLHeightKeyString() { return RE_FONT_XML_HEIGHT_KEY_STRING; }
const char * REFontObject::getXMLHeightFormatString() { return RE_FONT_XML_HEIGHT_FORMAT_STRING; }



