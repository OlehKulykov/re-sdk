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


#include "../../include/recore/REColor.h"

//#ifdef __RE_OS_BADA__
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <wctype.h>
//#endif /* __RE_OS_BADA__ */


class REColorPrivate
{
	
public:
	static void SetColorFromHEXString(REColor * color, const char * hexString);
};

void REColorPrivate::SetColorFromHEXString(REColor * color, const char * hexString)
{
	if ( strlen(hexString) >= 7 )
	{
		char buff[8] = { 0 };
		for ( int i = 0; i < 7; i++ )
		{
			buff[i] = tolower(hexString[i]);
		}
		REUInt32 redByte = 0;
		REUInt32 greenByte = 0;
		REUInt32 blueByte = 0;
#if defined(__RE_OS_WINDOWS__) && defined(_MSC_VER)
		int readed = sscanf_s(buff, "#%2x%2x%2x", &redByte, &greenByte, &blueByte);
#else
		int readed = sscanf(buff, "#%2x%2x%2x", &redByte, &greenByte, &blueByte);
#endif
		if (readed == 3)
		{
			if (redByte > 255) { redByte = 255; }
			if (greenByte > 255) { greenByte = 255; }
			if (blueByte > 255) { blueByte = 255; }
			color->red = (REFloat32)redByte / 255.0f;
			color->green = (REFloat32)greenByte / 255.0f;
			color->blue = (REFloat32)blueByte / 255.0f;
			color->alpha = 1.0f;
		}
	}
}


void REColor::SetRGB(const REUByte newRed, const REUByte newGreen, const REUByte newBlue)
{
	red = (REFloat32)newRed / 255.0f;
	green = (REFloat32)newGreen / 255.0f;
	blue = (REFloat32)newBlue / 255.0f;
	alpha = 1.0f;
}

void REColor::SetRGBA(const REUByte newRed, const REUByte newGreen, const REUByte newBlue, const REUByte newAlpha)
{
	red = (REFloat32)newRed / 255.0f;
	green = (REFloat32)newGreen / 255.0f;
	blue = (REFloat32)newBlue / 255.0f;
	alpha = (REFloat32)newAlpha / 255.0f;
}

void REColor::SetHEX(const char * hexString)
{
	if (hexString)
	{
		REColorPrivate::SetColorFromHEXString(this, hexString);
	}
}

REColor::REColor(const REFloat32 newRed, const REFloat32 newGreen, const REFloat32 newBlue, const REFloat32 newAlpha) : 
	red(newRed),
	green(newGreen),
	blue(newBlue),
	alpha(newAlpha)	
{

}

REColor::REColor(const char * hexString) : 
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	alpha(1.0f)
{
	this->SetHEX(hexString);
}

REColor::REColor() : 
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	alpha(1.0f)
{

}

REColor::~REColor()
{

}
