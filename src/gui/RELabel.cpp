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


#include "../../include/regui/RELabel.h"
#include "../../include/recore/RERange.h"

class RELabelRETextLineBreakGeneratorPrivate 
{
public:
	RESize TextSize(REArray<RETTFFontChar *> * charsArr, 
					REFontObject * font, 
					const REFloat32 charsSpaceRatio);
	RESize TruncTailOrHead(REArray<RETTFFontChar *> * resultChars, 
						   REArray<RETTFFontChar *> * allChars, 
						   REArray<RETTFFontChar *> * dotsChars, 
						   REFontObject * font, 
						   const REFloat32 maxWidth, 
						   const REFloat32 dotsCharsWidth,
						   const REFloat32 charsSpaceRatio,
						   const REBOOL isAddDotsLast);
	RESize TruncMiddle(REArray<RETTFFontChar *> * resultChars, 
					   REArray<RETTFFontChar *> * allChars, 
					   REArray<RETTFFontChar *> * dotsChars, 
					   REFontObject * font, 
					   const REFloat32 maxWidth, 
					   const REFloat32 dotsCharsWidth,
					   const REFloat32 charsSpaceRatio);
	RELabelRETextLineBreakGeneratorPrivate() { }
	~RELabelRETextLineBreakGeneratorPrivate() { }
};


#define RE_LABEL_XML_TEXT_KEY_STRING "text"
#define RE_LABEL_XML_TEXT_COLOR_KEY_STRING "textcolorrgbaf"
#define RE_LABEL_XML_TEXT_COLOR_FORMAT_STRING "%f;%f;%f;%f"
#define RE_LABEL_XML_TEXT_INSETS_KEY_STRING "textinsetsf"
#define RE_LABEL_XML_TEXT_INSETS_FORMAT_STRING "%f;%f;%f;%f"
#define RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING "charsspaceratiof"
#define RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING "%f"
#define RE_LABEL_XML_SHADOW_COLOR_KEY_STRING "shadowcolorrgbaf"
#define RE_LABEL_XML_SHADOW_COLOR_FORMAT_STRING "%f;%f;%f;%f"
#define RE_LABEL_XML_SHADOW_OFFSET_KEY_STRING "shadowoffset"
#define RE_LABEL_XML_SHADOW_OFFSET_FORMAT_STRING "%f;%f"
#define RE_LABEL_XML_USING_SHADOW_KEY_STRING "useshadow"
#define RE_LABEL_XML_USING_SHADOW_FORMAT_STRING "%i"
#define RE_LABEL_XML_SHOW_BACKGROUND_KEY_STRING "showback"
#define RE_LABEL_XML_SHOW_BACKGROUND_FORMAT_STRING "%i"
#define RE_LABEL_XML_TEXT_ALIGNMENT_KEY_STRING "textalign"
#define RE_LABEL_XML_TEXT_ALIGNMENT_LEFT_KEY_STRING "left"
#define RE_LABEL_XML_TEXT_ALIGNMENT_CENTER_KEY_STRING "center"
#define RE_LABEL_XML_TEXT_ALIGNMENT_RIGHT_KEY_STRING "right"
#define RE_LABEL_XML_LINE_BREAK_KEY_STRING "linebreak"
#define RE_LABEL_XML_LINE_BREAK_NONE_KEY_STRING "none"
#define RE_LABEL_XML_LINE_BREAK_TRUNC_TAIL_KEY_STRING "trunctail"
#define RE_LABEL_XML_LINE_BREAK_TRUNC_MIDDLE_KEY_STRING "truncmidl"
#define RE_LABEL_XML_LINE_BREAK_TRUNC_HEAD_KEY_STRING "trunchead"
#define RE_LABEL_XML_TEXT_LINE_TRANC_STRING_KEY_STRING "textlinetruncstr"
#define RE_LABEL_XML_FONT_OBJECT_KEY_STRING "font"


/* REObject */
const REUInt32 RELabel::GetClassIdentifier() const
{
	return RELabel::ClassIdentifier();
}

const REUInt32 RELabel::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RELabel");
	return clasIdentif;
}

REBOOL RELabel::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RELabel::ClassIdentifier() == classIdentifier) ||
			REView::IsImplementsClass(classIdentifier));
}

