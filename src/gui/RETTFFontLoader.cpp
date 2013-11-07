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


#include "../../include/regui/RETTFFontLoader.h"
#include "../../include/regui/RERenderDevice.h"
#include "../../include/recore/REMath.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REThread.h"
#include "../../include/recore/REMem.h"


#if defined(__RE_USING_ADITIONAL_FREETYPE_LIBRARY__)
#include "../addlibs/freetype.h"
#elif defined(__RE_USING_SYSTEM_FREETYPE_LIBRARY__)
#include <ft2build.h>
#include <freetype/freetype.h>
#else
#define __RE_NO_FREETYPE_LIBRARY_PRIVATE__
#endif


#define CHARS_HORIZONTAL_SPACE (2)
#define CHARS_VERTICAL_SPACE (2)

/*

 http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
 http://code.google.com/p/freetype-gl/

*/

void RETTFFontLoader::updateLastTexture()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	_imgTex.texture->update(_imgTex.image->imageData(), 
							_imgTex.image->format(), 
							_imgTex.image->width(), 
							_imgTex.image->height());
	
	delete _imgTex.image;
	_imgTex.image = NULL;
	
	_imgTex.texture->release();
	_imgTex.texture = NULL;
		
		/*
		REImageManager m;
		REBufferObject * data = m.CreatePNGFilePresentation(_imgTex.image->GetImageData(),
															_imgTex.image->GetWidth(), 
															_imgTex.image->GetHeight(), 
															_imgTex.image->GetFormat());
		if (data)
		{
			REString p("/Users/residentevil/TEMP/");
			p.AppendFormat("char%u.png", (REUInt32)data);
			FILE * f = REFile::FOpen(p, "wb");
			if (f) 
			{
				fwrite(data->GetBuffer(), 1, data->GetSize(), f);
				fclose(f);
			}
			data->release();
		}
		*/
#endif
}

int RETTFFontLoader::compareByHeight(RETTFFontChar ** c1, RETTFFontChar ** c2)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	return ((int)(*c2)->bitmapHeight - (int)(*c1)->bitmapHeight);
#else
	return 0;
#endif
}

int RETTFFontLoader::compareByCharCode(RETTFFontChar ** c1, RETTFFontChar ** c2)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	return ((int)(*c1)->charCode() - (int)(*c2)->charCode());
#else
	return 0;
#endif
}

void RETTFFontLoader::calcTextureSize(RESize * retSize, const REUInt32 totalSquare)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	const REUInt32 sizes[5] = { 32, 64, 128, 256, 512 };
	for (REUInt32 w = 0; w < 5; w++) 
	{
		for (REUInt32 h = 0; h < 5; h++) 
		{
			if ( totalSquare < (sizes[w]*sizes[h]) ) 
			{
				if (sizes[w] >= sizes[h]) 
				{
					retSize->width = (REFloat32)sizes[w];
					retSize->height = (REFloat32)sizes[h];
				}
				else
				{
					retSize->width = (REFloat32)sizes[h];
					retSize->height = (REFloat32)sizes[w];
				}
				return;
			}
		}
	}
	retSize->width = 512.0f;
	retSize->height = 512.0f;
#endif
}

void RETTFFontLoader::createNewImage()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	RESize needSize;
	RETTFFontLoader::calcTextureSize(&needSize, (_totalSquare - _processedSquare));
	
	REImageBase * newBase = new REImageBase(REImagePixelFormatAlpha8, 
											(REUInt32)needSize.width, 
											(REUInt32)needSize.height);
	if (newBase) 
	{
		if ( !newBase->isNull() )
		{
			RETexture * newTexture = RETexture::createBlankTexture();
			if (newTexture) 
			{
				_imgTex.image = newBase;
				_imgTex.texture = newTexture;
				const REUInt32 dataSize = newBase->imageDataSize();
				memset(newBase->imageData(), 0, dataSize);
				return;
			}
			
			if (newTexture) { newTexture->release(); }
		}
		
		if (newBase) { delete newBase; }
	}
	
	_imgTex.image = NULL;
	_imgTex.texture = NULL;
#endif
}

REBOOL RETTFFontLoader::isCanWrite(REImageBase * img,const REUInt32 pX,const REUInt32 pY,const REUInt32 cW, const REUInt32 cH)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if ( (pX + cW) > (img->width() - 1) ) 
	{
		return false;
	}
	if ( (pY + cH) > (img->height() - 1) ) 
	{
		return false;
	}
