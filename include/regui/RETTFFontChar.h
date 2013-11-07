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


#ifndef __RETTFFONTCHAR_H__
#define __RETTFFONTCHAR_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REBuffer.h"
#include "../recore/REArray.h"
#include "../recore/REArrayObject.h"
#include "../recore/REPoint2.h"
#include "../recore/RESize.h"
#include "../recore/RETetragon.h"
#include "../recore/REFloat16.h"
#include "RETexture.h"

class __RE_PUBLIC_CLASS_API__ RETTFFontChar
{
private:
	wchar_t _charCode;
public:
	RETexture * texture;
	void * bitmap;
	
	RETetragon textureFrame;
	
	REFloat16 advanceX;
	REFloat16 advanceY;
	
	REFloat16 offsetX;
	REFloat16 offsetY;
	
	REFloat16 width;
	REFloat16 height;
	
	REUByte bitmapWidth;
	REUByte bitmapHeight;
	REUByte bitmapPitch;
	
	const wchar_t charCode() const { return _charCode; }
	RETTFFontChar(const wchar_t charCode);
	~RETTFFontChar();
};


#endif /* __RETTFFONTCHAR_H__ */