RERect RELabel::GetTextFrame() const
{
	RERect r(0.0f, _textInsets.top, _charsSize.width, _charsSize.height);
	
	switch (this->GetTextAlignment()) 
	{
		case RETextAlignmentLeft:
			r.x = _textInsets.left;
			break;
		case RETextAlignmentCenter:
			r.x = (_frame.width / 2.0f) - (_charsSize.width / 2.0f);
			break;
		case RETextAlignmentRight:
			r.x = _frame.width - _charsSize.width;
			break;
		default:
			break;
	}
	
	if (_frame.height != 0.0f)
	{
		r.height *= (_textInsets.GetAdjustedRect(_frame).height / _frame.height);
	}
	
	return r;
}

void RELabel::RenderAtWithShadow(const REFloat32 x, const REFloat32 y)
{
	if (_font) 
	{
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		RESize ratio(_font->GetCharsScaleRatio());
		if (_frame.height != 0.0f) 
		{
			ratio.height *= (_textInsets.GetAdjustedRect(_frame).height / _frame.height);
		}
		REFloat32 penX = x;
		const REFloat32 bottomY = y + _font->GetHeight() - _textInsets.bottom - _textInsets.top;
		for (REUInt32 i = 0; i < _chars.Count(); i++) 
		{
			RETTFFontChar * fontChar = _chars[i];
			penX += ((ratio.width * fontChar->offsetX) * _charsSpaceRatio);
			const REFloat32 penY = bottomY - (ratio.height * fontChar->offsetY);
			const REFloat32 width = (ratio.width * fontChar->width);
			const REFloat32 height = (ratio.height * fontChar->height);
			device->RenderRect(RERect(penX + _shadowOffset.x, penY + _shadowOffset.y, width, height),
							   _shadowColor, 
							   fontChar->textureFrame, 
							   fontChar->texture);
			device->RenderRect(RERect(penX, penY, width, height),
							   _textColor, 
							   fontChar->textureFrame, 
							   fontChar->texture);
			penX += (ratio.width * fontChar->advanceX);
		}
	}
}

void RELabel::RenderAtWithOutShadow(const REFloat32 x, const REFloat32 y)
{
	if (_font) 
	{
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		RESize ratio(_font->GetCharsScaleRatio());
		if (_frame.height != 0.0f) 
		{
			ratio.height *= (_textInsets.GetAdjustedRect(_frame).height / _frame.height);
		}
		REFloat32 penX = x;
		const REFloat32 bottomY = y + _font->GetHeight() - _textInsets.bottom - _textInsets.top;
		for (REUInt32 i = 0; i < _chars.Count(); i++) 
		{
			RETTFFontChar * fontChar = _chars[i];
			penX += ((ratio.width * fontChar->offsetX) * _charsSpaceRatio);
			const REFloat32 penY = bottomY - (ratio.height * fontChar->offsetY);
			const REFloat32 width = (ratio.width * fontChar->width);
			const REFloat32 height = (ratio.height * fontChar->height);
			device->RenderRect(RERect(penX, penY, width, height),
							   _textColor, 
							   fontChar->textureFrame, 
							   fontChar->texture);
			penX += (ratio.width * fontChar->advanceX);
		}
	}
}

void RELabel::RenderAt(const REFloat32 x, const REFloat32 y)
{
	REBOOL isShadow = _isUsingShadow;
	if (isShadow) 
	{ 
        isShadow = ( (_shadowOffset.x != 0.0f) || (_shadowOffset.y != 0.0f) );
	}
	
	if (isShadow) 
	{
		this->RenderAtWithShadow(x, y);
	}
	else
	{
		this->RenderAtWithOutShadow(x, y);
	}
}

/* IRERenderable */
void RELabel::Render()
{
	if (_isVisible) 
	{
		if (_isShowBackground && (_color.alpha > 0.0f)) 
		{
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(_frame, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				device->RenderRect(_frame, _color, NULL);
			}
		}
		
		RERect frame(_textInsets.GetAdjustedRect(_frame));
		switch (this->GetTextAlignment()) 
		{
			case RETextAlignmentLeft:
				this->RenderAt(frame.x, frame.y);
				break;
			case RETextAlignmentCenter:
				this->RenderAt((frame.x + (frame.width / 2.0f)) - (_charsSize.width / 2.0f), frame.y);
				break;
			case RETextAlignmentRight:
				this->RenderAt(frame.x + frame.width - _charsSize.width, frame.y);
				break;
			default:
				break;
		}
		
		this->RenderSubViews(_frame.x, _frame.y);
	}
}