#endif
	return true;
}

void RETTFFontLoader::prepareForWriteChar(const REUInt32 cW, const REUInt32 cH)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_imgTex.image == NULL) 
	{
		_lastX = CHARS_HORIZONTAL_SPACE;
		_lastY = CHARS_VERTICAL_SPACE;
		_lineFirstCharHeight = cH;
		this->createNewImage();
		if (_imgTex.image == NULL) { return; }
	}
	
	if ( RETTFFontLoader::isCanWrite(_imgTex.image, _lastX, _lastY, cW, cH) ) { return; }
	
	
	//new line
	const REUInt32 newPosX = CHARS_HORIZONTAL_SPACE;
	const REUInt32 newPosY = _lastY + _lineFirstCharHeight + CHARS_VERTICAL_SPACE;
	if (RETTFFontLoader::isCanWrite(_imgTex.image, newPosX, newPosY, cW, cH))
	{
		_lastX = newPosX;
		_lastY = newPosY;
		_lineFirstCharHeight = cH;
	}
	else
	{
		//new image for texture
		if (_imgTex.image && _imgTex.texture) 
		{
			this->updateLastTexture();
		}
		
		_lastX = CHARS_HORIZONTAL_SPACE;
		_lastY = CHARS_VERTICAL_SPACE;
		_lineFirstCharHeight = cH;
		this->createNewImage();
	}
#endif
}

void RETTFFontLoader::writeGlyphToImage(RETTFFontChar * ttfChar)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	const REUInt32 charWidth = (REUInt32)ttfChar->bitmapWidth;
	const REUInt32 charHeight = (REUInt32)ttfChar->bitmapHeight;
	if ( (charWidth == 0) || (charHeight == 0) ) { return; }
	if (ttfChar->bitmap == NULL) 
	{
		//TODO: write empty buffer
		return; 
	}
	
	this->prepareForWriteChar(charWidth, charHeight);
	if (_imgTex.image == NULL) { return; }
	
	const REUInt32 imgWidth = _imgTex.image->width();
	const REFloat32 floatImageWidth = (REFloat32)imgWidth;
	const REFloat32 floatImageHeight = (REFloat32)_imgTex.image->height();
	ttfChar->textureFrame.bottomLeftX = ttfChar->textureFrame.topLeftX = ((REFloat32)_lastX) / floatImageWidth;
	ttfChar->textureFrame.topRightY = ttfChar->textureFrame.topLeftY = ((REFloat32)_lastY) / floatImageHeight;
	ttfChar->textureFrame.topRightX = ttfChar->textureFrame.bottomRightX = ((REFloat32)(charWidth + _lastX)) / floatImageWidth;
	ttfChar->textureFrame.bottomLeftY = ttfChar->textureFrame.bottomRightY = ((REFloat32)(charHeight + _lastY)) / floatImageHeight;

	if (_imgTex.texture) 
	{
		ttfChar->texture = _imgTex.texture;
		ttfChar->texture->retain();
	}
	
	if (_isUseGammaCorection) 
	{
		REUInt32 dstWriteY = _lastY;
		const REUByte * bitmap_buffer = (REUByte *)ttfChar->bitmap;
		for (REUInt32 y = 0; y < charHeight; y++)
		{
			REUByte * dst = _imgTex.image->imageData();
			dst += ((dstWriteY * imgWidth) + _lastX);
			for (REUInt32 x = 0; x < charWidth; x++)
			{
				const REUByte c = *((REUByte*)(bitmap_buffer + (y*ttfChar->bitmapPitch) + x));
				(*dst) = (REUByte)(powf(c/255.0f, _gammaCorectionValue) * 255);
				dst++;
			}
			dstWriteY++;
		}
	}
	else
	{
		const REUByte * bitmap_buffer = (REUByte *)ttfChar->bitmap;
		for (REUInt32 y = 0; y < charHeight; y++)
		{
			REUByte * dst = _imgTex.image->imageData();
			dst += (((y+_lastY)*imgWidth) + _lastX);
			memcpy(dst, bitmap_buffer + (y*ttfChar->bitmapPitch), charWidth);
		}
	}
	free(ttfChar->bitmap);
	ttfChar->bitmap = NULL;
	_lastX += (charWidth + CHARS_HORIZONTAL_SPACE);
	_processedSquare += (((REUInt32)ttfChar->bitmapWidth + CHARS_HORIZONTAL_SPACE) * 
						 ((REUInt32)ttfChar->bitmapHeight + CHARS_VERTICAL_SPACE));
