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
#define RE_LABEL_XML_TEXT_INSETS_KEY_STRING "textinsetsf"
#define RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING "charsspaceratiof"
#define RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING "%f"
#define RE_LABEL_XML_SHADOW_COLOR_KEY_STRING "shadowcolorrgbaf"
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
const REUInt32 RELabel::getClassIdentifier() const
{
	return RELabel::classIdentifier();
}

const REUInt32 RELabel::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("RELabel");
	return clasIdentif;
}

REBOOL RELabel::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RELabel::classIdentifier() == classIdentifier) ||
			REView::isImplementsClass(classIdentifier));
}

RERect RELabel::getTextFrame() const
{
	RERect r(0.0f, _textInsets.top, _charsSize.width, _charsSize.height);
	
	switch (this->getTextAlignment()) 
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
		r.height *= (_textInsets.adjustedRect(_frame).height / _frame.height);
	}
	
	return r;
}

void RELabel::renderAtWithShadow(const REFloat32 x, const REFloat32 y)
{
	if (_font) 
	{
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		RESize ratio(_font->getCharsScaleRatio());
		if (_frame.height != 0.0f) 
		{
			ratio.height *= (_textInsets.adjustedRect(_frame).height / _frame.height);
		}
		REFloat32 penX = x;
		const REFloat32 bottomY = y + _font->getHeight() - _textInsets.bottom - _textInsets.top;
		for (REUInt32 i = 0; i < _chars.count(); i++) 
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

void RELabel::renderAtWithOutShadow(const REFloat32 x, const REFloat32 y)
{
	if (_font) 
	{
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		RESize ratio(_font->getCharsScaleRatio());
		if (_frame.height != 0.0f) 
		{
			ratio.height *= (_textInsets.adjustedRect(_frame).height / _frame.height);
		}
		REFloat32 penX = x;
		const REFloat32 bottomY = y + _font->getHeight() - _textInsets.bottom - _textInsets.top;
		for (REUInt32 i = 0; i < _chars.count(); i++) 
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

void RELabel::renderAt(const REFloat32 x, const REFloat32 y)
{
	REBOOL isShadow = _isUsingShadow;
	if (isShadow) 
	{ 
        isShadow = ( (_shadowOffset.x != 0.0f) || (_shadowOffset.y != 0.0f) );
	}
	
	if (isShadow) 
	{
		this->renderAtWithShadow(x, y);
	}
	else
	{
		this->renderAtWithOutShadow(x, y);
	}
}

/* IRERenderable */
void RELabel::render()
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
		
		RERect frame(_textInsets.adjustedRect(_frame));
		switch (this->getTextAlignment()) 
		{
			case RETextAlignmentLeft:
				this->renderAt(frame.x, frame.y);
				break;
			case RETextAlignmentCenter:
				this->renderAt((frame.x + (frame.width / 2.0f)) - (_charsSize.width / 2.0f), frame.y);
				break;
			case RETextAlignmentRight:
				this->renderAt(frame.x + frame.width - _charsSize.width, frame.y);
				break;
			default:
				break;
		}
		
		this->renderSubViews(_frame.x, _frame.y);
	}
}

void RELabel::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
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
		
		RERect frame(_textInsets.adjustedRect(_frame));
		const REFloat32 renderXInset = frame.x + offsetX;
		const REFloat32 renderYInset = frame.y + offsetY;
		switch (this->getTextAlignment()) 
		{
			case RETextAlignmentLeft:
				this->renderAt(renderXInset, renderYInset);
				break;
			case RETextAlignmentCenter:
				this->renderAt((renderXInset + (frame.width / 2.0f)) - (_charsSize.width / 2.0f), renderYInset);
				break;
			case RETextAlignmentRight:
				this->renderAt(renderXInset + frame.width - _charsSize.width, renderYInset);
				break;
			default:
				break;
		}
		
		this->renderSubViews(renderX, renderY);
	}
}

/// Setting rectangular frame of view.
void RELabel::setFrame(const RERect & newViewFrame)
{
	REView::setFrame(newViewFrame);
	
	this->reloadChars();
}

void RELabel::setShadowOffset(const REPoint2 & newOffset)
{
	_shadowOffset = newOffset;
}

void RELabel::setShadowColor(const REColor & newColor)
{
	_shadowColor = newColor;
}

void RELabel::setCharsSpaceRatio(const REFloat32 newRatio)
{
	if (_charsSpaceRatio != newRatio) 
	{
		_charsSpaceRatio = newRatio;
		this->reloadChars();
	}
}