void RELabel::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_isVisible) 
	{
		const REFloat32 renderX = _frame.x + offsetX;
		const REFloat32 renderY = _frame.y + offsetY;
		
		if (_isShowBackground && (_color.alpha > 0.0f)) 
		{
			RERect rectWithOffset(renderX, renderY, _frame.width, _frame.height);
			RERenderDevice * device = RERenderDevice::GetDefaultDevice();
			if (_texture) 
			{
				device->RenderRect(rectWithOffset, _color, _texture->GetFrame(), _texture->GetTexture());
			}
			else
			{
				device->RenderRect(rectWithOffset, _color, NULL);
			}
		}
		
		RERect frame(_textInsets.GetAdjustedRect(_frame));
		const REFloat32 renderXInset = frame.x + offsetX;
		const REFloat32 renderYInset = frame.y + offsetY;
		switch (this->GetTextAlignment()) 
		{
			case RETextAlignmentLeft:
				this->RenderAt(renderXInset, renderYInset);
				break;
			case RETextAlignmentCenter:
				this->RenderAt((renderXInset + (frame.width / 2.0f)) - (_charsSize.width / 2.0f), renderYInset);
				break;
			case RETextAlignmentRight:
				this->RenderAt(renderXInset + frame.width - _charsSize.width, renderYInset);
				break;
			default:
				break;
		}
		
		this->RenderSubViews(renderX, renderY);
	}
}

/// Setting rectangular frame of view.
void RELabel::SetFrame(const RERect & newViewFrame)
{
	REView::SetFrame(newViewFrame);
	this->ReloadChars();
}

void RELabel::SetShadowOffset(const REPoint2 & newOffset)
{
	_shadowOffset = newOffset;
}

void RELabel::SetShadowColor(const REColor & newColor)
{
	_shadowColor = newColor;
}

void RELabel::SetCharsSpaceRatio(const REFloat32 newRatio)
{
	if (_charsSpaceRatio != newRatio) 
	{
		_charsSpaceRatio = newRatio;
		this->ReloadChars();
	}
}

REBOOL RELabel::IsCanReloadChars() const
{
	if (_font && _text.Length()) 
	{
		REArray<RETTFFontChar *> * charsArray = _font->GetChars();
		if (charsArray) 
		{
			return true; 
		}
	}
	return false;
}

RESize RELabelRETextLineBreakGeneratorPrivate::TextSize(REArray<RETTFFontChar *> * charsArr, 
														REFontObject * font, 
														const REFloat32 charsSpaceRatio)
{
	if (charsArr && font) 
	{
		RESize size(0.0f, 0.0f);
		RESize ratio(font->GetCharsScaleRatio());
		for (REUInt32 i = 0; i < charsArr->Count(); i++) 
		{
			RETTFFontChar * fontChar = (*charsArr)[i];
			size.width += ((ratio.width * fontChar->offsetX) * charsSpaceRatio);
			size.width += (ratio.width * fontChar->advanceX);
			const REFloat32 height = (ratio.height * fontChar->height);
			if (size.height < height) 
			{
				size.height = height;
			}
		}
		return size;
	}
	return RESize(0.0f, 0.0f);
}