#endif
}

void RETTFFontLoader::processReadedGlyphs()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	_chars->sort(RETTFFontLoader::compareByHeight);
	
	_processedSquare = 0;
	for (REUInt32 i = 0; i < _chars->count(); i++) 
	{
		RETTFFontChar * loadedChar = (*_chars)[i];
		this->writeGlyphToImage(loadedChar);
	}
	
	if (_imgTex.image && _imgTex.texture) 
	{
		this->updateLastTexture();
	}
	
	_chars->sort(RETTFFontLoader::compareByCharCode);
#endif
}

void RETTFFontLoader::readGlyphs()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	_totalSquare = 0;
	FT_Face * face = (FT_Face *)_face;
	for (REUInt32 i = 0; i < _chars->count(); i++) 
	{
		RETTFFontChar * loadedChar = (*_chars)[i];
		const FT_UInt glyph_index = FT_Get_Char_Index(*face, (FT_ULong)loadedChar->charCode());
		//if (loadedChar->GetCharCode() == L'g') { printf("\nchar=g"); }
		if (glyph_index != 0) 
		{
			const FT_Int32 flags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;
			FT_Error error = FT_Load_Glyph(*face, glyph_index, flags);
			if (error == 0) 
			{
				FT_GlyphSlot slot = (*face)->glyph;
				//const REInt32 bearingY = (REInt32)(slot->metrics.vertBearingY >> 6);
				loadedChar->offsetX = (REFloat32)slot->bitmap_left;
				loadedChar->offsetY = (REFloat32)slot->bitmap_top;
				loadedChar->width = (REFloat32)slot->bitmap.width;
				loadedChar->height = (REFloat32)slot->bitmap.rows;
				
				const REUInt32 bitmapDataSize = (slot->bitmap.width * slot->bitmap.rows);
				loadedChar->bitmap = malloc(bitmapDataSize);
				if (loadedChar->bitmap) 
				{
					memcpy(loadedChar->bitmap, slot->bitmap.buffer, bitmapDataSize);
					loadedChar->bitmapWidth = (REUByte)slot->bitmap.width;
					loadedChar->bitmapHeight = (REUByte)slot->bitmap.rows;
					loadedChar->bitmapPitch = (REUByte)slot->bitmap.pitch;
					
					_totalSquare += (((REUInt32)loadedChar->bitmapWidth + CHARS_HORIZONTAL_SPACE) * 
									 ((REUInt32)loadedChar->bitmapHeight + CHARS_VERTICAL_SPACE));
				}
				
				FT_Load_Glyph( (*face), glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
				
				slot = (*face)->glyph;
				
				loadedChar->advanceX = (REFloat32)(slot->advance.x >> 6);
				loadedChar->advanceY = (REFloat32)(slot->advance.y >> 6);
				
				//FT_Done_Glyph((FT_Glyph)slot);
			}
		}
	}
#endif
}

void RETTFFontLoader::readChars()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	const REUInt32 numGlyphs = this->getGlyphsCount();
	if (numGlyphs == 0) { return; }
	
	_chars->setCapacity(numGlyphs + 1);
	FT_Face * face = (FT_Face *)_face;
	FT_UInt gindex = 0;
	FT_ULong charcode = FT_Get_First_Char(*face, &gindex);
	//printf("\n\nReadChars: ");
	while ( gindex != 0 ) 
	{
		RETTFFontChar * newChar = new RETTFFontChar((wchar_t)charcode);
		if (newChar) 
		{
			if ( !_chars->add(newChar) ) 
			{
				delete newChar;
			}
			//printf("%u, ", (REUInt32)charcode);
		}
		charcode = FT_Get_Next_Char(*face, charcode, &gindex);
	}
#endif
}

REBOOL RETTFFontLoader::isCanLoad() const
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_library && _face) { return true; }
#endif
	return false;
}

void * RETTFFontLoader::createLib()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	FT_Library * library = (FT_Library *)malloc(sizeof(FT_Library));
	if (library) 
	{
		memset(library, 0, sizeof(FT_Library));
		if ( FT_Init_FreeType(library) == 0) 
		{
			return library;
		}
		free(library);
	}
#endif
	return NULL;
}

