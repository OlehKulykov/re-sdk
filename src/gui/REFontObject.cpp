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

#define RE_FONT_XML_HEIGHT_KEY_STRING "heightf"
#define RE_FONT_XML_HEIGHT_FORMAT_STRING "%f"
#define RE_FONT_XML_PATH_KEY_STRING "path"

/* REObject */
const REUInt32 REFontObject::GetClassIdentifier() const
{
    return REFontObject::ClassIdentifier();
}

const REUInt32 REFontObject::ClassIdentifier()
{
    static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REFontObject");
    return clasIdentif;
}

REBOOL REFontObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REFontObject::ClassIdentifier() == classIdentifier) ||
			REGUIObject::IsImplementsClass(classIdentifier));
}

void REFontObject::RecalculateScaleRatio(const REFloat32 needHeight, const REFloat32 loadedHeight)
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

REBOOL REFontObject::FillArrayWithCharsForTextFromFont(REArray<RETTFFontChar*> * charsArray,
													   const REString & text,
													   REArray<RETTFFontChar*> * fontChars)
{
	REStringPresentation p(text);
	charsArray->SetCapacity(p.GetWideLength() + 1);
	const REUInt32 startRight = fontChars->Count() - 1;
	const wchar_t * wideText = p.WideString();
	wchar_t needChar;
	while ((needChar = *wideText++))
	{
		REUInt32 left = 0;
		REUInt32 right = startRight;
		while (left <= right)
		{
			const REUInt32 middle = (left + right) / 2;
			RETTFFontChar * middleValue = (*fontChars)[middle];
			const wchar_t foundChar = middleValue->GetCharCode();
			if (foundChar == needChar) { charsArray->Add(middleValue); break; }
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

REBOOL REFontObject::FillArrayWithCharsForText(REArray<RETTFFontChar*> * charsArray,
											   const REString & text)
{
	if (charsArray && text.Length())
	{
		REArray<RETTFFontChar*> * fontChars = this->GetChars();
		if (REArray<RETTFFontChar *>::IsNotEmpty(fontChars))
		{
			return REFontObject::FillArrayWithCharsForTextFromFont(charsArray, text, fontChars);
		}
	}
	return false;
}

REArray<RETTFFontChar*> * REFontObject::GetChars()
{
	if (_base) 
	{
		if ( ((REFontBase *)_base)->IsLoaded() ) 
		{
			return ((REFontBase *)_base)->charsArray;
		}
		else if (this->Load())
		{
			return ((REFontBase *)_base)->charsArray;
		}
	}	
	return NULL;
}

REBOOL REFontObject::Load()
{
	REBOOL isLoaded = false;
	if (_base) 
	{
		REFontBase * oldBase = (REFontBase*)_base;
		_base = NULL;
		REFontsCache c;
		REFontBase * newBase = c.Get(oldBase);
		if (newBase) 
		{
			_base = newBase;
			newBase->Retain();
			this->RecalculateScaleRatio(oldBase->height, newBase->height);
			isLoaded = true;
		}
		
		oldBase->Release();
	}
	return isLoaded;
}

REBOOL REFontObject::PrepareForSetParams()
{
	if (_base) 
	{
		REFontBase * base = (REFontBase*)_base;
		if (base->IsLoaded()) 
		{
			base->Release();
			_base = NULL;
		}
		else 
		{
			return true;
		}
	}
	
	REFontBase * newBase = REFontBase::Create();
	if (newBase) 
	{
		_base = newBase;
		return true;
	}
	
	return false;
}

void REFontObject::SetPath(const REString & newPath)
{
	if (this->PrepareForSetParams()) 
	{
		((REFontBase*)_base)->path.Set(newPath);
	}
}

void REFontObject::SetHeight(const REFloat32 newHeight)
{
	if (this->PrepareForSetParams()) 
	{
		_height = newHeight;
		((REFontBase*)_base)->height = newHeight;
	}
}

void REFontObject::OnPrepareGUIObjectForSetuping()
{
	this->PrepareForSetParams();
}

void REFontObject::OnSetupingGUIObjectFinished(const REBOOL isAcceptedByParent)
{
	if (isAcceptedByParent) 
	{
		this->Load();
	}
}

REBOOL REFontObject::AcceptStringParameter(const char * key, const char * value)
{
	if (key && value) 
	{
        if (strcmp(key, RE_FONT_XML_HEIGHT_KEY_STRING) == 0)
		{
			float h = 0.0f;
            if (sscanf(value, RE_FONT_XML_HEIGHT_FORMAT_STRING, &h) == 1)
			{
				this->SetHeight((REFloat32)h);
				return true;
			}
		}
        else if (strcmp(key, RE_FONT_XML_PATH_KEY_STRING) == 0)
		{
			this->SetPath(REString(value));
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

void REFontObject::OnReleased()
{
	if (_base) 
	{
		REFontBase * base = (REFontBase*)_base;
		_base = NULL;
		
		base->Release();
	}
	
	REGUIObject::OnReleased();
}

REFontObject::~REFontObject()
{
	
}

REFontObject * REFontObject::Create()
{
	REFontObject * newFont = new REFontObject();
	return newFont;
}

REFontObject * REFontObject::CreateWithPath(const REString & path, const REFloat32 height)
{
	REFontObject * newFont = new REFontObject();
	if (newFont) 
	{
		newFont->SetPath(path);
		newFont->SetHeight(height);
		if (newFont->Load())
		{
			return newFont;
		}
		delete newFont;
	}
	return NULL;
}


const char * REFontObject::GetXMLPathKeyString() { return RE_FONT_XML_PATH_KEY_STRING; }
const char * REFontObject::GetXMLHeightKeyString() { return RE_FONT_XML_HEIGHT_KEY_STRING; }
const char * REFontObject::GetXMLHeightFormatString() { return RE_FONT_XML_HEIGHT_FORMAT_STRING; }