RESize RELabelRETextLineBreakGeneratorPrivate::TruncTailOrHead(REArray<RETTFFontChar *> * resultChars, 
															   REArray<RETTFFontChar *> * allChars, 
															   REArray<RETTFFontChar *> * dotsChars, 
															   REFontObject * font, 
															   const REFloat32 maxWidth, 
															   const REFloat32 dotsCharsWidth,
															   const REFloat32 charsSpaceRatio,
															   const REBOOL isAddDotsLast)
{
	if (resultChars && allChars && dotsChars && font) 
	{
		RESize size(0.0f, 0.0f);
		
		REUInt32 index = 0;
		if (allChars->Count()) { if (!isAddDotsLast) { index = (allChars->Count() - 1); } }
		else { return size; }
		
		RESize ratio(font->GetCharsScaleRatio());
		REBOOL isIterating = true;
		while (isIterating) 
		{
			RETTFFontChar * fontChar = (*allChars)[index];
			REFloat32 width = ((ratio.width * fontChar->offsetX) * charsSpaceRatio);
			width += (ratio.width * fontChar->advanceX);
			const REFloat32 totalWidth = size.width + width + dotsCharsWidth;
			if (totalWidth < maxWidth) 
			{
				if (isAddDotsLast) { resultChars->Add(fontChar); }
				else { resultChars->Insert(0, fontChar); }
				size.width += width;
			}
			else 
			{
				if (resultChars->Count() && dotsChars->Count()) 
				{
					REUInt32 j = 0;
					REBOOL isAdding = true;
					if (!isAddDotsLast) { j = (dotsChars->Count() - 1); }
					while (isAdding) 
					{
						RETTFFontChar * dotChar = (*dotsChars)[j];
						if (isAddDotsLast) { resultChars->Add(dotChar); }
						else { resultChars->Insert(0, dotChar); }
						REFloat32 dotWidth = ((ratio.width * dotChar->offsetX) * charsSpaceRatio);
						dotWidth += (ratio.width * dotChar->advanceX);
						size.width += dotWidth;
						if (isAddDotsLast) 
						{
							j++;
							isAdding = (j < dotsChars->Count());
						}
						else if (j) { j--; }
						else { isAdding = false; }
					}
				}
				return size;
			}
			
			if (isAddDotsLast) 
			{
				index++;
				isIterating = (index < allChars->Count());
			}
			else if (index) { index--; }
			else { isIterating = false; }
		}
		
		return size;
	}
	return RESize(0.0f, 0.0f);
}

RESize RELabelRETextLineBreakGeneratorPrivate::TruncMiddle(REArray<RETTFFontChar *> * resultChars, 
																	   REArray<RETTFFontChar *> * allChars, 
																	   REArray<RETTFFontChar *> * dotsChars, 
																	   REFontObject * font, 
																	   const REFloat32 maxWidth, 
																	   const REFloat32 dotsCharsWidth,
																	   const REFloat32 charsSpaceRatio)
{
	if (resultChars && allChars && dotsChars && font) 
	{
		RESize size(0.0f, 0.0f);
		RESize ratio(font->GetCharsScaleRatio());
		if (allChars->IsEmpty()) 
		{
			return size;
		}
		
		RERange leftRange(0, 0);
		RERange rightRange(0, 0);
		REUInt32 leftIndex = 0;
		REUInt32 rightIndex = (allChars->Count() - 1);
		REBOOL isLeft = true;
		REUInt32 processedCharsCount = 0;
		while (1) 
		{
			RETTFFontChar * fontChar = isLeft ? (*allChars)[leftIndex] : (*allChars)[rightIndex];
			REFloat32 width = ((ratio.width * fontChar->offsetX) * charsSpaceRatio);
			width += (ratio.width * fontChar->advanceX);
			const REFloat32 totalWidth = size.width + width + dotsCharsWidth;
			if (totalWidth < maxWidth) 
			{
				if (isLeft) { leftRange.length++; }
				else
				{
					rightRange.length++;
					rightRange.location = rightIndex;
				}
				size.width += width;
			}
			else { break; }
			
			processedCharsCount++;
			if (processedCharsCount == allChars->Count()) { break; }
			
			if (isLeft) { leftIndex++; }
			else { rightIndex--; }
			isLeft = !isLeft;
		}
		
		if ((leftRange.length > 0) || (rightRange.length > 0)) 
		{
			for (REUInt32 i = 0; i < leftRange.length; i++) 
			{
				RETTFFontChar * fontChar = (*allChars)[i];
				resultChars->Add(fontChar);
			}
			for (REUInt32 i = 0; i < dotsChars->Count(); i++) 
			{
				RETTFFontChar * dotChar = (*dotsChars)[i];
				resultChars->Add(dotChar);
				REFloat32 dotWidth = ((ratio.width * dotChar->offsetX) * charsSpaceRatio);
				dotWidth += (ratio.width * dotChar->advanceX);
				size.width += dotWidth;
			}
			for (REUInt32 i = 0; i < rightRange.length; i++) 
			{
				RETTFFontChar * fontChar = (*allChars)[rightRange.location];
				resultChars->Add(fontChar);
				rightRange.location++;
			}
		}		
		
		return size;
	}
	return RESize(0.0f, 0.0f);
}