void * RETTFFontLoader::createFace(const REData & ttfFileData)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	FT_Face * face = (FT_Face *)malloc(sizeof(FT_Face));
	if (face) 
	{
		memset(face, 0, sizeof(FT_Face));
		FT_Library * library = (FT_Library *)_library;
		if (FT_New_Memory_Face(*library, 
							   (const FT_Byte*)ttfFileData.bytes(), 
							   (FT_Long)ttfFileData.size(), 
							   0, 
							   face ) == 0)
		{
			FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
			
			const REUInt32 heightInPixels = this->getFontHeightInPixels();
			//size_t hres = 16;
			//hres = 1;
			//FT_Set_Char_Size(*face, heightInPixels * 64, heightInPixels * 64, 96, 96);
			
			const FT_UInt dpi = RERenderDevice::defaultDevice()->screenResolution();
			FT_Set_Char_Size(*face, 0, (heightInPixels << 6), dpi, dpi);
			/*
			FT_Matrix matrix = { (int)((1.0/hres) * 0x10000L),
				(int)((0.0)      * 0x10000L),
				(int)((0.0)      * 0x10000L),
				(int)((1.0)      * 0x10000L) };
			
			FT_Set_Transform( *face, &matrix, NULL );
			*/
			return face;
		}
		free(face);
	}
#endif
	return NULL;
}

void RETTFFontLoader::getInfo()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	FT_Face * face = (FT_Face *)_face;
	_isHasKerning = (FT_HAS_KERNING((*face)) != 0);
#endif
}

const REUInt32 RETTFFontLoader::getGlyphsCount() const
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_face) 
	{
		FT_Face * face = (FT_Face *)_face;
		const FT_Long num = (*face)->num_glyphs;
		if (num > 0) { return ((REUInt32)num); }
	}
#endif
	return 0;
}

const char * RETTFFontLoader::getFamilyName() const
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_face) 
	{
		FT_Face * face = (FT_Face *)_face;
		return (const char*)((*face)->family_name);
	}
#endif
	return NULL;
}

const char * RETTFFontLoader::getStyleName() const
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_face) 
	{
		FT_Face * face = (FT_Face *)_face;
		return (const char*)((*face)->style_name);
	}
#endif
	return NULL;
}

REBOOL RETTFFontLoader::loadChars(REArray<RETTFFontChar *> * charsArray)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (this->isCanLoad() && charsArray) 
	{
		_chars = charsArray;
		this->readChars();
		this->readGlyphs();
		this->processReadedGlyphs();
		_chars = NULL;
		return true;
	}
#endif
	return false;
}

const REUInt32 RETTFFontLoader::getFontHeightInPixels(const REFloat32 height)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	RERenderDevice * device = RERenderDevice::defaultDevice();
	if (device) 
	{
		RESize ratio(device->renderToScreenSizeRatio());
		const REUInt32 h = (REUInt32)(ratio.height * height);
		return h;
	}
#endif
	return 0;
}

RETTFFontLoader::RETTFFontLoader(const REData & ttfFileData, const REFloat32 fontHeight) :
	_library(NULL),
	_face(NULL),
	_chars(NULL),
	_fontHeight(fontHeight),
	_gammaCorectionValue(1.5f),
	_fontHeightPX(0),
	_totalSquare(0),
	_processedSquare(0),
	_isHasKerning(false),
	_isUseGammaCorection(false)
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	_imgTex.image = NULL;
	_imgTex.texture = NULL;
	
	if ((ttfFileData.size() > 0) && (_fontHeight > 0.0f))
	{
		_fontHeightPX = RETTFFontLoader::getFontHeightInPixels(_fontHeight);
		
		_library = this->createLib();
		if (_library) 
		{
			_face = this->createFace(ttfFileData);
			if (_face) 
			{
				this->getInfo();
			}
		}
	}
#endif
}

RETTFFontLoader::~RETTFFontLoader()
{
#ifndef __RE_NO_FREETYPE_LIBRARY_PRIVATE__
	if (_face) 
	{
		FT_Face * face = (FT_Face *)_face;
		FT_Done_Face(*face);
		free(_face);
	}
	
	if (_library) 
	{
		FT_Library * lib = (FT_Library *)_library;
		FT_Done_FreeType(*lib);
		free(_library);
	}

	if (_imgTex.image) 
	{
		delete _imgTex.image;
	}
	
	if (_imgTex.texture) 
	{
		_imgTex.texture->release();
	}
#endif
}


