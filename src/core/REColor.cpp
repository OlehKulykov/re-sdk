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
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif


class REColorPrivate
{
	
public:
	static void setColorFromHEXString(REColor * color, const char * hexString);
};

void REColorPrivate::setColorFromHEXString(REColor * color, const char * hexString)
{
	if (strlen(hexString) >= 7)
	{
		char buff[8] = { 0 };
		for (int i = 0; i < 7; i++)
		{
			buff[i] = tolower(hexString[i]);
		}
		REUInt32 redByte = 0;
		REUInt32 greenByte = 0;
		REUInt32 blueByte = 0;
		
#if defined(HAVE_FUNCTION_SSCANF_S)		
		const int readed = sscanf_s(buff, "#%2x%2x%2x", &redByte, &greenByte, &blueByte);
#else
		const int readed = sscanf(buff, "#%2x%2x%2x", &redByte, &greenByte, &blueByte);
#endif		
		if (readed == 3)
		{
			color->red = (redByte < 255) ? ((REFloat32)redByte) / 255.0f : 1.0f;
			color->green = (greenByte < 255) ? ((REFloat32)greenByte) / 255.0f : 1.0f;
			color->blue = (blueByte < 255) ? ((REFloat32)blueByte) / 255.0f : 1.0f;
			color->alpha = 1.0f;
		}
	}
}


void REColor::setRGB(const REUByte newRed, const REUByte newGreen, const REUByte newBlue)
{
	red = (REFloat32)newRed / 255.0f;
	green = (REFloat32)newGreen / 255.0f;
	blue = (REFloat32)newBlue / 255.0f;
	alpha = 1.0f;
}

void REColor::setRGBA(const REUByte newRed, const REUByte newGreen, const REUByte newBlue, const REUByte newAlpha)
{
	red = (REFloat32)newRed / 255.0f;
	green = (REFloat32)newGreen / 255.0f;
	blue = (REFloat32)newBlue / 255.0f;
	alpha = (REFloat32)newAlpha / 255.0f;
}

void REColor::setHEX(const char * hexString)
{
	if (hexString)
	{
		REColorPrivate::setColorFromHEXString(this, hexString);
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
	this->setHEX(hexString);
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

REColor REColor::fromString(const char * string)
{
	if (string) 
	{
		REColor c;
		if (REStringUtilsPrivate::readArrayF32(string, c.rgba, 4, ';') == 4) 
		{
			return c;
		}
	}
	return REColor();
}

REColor REColor::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REColor c;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), c.rgba, 4, ';') == 4) 
		{
			return c;
		}
	}
	return REColor();
}

REString REColor::toString(const REColor & color)
{
	char buff[64];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(color.rgba, buff, 4, ';');
	return REString(buff, len);
}

REColor REColor::redColor()
{
	return REColor(1.0f, 0.0f, 0.0f, 1.0f);
}

REColor REColor::greenColor()
{
	return REColor(0.0f, 1.0f, 0.0f, 1.0f);
}

REColor REColor::blueColor()
{
	return REColor(0.0f, 0.0f, 1.0f, 1.0f);
}

REColor REColor::blackColor()
{
	return REColor(0.0f, 0.0f, 0.0f, 1.0f);
}