void RELabel::LayoutChars()
{
	if (_chars.Count()) 
	{
		RELabelRETextLineBreakGeneratorPrivate generator;
		_charsSize = generator.TextSize(&_chars, _font, _charsSpaceRatio);
		RERect frame(_textInsets.GetAdjustedRect(_frame));
		if (_charsSize.width > frame.width) 
		{
			REArray<RETTFFontChar *> dotsChars;
			RESize dotsSize(0.0f, 0.0f);
			RELabelRETextLineBreakGeneratorPrivate generator;
			if (((RETextLineBreak)_lineBreakMode) != RETextLineBreakNone) 
			{
				_font->FillArrayWithCharsForText(&dotsChars, _textLineTruncationString);
				dotsSize = generator.TextSize(&dotsChars, _font, _charsSpaceRatio);
			}
			
			REArray<RETTFFontChar *> resultChars(_chars.Count() + 1);
			RESize withBreakSize(0.0f, 0.0f);
			switch (((RETextLineBreak)_lineBreakMode)) 
			{
				case RETextLineBreakNone:
					withBreakSize = generator.TruncTailOrHead(&resultChars, 
															  &_chars, 
															  &dotsChars, 
															  _font, 
															  frame.width, 
															  dotsSize.width, 
															  _charsSpaceRatio, 
															  true);
					break;
				case RETextLineBreakTruncateTail:
					withBreakSize = generator.TruncTailOrHead(&resultChars, 
															  &_chars, 
															  &dotsChars,
															  _font, 
															  frame.width, 
															  dotsSize.width,
															  _charsSpaceRatio,
															  true);
					break;
				case RETextLineBreakTruncateMiddle:
					withBreakSize = generator.TruncMiddle(&resultChars, 
														  &_chars, 
														  &dotsChars,
														  _font, 
														  frame.width, 
														  dotsSize.width,
														  _charsSpaceRatio);
					break;
				case RETextLineBreakTruncateHead:
					withBreakSize = generator.TruncTailOrHead(&resultChars, 
															  &_chars, 
															  &dotsChars,
															  _font, 
															  frame.width, 
															  dotsSize.width,
															  _charsSpaceRatio,
															  false);
					break;
				default:
					break;
			}
			_chars = resultChars;
			_charsSize = withBreakSize;
		}
	}
}

void RELabel::ReloadChars()
{
	_chars.Clear();
	_charsSize.width = _charsSize.height = 0.0f;
	if (this->IsCanReloadChars()) 
	{
		_font->FillArrayWithCharsForText(&_chars, _text);
		this->LayoutChars();
	}
}

void RELabel::SetText(const REString & newText)
{
	_text.Set(newText);
	this->ReloadChars();
}

void RELabel::SetFont(REFontObject * newFont)
{	
	if (_font) 
	{
		_font->Release();
		_font = NULL;
	}
	
	if (newFont) 
	{
		_font = newFont;
		_font->Retain();
	}
	this->ReloadChars();
}

/// Returns string wich replaces on truncation.
/// Default is three dots.
const REString & RELabel::GetTextLineTruncationString() const
{
	return _textLineTruncationString;
}

/// Setting new custom truncation string.
void RELabel::SetTextLineTruncationString(const REString & newTruncationString)
{
	_textLineTruncationString.Set(newTruncationString);
}