REBOOL RELabel::isCanReloadChars() const
{
	if (_font && _text.length()) 
	{
		REArray<RETTFFontChar *> * charsArray = _font->getChars();
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
		RESize ratio(font->getCharsScaleRatio());
		for (REUInt32 i = 0; i < charsArr->count(); i++) 
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
		if (allChars->count()) { if (!isAddDotsLast) { index = (allChars->count() - 1); } }
		else { return size; }
		
		RESize ratio(font->getCharsScaleRatio());
		REBOOL isIterating = true;
		while (isIterating) 
		{
			RETTFFontChar * fontChar = (*allChars)[index];
			REFloat32 width = ((ratio.width * fontChar->offsetX) * charsSpaceRatio);
			width += (ratio.width * fontChar->advanceX);
			const REFloat32 totalWidth = size.width + width + dotsCharsWidth;
			if (totalWidth < maxWidth) 
			{
				if (isAddDotsLast) { resultChars->add(fontChar); }
				else { resultChars->insert(0, fontChar); }
				size.width += width;
			}
			else 
			{
				if (resultChars->count() && dotsChars->count()) 
				{
					REUInt32 j = 0;
					REBOOL isAdding = true;
					if (!isAddDotsLast) { j = (dotsChars->count() - 1); }
					while (isAdding) 
					{
						RETTFFontChar * dotChar = (*dotsChars)[j];
						if (isAddDotsLast) { resultChars->add(dotChar); }
						else { resultChars->insert(0, dotChar); }
						REFloat32 dotWidth = ((ratio.width * dotChar->offsetX) * charsSpaceRatio);
						dotWidth += (ratio.width * dotChar->advanceX);
						size.width += dotWidth;
						if (isAddDotsLast) 
						{
							j++;
							isAdding = (j < dotsChars->count());
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
				isIterating = (index < allChars->count());
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
		RESize ratio(font->getCharsScaleRatio());
		if (allChars->isEmpty()) 
		{
			return size;
		}
		
		RERange leftRange(0, 0);
		RERange rightRange(0, 0);
		REUInt32 leftIndex = 0;
		REUInt32 rightIndex = (allChars->count() - 1);
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
			if (processedCharsCount == allChars->count()) { break; }
			
			if (isLeft) { leftIndex++; }
			else { rightIndex--; }
			isLeft = !isLeft;
		}
		
		if ((leftRange.length > 0) || (rightRange.length > 0)) 
		{
			for (REUInt32 i = 0; i < leftRange.length; i++) 
			{
				RETTFFontChar * fontChar = (*allChars)[i];
				resultChars->add(fontChar);
			}
			for (REUInt32 i = 0; i < dotsChars->count(); i++) 
			{
				RETTFFontChar * dotChar = (*dotsChars)[i];
				resultChars->add(dotChar);
				REFloat32 dotWidth = ((ratio.width * dotChar->offsetX) * charsSpaceRatio);
				dotWidth += (ratio.width * dotChar->advanceX);
				size.width += dotWidth;
			}
			for (REUInt32 i = 0; i < rightRange.length; i++) 
			{
				RETTFFontChar * fontChar = (*allChars)[rightRange.location];
				resultChars->add(fontChar);
				rightRange.location++;
			}
		}		
		
		return size;
	}
	return RESize(0.0f, 0.0f);
}

void RELabel::layoutChars()
{
	if (_chars.count()) 
	{
		RELabelRETextLineBreakGeneratorPrivate generator;
		_charsSize = generator.TextSize(&_chars, _font, _charsSpaceRatio);
		RERect frame(_textInsets.adjustedRect(_frame));
		if (_charsSize.width > frame.width) 
		{
			REArray<RETTFFontChar *> dotsChars;
			RESize dotsSize(0.0f, 0.0f);
			RELabelRETextLineBreakGeneratorPrivate generator;
			if (((RETextLineBreak)_lineBreakMode) != RETextLineBreakNone) 
			{
				_font->fillArrayWithCharsForText(&dotsChars, _textLineTruncationString);
				dotsSize = generator.TextSize(&dotsChars, _font, _charsSpaceRatio);
			}
			
			REArray<RETTFFontChar *> resultChars(_chars.count() + 1);
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

void RELabel::reloadChars()
{
	_chars.clear();
	_charsSize.width = _charsSize.height = 0.0f;
	if (this->isCanReloadChars()) 
	{
		_font->fillArrayWithCharsForText(&_chars, _text);
		this->layoutChars();
	}
}

void RELabel::setText(const REString & newText)
{
	_text = newText;
	this->reloadChars();
}

void RELabel::setFont(REFontObject * newFont)
{	
	if (_font) 
	{
		_font->release();
		_font = NULL;
	}
	
	if (newFont) 
	{
		_font = newFont;
		_font->retain();
	}
	this->reloadChars();
}

/// Returns string wich replaces on truncation.
/// Default is three dots.
const REString & RELabel::getTextLineTruncationString() const
{
	return _textLineTruncationString;
}

/// Setting new custom truncation string.
void RELabel::setTextLineTruncationString(const REString & newTruncationString)
{
	_textLineTruncationString = newTruncationString;
}

REBOOL RELabel::acceptLabelStringParameter(RELabel * label, const char * key, const char * value)
{
    if (strcmp(key, RE_LABEL_XML_TEXT_KEY_STRING) == 0)
    {
        label->setText(REString(value));
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_COLOR_KEY_STRING) == 0)
    {
		label->setTextColor(REColor::fromString(value));
		return true;
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_INSETS_KEY_STRING) == 0)
    {
		label->setTextInsets(REEdgeInsets::fromString(value));
		return true;
    }
    else if (strcmp(key, RE_LABEL_XML_SHADOW_COLOR_KEY_STRING) == 0)
    {
		label->setShadowColor(REColor::fromString(value));
		return true;
    }
    else if (strcmp(key, RE_LABEL_XML_SHADOW_OFFSET_KEY_STRING) == 0)
    {
        REPoint2 o;
        if (sscanf(value, RE_LABEL_XML_SHADOW_OFFSET_FORMAT_STRING, &o.x, &o.y) == 2)
        {
            label->setShadowOffset(o);
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_USING_SHADOW_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_LABEL_XML_USING_SHADOW_FORMAT_STRING, &v) == 1)
        {
            label->setUsingShadow((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_SHOW_BACKGROUND_KEY_STRING) == 0)
    {
        int v = 0;
        if (sscanf(value, RE_LABEL_XML_SHOW_BACKGROUND_FORMAT_STRING, &v) == 1)
        {
            label->setShowBackground((v != 0));
            return true;
        }
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_ALIGNMENT_KEY_STRING) == 0)
    {
        if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_LEFT_KEY_STRING) == 0) { label->setTextAlignment(RETextAlignmentLeft); }
        else if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_CENTER_KEY_STRING) == 0) { label->setTextAlignment(RETextAlignmentCenter); }
        else if (strcmp(value, RE_LABEL_XML_TEXT_ALIGNMENT_RIGHT_KEY_STRING) == 0) { label->setTextAlignment(RETextAlignmentRight); }
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_LINE_BREAK_KEY_STRING) == 0)
    {
        if (strcmp(value, RE_LABEL_XML_LINE_BREAK_NONE_KEY_STRING) == 0) { label->setLineBreak(RETextLineBreakNone); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_TAIL_KEY_STRING) == 0) { label->setLineBreak(RETextLineBreakTruncateTail); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_MIDDLE_KEY_STRING) == 0) { label->setLineBreak(RETextLineBreakTruncateMiddle); }
        else if (strcmp(value, RE_LABEL_XML_LINE_BREAK_TRUNC_HEAD_KEY_STRING) == 0) { label->setLineBreak(RETextLineBreakTruncateHead); }
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_TEXT_LINE_TRANC_STRING_KEY_STRING) == 0)
    {
        label->setTextLineTruncationString(REString(value));
        return true;
    }
    else if (strcmp(key, RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING) == 0)
    {
        float r = -1.0f;
        if (sscanf(value, RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING, &r) == 1)
        {
            label->setCharsSpaceRatio((REFloat32)r);
            return true;
        }
    }
    return false;
}

