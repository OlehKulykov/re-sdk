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


#ifndef __RETTFFONTLOADER_H__
#define __RETTFFONTLOADER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REBuffer.h"
#include "../recore/REArray.h"
#include "../recore/REObjectsArray.h"
#include "../recore/REImage.h"
#include "RETTFFontChar.h"
#include "RETextureObject.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__))
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

class __RE_PUBLIC_CLASS_API__ RETTFFontLoader
{
private:
	typedef struct _imageAndTextureStruct
	{
		REImageBase * image;
		RETextureObject * texture;
	} ImageAndTextureStruct;
	RETTFFontLoader::ImageAndTextureStruct _imgTex;
	void * _library;
	void * _face;
	REArray<RETTFFontChar*> * _chars;
	REFloat32 _fontHeight;
	REFloat32 _gammaCorectionValue;
	REUInt32 _fontHeightPX;
	REUInt32 _fontDPI;
	REUInt32 _totalSquare;
	REUInt32 _processedSquare;
	REUInt16 _lastX;
	REUInt16 _lastY;
	REUInt16 _lineFirstCharHeight;
	REBOOL _isHasKerning;
	REBOOL _isUseGammaCorection;
	
	void UpdateLastTexture();
	void CreateNewTextureMainThreadMethod(REObject * mainThreadTask);
	void UpdateTextureMainThreadMethod(REObject * mainThreadTask);
	void PrepareForWriteChar(const REUInt32 cW, const REUInt32 cH);
	static REBOOL IsCanWrite(REImageBase * img,const REUInt32 pX,const REUInt32 pY,const REUInt32 cW, const REUInt32 cH);
	void CreateNewImage();
	void WriteGlyphToImage(RETTFFontChar * ttfChar);
	static void CalcTextureSize(RESize * retSize, const REUInt32 totalSquare);
	void * CreateLib();
	void * CreateFace(const REBuffer & ttfFileBuffer);
	void GetInfo();
	void ReadChars();
	void ReadGlyphs();
	void ProcessReadedGlyphs();
	
	static int CompareByCharCode(RETTFFontChar ** c1, RETTFFontChar ** c2);
	static int CompareByHeight(RETTFFontChar ** c1, RETTFFontChar ** c2);
	
public:
	void SetUseGammaCorection(REBOOL isUseCorrection) { _isUseGammaCorection = isUseCorrection; }
	REBOOL IsUseGammaCorection() const { return _isUseGammaCorection; }
	const REFloat32 GetGammaCorectionValue() const { return _gammaCorectionValue; }
	void SetGammaCorectionValue(const REFloat32 newValue) { _gammaCorectionValue = newValue; }
	const REUInt32 GetFontDPI() const { return _fontDPI; }
	void SetFontDPI(const REUInt32 newDPI);
	const REUInt32 GetGlyphsCount() const;
	const char * GetFamilyName() const;
	const char * GetStyleName() const;
	const REFloat32 GetFontHeight() const { return _fontHeight; }
	const REUInt32 GetFontHeightInPixels() const { return _fontHeightPX; }
	
	REBOOL IsCanLoad() const;
	REBOOL LoadChars(REArray<RETTFFontChar*> * charsArray);
	
	RETTFFontLoader(const REBuffer & ttfFileBuffer, const REFloat32 fontHeight);
	~RETTFFontLoader();
	
	static const REUInt32 GetFontHeightInPixels(const REFloat32 height);
};


#endif /* __RETTFFONTLOADER_H__ */