REBOOL RELabel::AcceptLabelStringParameter(RELabel * label, const char * key, const char * value)
{
    if (strcmp(key, RE_LABEL_XML_TEXT_KEY_STRING) == 0)
    {
        label->SetText(REString(value));
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_COLOR_KEY_STRING) == 0)
    {
        REColor c;
        if (sscanf(value, RE_LABEL_XML_TEXT_COLOR_FORMAT_STRING, &c.red, &c.green, &c.blue, &c.alpha) == 4)
        {
            label->SetTextColor(c);
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_INSETS_KEY_STRING) == 0)
    {
        REEdgeInsets i;
        if (sscanf(value, RE_LABEL_XML_TEXT_INSETS_FORMAT_STRING, &i.top, &i.left, &i.bottom, &i.right) == 4)
        {
            label->SetTextInsets(i);
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_SHADOW_COLOR_KEY_STRING) == 0)
    {
        REColor c;
        if (sscanf(value, RE_LABEL_XML_SHADOW_COLOR_FORMAT_STRING, &c.red, &c.green, &c.blue, &c.alpha) == 4)
        {
            label->SetShadowColor(c);
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_SHADOW_OFFSET_KEY_STRING) == 0)
    {
        REPoint2 o;
        if (sscanf(value, RE_LABEL_XML_SHADOW_OFFSET_FORMAT_STRING, &o.x, &o.y) == 2)
        {
            label->SetShadowOffset(o);
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_USING_SHADOW_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_LABEL_XML_USING_SHADOW_FORMAT_STRING, &v) == 1)
        {
            label->SetUsingShadow((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_SHOW_BACKGROUND_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_LABEL_XML_SHOW_BACKGROUND_FORMAT_STRING, &v) == 1)
        {
            label->SetShowBackground((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_ALIGNMENT_KEY_STRING) == 0)
    {
        if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_LEFT_KEY_STRING) == 0) { label->SetTextAlignment(RETextAlignmentLeft); }
        else if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_CENTER_KEY_STRING) == 0) { label->SetTextAlignment(RETextAlignmentCenter); }
        else if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_RIGHT_KEY_STRING) == 0) { label->SetTextAlignment(RETextAlignmentRight); }
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_LINE_BREAK_KEY_STRING) == 0)
    {
        if (strcmp(value, RE_LABEL_XML_LINE_BREAK_NONE_KEY_STRING) == 0) { label->SetLineBreak(RETextLineBreakNone); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_TAIL_KEY_STRING) == 0) { label->SetLineBreak(RETextLineBreakTruncateTail); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_MIDDLE_KEY_STRING) == 0) { label->SetLineBreak(RETextLineBreakTruncateMiddle); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_HEAD_KEY_STRING) == 0) { label->SetLineBreak(RETextLineBreakTruncateHead); }
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_LINE_TRANC_STRING_KEY_STRING) == 0)
    {
        label->SetTextLineTruncationString(REString(value));
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING) == 0)
    {
        float r = -1.0f;
        if (sscanf(value, RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING, &r) == 1)
        {
            label->SetCharsSpaceRatio((REFloat32)r);
            return true;
        }
    }
    return false;
}

REBOOL RELabel::AcceptStringParameter(const char * key, const char * value)
{
    if (REView::AcceptStringParameter(key, value))
	{
		return true;
	}
	
	if (key && value) 
	{
        return RELabel::AcceptLabelStringParameter(this, key, value);
	}
	return false;
}

REBOOL RELabel::AcceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if ( REView::AcceptObjectParameter(className, key, value) ) 
	{
		return true;
	}
	
	if (className && key && value) 
	{
        if (strcmp(key, RE_LABEL_XML_FONT_OBJECT_KEY_STRING) == 0)
		{
			if (strcmp(className, "REFontObject") == 0) 
			{
				REFontObject * font = (REFontObject *)value;
				if (font) 
				{
					this->SetFont(font);
					font->Release();
					return true;
				}
			}
		}
	}
	
	return false;
}

const RETextAlignment RELabel::GetTextAlignment() const
{
	return (RETextAlignment)_textAlignment;
}

void RELabel::SetTextAlignment(const RETextAlignment newTextAlignment)
{
	_textAlignment = (REUByte)newTextAlignment;
}

const RETextLineBreak RELabel::GetLineBreak() const
{
	return (RETextLineBreak)_lineBreakMode;
}

void RELabel::SetLineBreak(const RETextLineBreak newLineBreak)
{
	if (_lineBreakMode != (REUByte)newLineBreak) 
	{
		_lineBreakMode = (REUByte)newLineBreak;
		this->ReloadChars();
	}
}

/// Getting label text insets.
const REEdgeInsets & RELabel::GetTextInsets() const
{
	return _textInsets;
}

/// Setting new label text insets.
void RELabel::SetTextInsets(const REEdgeInsets & newInsets)
{
	_textInsets = newInsets;
	
	this->ReloadChars();
}

/// Returns text color.
const REColor & RELabel::GetTextColor() const
{
	return _textColor;
}

/// Setting text color.
void RELabel::SetTextColor(const REColor & newTextColor)
{
	_textColor = newTextColor;
}

/// Is showing label background using view color and texture.
REBOOL RELabel::IsShowBackground() const
{
	return _isShowBackground;
}

/// Setting showing label background using view color and texture.
void RELabel::SetShowBackground(REBOOL isShowBackground)
{
	_isShowBackground = isShowBackground;
}

void RELabel::OnReleased()
{
	RE_SAFE_RELEASE(_font);
	
	REView::OnReleased();
}

