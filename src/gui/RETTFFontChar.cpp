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


#include "../../include/regui/RETTFFontChar.h"
#include "../../include/recore/REImage.h"
#include "../../include/recore/REImageManager.h"
#include "../../include/recore/REFile.h"

RETTFFontChar::RETTFFontChar(const wchar_t charCode) :
	_charCode(charCode),
	texture(NULL),
	bitmap(NULL),
	advanceX(0.0f),
	advanceY(0.0f),
	offsetX(0.0f),
	offsetY(0.0f),
	width(0.0f),
	height(0.0f),
	bitmapWidth(0),
	bitmapHeight(0),
	bitmapPitch(0)
{
	
}

RETTFFontChar::~RETTFFontChar()
{
	if (texture) 
	{
		texture->release();
	}
}