REBOOL RELabel::acceptStringParameter(const char * key, const char * value)
{
    if (REView::acceptStringParameter(key, value))
	{
		return true;
	}
	
	if (key && value) 
	{
        return RELabel::acceptLabelStringParameter(this, key, value);
	}
	return false;
}

REBOOL RELabel::acceptObjectParameter(const char * className, const char * key, REGUIObject * value)
{
	if ( REView::acceptObjectParameter(className, key, value) ) 
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
					this->setFont(font);
					font->release();
					return true;
				}
			}
		}
	}
	
	return false;
}

const RETextAlignment RELabel::getTextAlignment() const
{
	return (RETextAlignment)_textAlignment;
}

void RELabel::setTextAlignment(const RETextAlignment newTextAlignment)
{
	_textAlignment = (REUByte)newTextAlignment;
}

const RETextLineBreak RELabel::getLineBreak() const
{
	return (RETextLineBreak)_lineBreakMode;
}

void RELabel::setLineBreak(const RETextLineBreak newLineBreak)
{
	if (_lineBreakMode != (REUByte)newLineBreak) 
	{
		_lineBreakMode = (REUByte)newLineBreak;
		this->reloadChars();
	}
}

/// Getting label text insets.
const REEdgeInsets & RELabel::getTextInsets() const
{
	return _textInsets;
}

