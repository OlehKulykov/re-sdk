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
	
	REBOOL isCanReloadChars() const;
	void reloadChars();
	void layoutChars();
	void renderAtWithShadow(const REFloat32 x, const REFloat32 y);
	void renderAtWithOutShadow(const REFloat32 x, const REFloat32 y);
	void renderAt(const REFloat32 x, const REFloat32 y);
	RELabel();
	virtual ~RELabel();
private:
    static REBOOL acceptLabelStringParameter(RELabel * label, const char * key, const char * value);

public:
	/// Returns text frame from top left coord as [0.0f, 0.0f]
	/// using text insets, text breaking and text aligment.
	RERect getTextFrame() const;
	
	/// Is showing label background using view color and texture.
	REBOOL isShowBackground() const;
	
	/// Setting showing label background using view color and texture.
	virtual void setShowBackground(REBOOL isShowBackground);
	
	/// Returns text color.
	const REColor & getTextColor() const;
	
	/// Setting text color.
	virtual void setTextColor(const REColor & newTextColor);
	
	/// Getting label text insets.
	const REEdgeInsets & getTextInsets() const;
	
	/// Setting new label text insets.
	virtual void setTextInsets(const REEdgeInsets & newInsets);
	
	/// Returns type of text aligment.
	const RETextAlignment getTextAlignment() const;
	
	/// Setting type of text aligment.
    virtual void setTextAlignment(const RETextAlignment newTextAlignment);
	
	/// Returns type of text breaking.
	const RETextLineBreak getLineBreak() const;
	
	/// Setting type of text breaking.
    virtual void setLineBreak(const RETextLineBreak newLineBreak);
	
	/// Returns shadow offset.
	const REPoint2 & getShadowOffset() const { return _shadowOffset; }
	
	/// Setting shadow offset.
    virtual void setShadowOffset(const REPoint2 & newOffset);
	
	/// Returns color of shadow.
	const REColor & getShadowColor() const { return _shadowColor; }
	
	/// Setting shadow color.
    virtual void setShadowColor(const REColor & newColor);
	
	/// Returns is rendering text with shadow.
	REBOOL isUsingShadow() const { return _isUsingShadow; }
	
	/// Setting using shadowing on text rendering.
    virtual void setUsingShadow(REBOOL isUse) { _isUsingShadow = isUse; }
	
	/// Returns ratio of characters space. Default is 1.0f.
	const REFloat32 getCharsSpaceRatio() const { return _charsSpaceRatio; }
	
	/// Setting new ratio of characters space.
    virtual void setCharsSpaceRatio(const REFloat32 newRatio);
	
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual void onReleased();
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/// Setting rectangular frame of view.
	virtual void setFrame(const RERect & newViewFrame);
	
	/// Returns label text.
	const REString & getText() const { return _text; }
	
	/// Setting label text.
    virtual void setText(const REString & newText);
	
	/// Returns string wich replaces on truncation.
	/// Default is three dots.
	const REString & getTextLineTruncationString() const;
	
	/// Setting new custom truncation string.
    virtual void setTextLineTruncationString(const REString & newTruncationString);
	
	/// Returns label font object.
	REFontObject * getFont() const { return _font; }
	
	/// Setting label font object.
    virtual void setFont(REFontObject * newFont);
	
	/* REGUIObject */
	virtual REBOOL acceptStringParameter(const char * key, const char * value);
	virtual REBOOL acceptObjectParameter(const char * className, const char * key, REGUIObject * value);
	
	/// Creates and returns new label object.
	static RELabel * create();

	/// Returns XML key string for text.
    static const char * getXMLTextKeyString();
	
	/// Returns XML key string for text color.
    static const char * getXMLTextColorKeyString();
		
	/// Returns XML key string for text insets.
    static const char * getXMLTextInsetsKeyString();
	
	/// Returns XML key string for characters space ratio.
    static const char * getXMLCharsSpaceRatioKeyString();
	
	/// Returns XML format string for characters space ratio.
    static const char * getXMLCharsSpaceRatioFormatString();
	
	/// Returns XML key string for shadow offset.
    static const char * getXMLShadowOffsetKeyString();
	
	/// Returns XML format string for shadow offset.
    static const char * getXMLShadowOffsetFormatString();
	
	/// Returns XML key string for flag using shadow.
    static const char * getXMLUsingShadowKeyString();
	
	/// Returns XML format string for flag using shadow.
    static const char * getXMLUsingShadowFormatString();
	
	/// Returns XML key string for flag show background.
    static const char * getXMLShowBackgroundKeyString();
	
	/// Returns XML format string for flag show background.
    static const char * getXMLShowBackgroundFormatString();
	
	/// Returns XML key string for text alignment.
    static const char * getXMLTextAlignmentKeyString();
	
	/// Returns XML string for text alignment type.
    static const char * getXMLTextAlignmentStringByType(const RETextAlignment alignmentType);
	
	/// Returns XML key string for line break mode.
    static const char * getXMLLineBreakKeyString();
	
	/// Returns XML string for line break mode by it's type.
    static const char * getXMLLineBreakStringByType(const RETextLineBreak lineBreakType);
	
	/// Returns XML key string for text line truncation string.
    static const char * getXMLTextLineTruncationStringKeyString();
	
	/// Returns XML key string for font object.
    static const char * getXMLFontObjectKeyString();
};



#endif /* __RELABEL_H__ */


