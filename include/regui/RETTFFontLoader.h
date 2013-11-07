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
#include "../recore/REArrayObject.h"
#include "../recore/REImage.h"
#include "RETTFFontChar.h"
#include "RETexture.h"

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
		RETexture * texture;
	} ImageAndTextureStruct;
	RETTFFontLoader::ImageAndTextureStruct _imgTex;
	void * _library;
	void * _face;
	REArray<RETTFFontChar*> * _chars;
	REFloat32 _fontHeight;
	REFloat32 _gammaCorectionValue;
	REUInt32 _fontHeightPX;
	REUInt32 _totalSquare;
	REUInt32 _processedSquare;
	REUInt16 _lastX;
	REUInt16 _lastY;
	REUInt16 _lineFirstCharHeight;
	REBOOL _isHasKerning;
	REBOOL _isUseGammaCorection;
	
	void updateLastTexture();
//	void createNewTextureMainThreadMethod(REObject * mainThreadTask);
//	void updateTextureMainThreadMethod(REObject * mainThreadTask);
	void prepareForWriteChar(const REUInt32 cW, const REUInt32 cH);
	static REBOOL isCanWrite(REImageBase * img,const REUInt32 pX,const REUInt32 pY,const REUInt32 cW, const REUInt32 cH);
	void createNewImage();
	void writeGlyphToImage(RETTFFontChar * ttfChar);
	static void calcTextureSize(RESize * retSize, const REUInt32 totalSquare);
	void * createLib();
	void * createFace(const REData & ttfFileData);
	void getInfo();
	void readChars();
	void readGlyphs();
	void processReadedGlyphs();
	
	static int compareByCharCode(RETTFFontChar ** c1, RETTFFontChar ** c2);
	static int compareByHeight(RETTFFontChar ** c1, RETTFFontChar ** c2);
	
public:
	void setUseGammaCorection(REBOOL isUseCorrection) { _isUseGammaCorection = isUseCorrection; }
	REBOOL isUseGammaCorection() const { return _isUseGammaCorection; }
	const REFloat32 getGammaCorectionValue() const { return _gammaCorectionValue; }
	void setGammaCorectionValue(const REFloat32 newValue) { _gammaCorectionValue = newValue; }
	const REUInt32 getGlyphsCount() const;
	const char * getFamilyName() const;
	const char * getStyleName() const;
	const REFloat32 getFontHeight() const { return _fontHeight; }
	const REUInt32 getFontHeightInPixels() const { return _fontHeightPX; }
	
	REBOOL isCanLoad() const;
	REBOOL loadChars(REArray<RETTFFontChar*> * charsArray);
	
	RETTFFontLoader(const REData & ttfFileData, const REFloat32 fontHeight);
	~RETTFFontLoader();
	
	static const REUInt32 getFontHeightInPixels(const REFloat32 height);
};


#endif /* __RETTFFONTLOADER_H__ */


