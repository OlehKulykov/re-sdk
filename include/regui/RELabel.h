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


#ifndef __RELABEL_H__
#define __RELABEL_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REString.h"
#include "../recore/REPoint2.h"
#include "../recore/REEdgeInsets.h"
#include "REView.h"
#include "REFontObject.h"

/// Types of label text aligment.
typedef enum _reTextAlignment
{
	/// Alignes text to the left of frame.
	RETextAlignmentLeft = 0,
	
	/// Alignes text on the center of frame.
	RETextAlignmentCenter = 1,
	
	/// Alignes text to the right of frame.
	RETextAlignmentRight = 2
}
/// Types of label text aligment.
RETextAlignment;


/// Types of text breaking.
typedef enum _reTextLineBreak
{
	/// If text is out of frame it's simply truncates end of the text.
	RETextLineBreakNone = 0,
	
	/// If text If text is out of frame it's truncates end of the text and place 3 dots.
	RETextLineBreakTruncateTail = 1,
	
	/// If text If text is out of frame it's truncates middle(center) of the text and place 3 dots.
	RETextLineBreakTruncateMiddle = 2,
	
	/// If text If text is out of frame it's truncates beginning of the text and place 3 dots.
	RETextLineBreakTruncateHead = 3
} 
/// Types of text breaking.
RETextLineBreak;

/// Class using for displaying text.
class __RE_PUBLIC_CLASS_API__ RELabel : public REView
{
protected:
	REString _text;
	REString _textLineTruncationString;
	REFontObject * _font;
	REArray<RETTFFontChar *> _chars;
	REColor _textColor;
	RESize _charsSize;
	REEdgeInsets _textInsets;
	REFloat32 _charsSpaceRatio;
	
	REPoint2 _shadowOffset;
	REColor _shadowColor;
	REBOOL _isUsingShadow;
	REBOOL _isShowBackground;
	REUByte _textAlignment;
	REUByte _lineBreakMode;
	
	REBOOL IsCanReloadChars() const;
	void ReloadChars();
	void LayoutChars();
	void RenderAtWithShadow(const REFloat32 x, const REFloat32 y);
	void RenderAtWithOutShadow(const REFloat32 x, const REFloat32 y);
	void RenderAt(const REFloat32 x, const REFloat32 y);
	RELabel();
	virtual ~RELabel();
private:
    static REBOOL AcceptLabelStringParameter(RELabel * label, const char * key, const char * value);

public:
	/// Returns text frame from top left coord as [0.0f, 0.0f]
	/// using text insets, text breaking and text aligment.
	RERect GetTextFrame() const;
	
	/// Is showing label background using view color and texture.
	REBOOL IsShowBackground() const;
	
	/// Setting showing label background using view color and texture.
	virtual void SetShowBackground(REBOOL isShowBackground);
	
	/// Returns text color.
	const REColor & GetTextColor() const;
	
	/// Setting text color.
	virtual void SetTextColor(const REColor & newTextColor);
	
	/// Getting label text insets.
	const REEdgeInsets & GetTextInsets() const;
	
	/// Setting new label text insets.
	virtual void SetTextInsets(const REEdgeInsets & newInsets);
	
	/// Returns type of text aligment.
	const RETextAlignment GetTextAlignment() const;
	
	/// Setting type of text aligment.
    virtual void SetTextAlignment(const RETextAlignment newTextAlignment);
	
	/// Returns type of text breaking.
	const RETextLineBreak GetLineBreak() const;
	
	/// Setting type of text breaking.
    virtual void SetLineBreak(const RETextLineBreak newLineBreak);
	
	/// Returns shadow offset.
	const REPoint2 & GetShadowOffset() const { return _shadowOffset; }
	
	/// Setting shadow offset.
    virtual void SetShadowOffset(const REPoint2 & newOffset);
	
	/// Returns color of shadow.
	const REColor & GetShadowColor() const { return _shadowColor; }
	
	/// Setting shadow color.
    virtual void SetShadowColor(const REColor & newColor);
	
	/// Returns is rendering text with shadow.
	REBOOL IsUsingShadow() const { return _isUsingShadow; }
	
	/// Setting using shadowing on text rendering.
    virtual void SetUsingShadow(REBOOL isUse) { _isUsingShadow = isUse; }
	
	/// Returns ratio of characters space. Default is 1.0f.
	const REFloat32 GetCharsSpaceRatio() const { return _charsSpaceRatio; }
	
	/// Setting new ratio of characters space.
    virtual void SetCharsSpaceRatio(const REFloat32 newRatio);
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* IRERenderable */
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/// Setting rectangular frame of view.
	virtual void SetFrame(const RERect & newViewFrame);
	
	/// Returns label text.
	const REString & GetText() const { return _text; }
	
	/// Setting label text.
    virtual void SetText(const REString & newText);
	
	/// Returns string wich replaces on truncation.
	/// Default is three dots.
	const REString & GetTextLineTruncationString() const;
	
	/// Setting new custom truncation string.
    virtual void SetTextLineTruncationString(const REString & newTruncationString);
	
	/// Returns label font object.
	REFontObject * GetFont() const { return _font; }
	
	/// Setting label font object.
    virtual void SetFont(REFontObject * newFont);
	
	/* REGUIObject */
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	virtual REBOOL AcceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/// Creates and returns new label object.
	static RELabel * Create();

	/// Returns XML key string for text.
    static const char * GetXMLTextKeyString();
	
	/// Returns XML key string for text color.
    static const char * GetXMLTextColorKeyString();
	
	/// Returns XML format string for text color.
    static const char * GetXMLTextColorFormatString();
	
	/// Returns XML key string for text insets.
    static const char * GetXMLTextInsetsKeyString();
	
	/// Returns XML format string for text insets.
    static const char * GetXMLTextInsetsFormatString();
	
	/// Returns XML key string for characters space ratio.
    static const char * GetXMLCharsSpaceRatioKeyString();
	
	/// Returns XML format string for characters space ratio.
    static const char * GetXMLCharsSpaceRatioFormatString();
	
	/// Returns XML key string for shadow color.
    static const char * GetXMLShadowColorKeyString();
	
	/// Returns XML format string for shadow color.
    static const char * GetXMLShadowColorFormatString();
	
	/// Returns XML key string for shadow offset.
    static const char * GetXMLShadowOffsetKeyString();
	
	/// Returns XML format string for shadow offset.
    static const char * GetXMLShadowOffsetFormatString();
	
	/// Returns XML key string for flag using shadow.
    static const char * GetXMLUsingShadowKeyString();
	
	/// Returns XML format string for flag using shadow.
    static const char * GetXMLUsingShadowFormatString();
	
	/// Returns XML key string for flag show background.
    static const char * GetXMLShowBackgroundKeyString();
	
	/// Returns XML format string for flag show background.
    static const char * GetXMLShowBackgroundFormatString();
	
	/// Returns XML key string for text alignment.
    static const char * GetXMLTextAlignmentKeyString();
	
	/// Returns XML string for text alignment type.
    static const char * GetXMLTextAlignmentStringByType(const RETextAlignment alignmentType);
	
	/// Returns XML key string for line break mode.
    static const char * GetXMLLineBreakKeyString();
	
	/// Returns XML string for line break mode by it's type.
    static const char * GetXMLLineBreakStringByType(const RETextLineBreak lineBreakType);
	
	/// Returns XML key string for text line truncation string.
    static const char * GetXMLTextLineTruncationStringKeyString();
	
	/// Returns XML key string for font object.
    static const char * GetXMLFontObjectKeyString();
};



#endif /* __RELABEL_H__ */