RELabel::RELabel() : REView(),
	_font(NULL),
	_charsSpaceRatio(1.0f),
	_isUsingShadow(false),
	_isShowBackground(false),
	_textAlignment(0),
	_lineBreakMode(0)
{
	_textLineTruncationString.Set("...");
}

RELabel::~RELabel()
{
	
}

RELabel * RELabel::Create()
{
	RELabel * newLabel = new RELabel();
	return newLabel;
}

const char * RELabel::GetXMLTextKeyString() { return RE_LABEL_XML_TEXT_KEY_STRING; }
const char * RELabel::GetXMLTextColorKeyString() { return RE_LABEL_XML_TEXT_COLOR_KEY_STRING; }
const char * RELabel::GetXMLTextColorFormatString() { return RE_LABEL_XML_TEXT_COLOR_FORMAT_STRING; }
const char * RELabel::GetXMLTextInsetsKeyString() { return RE_LABEL_XML_TEXT_INSETS_KEY_STRING; }
const char * RELabel::GetXMLTextInsetsFormatString() { return RE_LABEL_XML_TEXT_INSETS_FORMAT_STRING; }
const char * RELabel::GetXMLCharsSpaceRatioKeyString() { return RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING; }
const char * RELabel::GetXMLCharsSpaceRatioFormatString() { return RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING; }
const char * RELabel::GetXMLShadowColorKeyString() { return RE_LABEL_XML_SHADOW_COLOR_KEY_STRING; }
const char * RELabel::GetXMLShadowColorFormatString() { return RE_LABEL_XML_SHADOW_COLOR_FORMAT_STRING; }
const char * RELabel::GetXMLShadowOffsetKeyString() { return RE_LABEL_XML_SHADOW_OFFSET_KEY_STRING; }
const char * RELabel::GetXMLShadowOffsetFormatString() { return RE_LABEL_XML_SHADOW_OFFSET_FORMAT_STRING; }
const char * RELabel::GetXMLUsingShadowKeyString() { return RE_LABEL_XML_USING_SHADOW_KEY_STRING; }
const char * RELabel::GetXMLUsingShadowFormatString() { return RE_LABEL_XML_USING_SHADOW_FORMAT_STRING; }
const char * RELabel::GetXMLShowBackgroundKeyString() { return RE_LABEL_XML_SHOW_BACKGROUND_KEY_STRING; }
const char * RELabel::GetXMLShowBackgroundFormatString() { return RE_LABEL_XML_SHOW_BACKGROUND_FORMAT_STRING; }
const char * RELabel::GetXMLTextAlignmentKeyString() { return RE_LABEL_XML_TEXT_ALIGNMENT_KEY_STRING; }
const char * RELabel::GetXMLTextAlignmentStringByType(const RETextAlignment alignmentType)
{
    switch (alignmentType)
    {
    case RETextAlignmentLeft: return RE_LABEL_XML_TEXT_ALIGNMENT_LEFT_KEY_STRING; break;
    case RETextAlignmentCenter: return RE_LABEL_XML_TEXT_ALIGNMENT_CENTER_KEY_STRING; break;
    case RETextAlignmentRight: return RE_LABEL_XML_TEXT_ALIGNMENT_RIGHT_KEY_STRING; break;
    default: break;
    };
    return "none";
}
const char * RELabel::GetXMLLineBreakKeyString() { return RE_LABEL_XML_LINE_BREAK_KEY_STRING; }
const char * RELabel::GetXMLLineBreakStringByType(const RETextLineBreak lineBreakType)
{
    switch (lineBreakType)
    {
        case RETextLineBreakTruncateTail: return RE_LABEL_XML_LINE_BREAK_TRUNC_TAIL_KEY_STRING; break;
        case RETextLineBreakTruncateMiddle: return RE_LABEL_XML_LINE_BREAK_TRUNC_MIDDLE_KEY_STRING; break;
        case RETextLineBreakTruncateHead: return RE_LABEL_XML_LINE_BREAK_TRUNC_HEAD_KEY_STRING; break;
		default: break;
    };
    return RE_LABEL_XML_LINE_BREAK_NONE_KEY_STRING;
}
const char * RELabel::GetXMLTextLineTruncationStringKeyString() { return RE_LABEL_XML_TEXT_LINE_TRANC_STRING_KEY_STRING; }
const char * RELabel::GetXMLFontObjectKeyString() { return RE_LABEL_XML_FONT_OBJECT_KEY_STRING; }