/// Setting new label text insets.
void RELabel::setTextInsets(const REEdgeInsets & newInsets)
{
	_textInsets = newInsets;
	
	this->reloadChars();
}

/// Returns text color.
const REColor & RELabel::getTextColor() const
{
	return _textColor;
}

/// Setting text color.
void RELabel::setTextColor(const REColor & newTextColor)
{
	_textColor = newTextColor;
}

/// Is showing label background using view color and texture.
REBOOL RELabel::isShowBackground() const
{
	return _isShowBackground;
}

/// Setting showing label background using view color and texture.
void RELabel::setShowBackground(REBOOL isShowBackground)
{
	_isShowBackground = isShowBackground;
}

void RELabel::onReleased()
{
	RE_SAFE_RELEASE(_font);
	
	REView::onReleased();
}

RELabel::RELabel() : REView(),
	_font(NULL),
	_charsSpaceRatio(1.0f),
	_isUsingShadow(false),
	_isShowBackground(false),
	_textAlignment(0),
	_lineBreakMode(0)
{
	_textLineTruncationString = "...";
}

RELabel::~RELabel()
{
	
}

RELabel * RELabel::create()
{
	RELabel * newLabel = new RELabel();
	return newLabel;
}

const char * RELabel::getXMLTextKeyString() { return RE_LABEL_XML_TEXT_KEY_STRING; }
const char * RELabel::getXMLTextColorKeyString() { return RE_LABEL_XML_TEXT_COLOR_KEY_STRING; }
const char * RELabel::getXMLTextInsetsKeyString() { return RE_LABEL_XML_TEXT_INSETS_KEY_STRING; }
const char * RELabel::getXMLCharsSpaceRatioKeyString() { return RE_LABEL_XML_CHARS_SPACE_RATIO_KEY_STRING; }
const char * RELabel::getXMLCharsSpaceRatioFormatString() { return RE_LABEL_XML_CHARS_SPACE_RATIO_FORMAT_STRING; }
const char * RELabel::getXMLShadowOffsetKeyString() { return RE_LABEL_XML_SHADOW_OFFSET_KEY_STRING; }
const char * RELabel::getXMLShadowOffsetFormatString() { return RE_LABEL_XML_SHADOW_OFFSET_FORMAT_STRING; }
const char * RELabel::getXMLUsingShadowKeyString() { return RE_LABEL_XML_USING_SHADOW_KEY_STRING; }
const char * RELabel::getXMLUsingShadowFormatString() { return RE_LABEL_XML_USING_SHADOW_FORMAT_STRING; }
const char * RELabel::getXMLShowBackgroundKeyString() { return RE_LABEL_XML_SHOW_BACKGROUND_KEY_STRING; }
const char * RELabel::getXMLShowBackgroundFormatString() { return RE_LABEL_XML_SHOW_BACKGROUND_FORMAT_STRING; }
const char * RELabel::getXMLTextAlignmentKeyString() { return RE_LABEL_XML_TEXT_ALIGNMENT_KEY_STRING; }
const char * RELabel::getXMLTextAlignmentStringByType(const RETextAlignment alignmentType)
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
const char * RELabel::getXMLLineBreakKeyString() { return RE_LABEL_XML_LINE_BREAK_KEY_STRING; }
const char * RELabel::getXMLLineBreakStringByType(const RETextLineBreak lineBreakType)
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
const char * RELabel::getXMLTextLineTruncationStringKeyString() { return RE_LABEL_XML_TEXT_LINE_TRANC_STRING_KEY_STRING; }
const char * RELabel::getXMLFontObjectKeyString() { return RE_LABEL_XML_FONT_OBJECT_KEY_STRING; }
