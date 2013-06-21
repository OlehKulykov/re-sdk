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


#include "../../include/recore/private/REStringUtilsPrivate.h"

REPtr<REBuffer> REStringUtilsPrivate::newBuffForUTF8String(const char * utf8String, 
														   const REUInt32 utf8StringLength)
{
	const REUInt32 len = REStringUtilsPrivate::actualUTF8StringLength(utf8String, utf8StringLength);
	if (len)
	{
		REBuffer * newBuff = new REBuffer();
		if (newBuff)
		{
			if (newBuff->resize(len + 1, false))
			{
				char * buff = (char *)newBuff->getBuffer();
				memcpy(buff, utf8String, len);
				buff[len] = 0;
				return REPtr<REBuffer>(newBuff);
			}
			delete newBuff;
		}
	}
	return REPtr<REBuffer>();
}

REPtr<REBuffer> REStringUtilsPrivate::newBuffForWideString(const wchar_t * wideString, 
														   const REUInt32 wideStringLength)
{
	const REUInt32 len = REStringUtilsPrivate::actualWideStringLength(wideString, wideStringLength);
	if (len)
	{
		REBuffer * newBuff = new REBuffer();
		if (newBuff)
		{
			if (newBuff->resize((len + 1) * sizeof(wchar_t), false))
			{
				wchar_t * buff = (wchar_t *)newBuff->getBuffer();
				memcpy(buff, wideString, (len * sizeof(wchar_t)));
				buff[len] = 0;
				return REPtr<REBuffer>(newBuff);
			}
			delete newBuff;
		}
	}
	return REPtr<REBuffer>();
}


REPtr<REBuffer> REStringUtilsPrivate::getWideFromUTF8(const char * utf8String, 
													  const REUInt32 utf8StringLength)
{
	const REUInt32 len = REStringUtilsPrivate::actualUTF8StringLength(utf8String, utf8StringLength);
	if (len)
	{
		REBuffer * newBuff = new REBuffer();
		if (newBuff)
		{
			if (newBuff->resize((len + 1) * sizeof(wchar_t), false))
			{
				wchar_t * wideString = (wchar_t *)newBuff->getBuffer();
				const int resLen = REStringUtilsPrivate::charsToWide(utf8String, (int)len, wideString);
				if (newBuff->resize((resLen + 1) * sizeof(wchar_t), true))
				{
					wideString = (wchar_t *)newBuff->getBuffer();
					wideString[resLen] = 0;
					return REPtr<REBuffer>(newBuff);
				}
			}
			delete newBuff;
		}
	}
	return REPtr<REBuffer>();
}

REPtr<REBuffer> REStringUtilsPrivate::getUTF8FromWide(const wchar_t * wideString, 
													  const REUInt32 wideStringLength)
{
	const REUInt32 len = REStringUtilsPrivate::actualWideStringLength(wideString, wideStringLength);
	if (len)
	{
		REBuffer * newBuffer = new REBuffer();
		if (newBuffer)
		{
			if (newBuffer->resize((len + 1) * sizeof(wchar_t), false)) 
			{
				char * charString = (char *)newBuffer->getBuffer();
				const int resLen = REStringUtilsPrivate::wideToChars(wideString,
																	 (int)len,
																	 charString);
				if (newBuffer->resize(resLen + 1, true))
				{
					charString = (char *)newBuffer->getBuffer();
					charString[resLen] = 0;
					return REPtr<REBuffer>(newBuffer);
				}
			}
			delete newBuffer;
		}
	}
	return REPtr<REBuffer>();
}

REPtr<REBuffer> REStringUtilsPrivate::getWideFromUTF8(const REPtr<REBuffer> & utf8StringBuffer)
{
	const REUInt32 len = REStringUtilsPrivate::stringLengthFromUTF8Buffer(utf8StringBuffer);
	if (len > 0)
	{
		return REStringUtilsPrivate::getWideFromUTF8((const char *)utf8StringBuffer->getBuffer(), 
													 len);
	}
	return REPtr<REBuffer>();
}

REPtr<REBuffer> REStringUtilsPrivate::getUTF8FromWide(const REPtr<REBuffer> & wideStringBuffer)
{
	const REUInt32 len = REStringUtilsPrivate::stringLengthFromWideBuffer(wideStringBuffer);
	if (len > 0)
	{
		return REStringUtilsPrivate::getUTF8FromWide((const wchar_t *)wideStringBuffer->getBuffer(), 
													 len);
	}
	return REPtr<REBuffer>();
}

REPtr<REBuffer> REStringUtilsPrivate::makeCopy(const REPtr<REBuffer> & sourceBuffer)
{
	if (sourceBuffer.isNotEmpty())
	{
		const REUInt32 size = sourceBuffer->getSize();
		if (size > 0)
		{
			REBuffer * newBuff = new REBuffer();
			if (newBuff)
			{
				if (newBuff->resize(size, false))
				{
					memcpy(newBuff->getBuffer(), sourceBuffer->getBuffer(), size);
					return REPtr<REBuffer>(newBuff);
				}
				delete newBuff;
			}
		}
	}
	return REPtr<REBuffer>();
}

REUInt32 REStringUtilsPrivate::stringLengthFromUTF8Buffer(const REPtr<REBuffer> & utf8StringBuffer)
{
	if (utf8StringBuffer.isNotEmpty()) 
	{
		const REUInt32 size = utf8StringBuffer->getSize();
		return (size > 0) ? (size - 1) : 0;
	}
	return 0;
}

REUInt32 REStringUtilsPrivate::stringLengthFromWideBuffer(const REPtr<REBuffer> & wideStringBuffer)
{
	if (wideStringBuffer.isNotEmpty()) 
	{
		const REUInt32 size = wideStringBuffer->getSize();
		const REUInt32 charSize = (REUInt32)sizeof(wchar_t);
		return (size >= charSize) ? ((size / charSize) - 1) : 0;
	}
	return 0;
}


int REStringUtilsPrivate::wideToChars(const wchar_t * wideString, int wideStringLength, char * charsString)
{
	int stringLength = 0;
	if (wideStringLength > 0)
	{
		int l = wideStringLength;
		unsigned char * cursor = (unsigned char *)charsString;
		const wchar_t * ch = wideString;
		for (int i = 0; i < l; i++) 
		{
			wchar_t u = *ch;
			if (u < (unsigned char)0x80) 
			{
				*cursor++ = (unsigned char)u;
			} 
			else 
			{
				if (u < 0x0800) 
				{
					*cursor++ = (unsigned char)0xc0 | ((unsigned char) (u >> 6));
				} 
				else 
				{
					if ( (u >= 0xd800) && (u < 0xdc00) && (i < l-1)) 
					{
						unsigned short low = ch[1];
						if ((low >= 0xdc00) && (low < 0xe000)) 
						{
							++ch;
							++i;
							u = ((u - 0xd800)*0x400) + (low - 0xdc00) + 0x10000;
						}
					}
					if (u > 0xffff) 
					{
						// if people are working in utf8, but strings are encoded in eg. latin1, the resulting
						// name might be invalid utf8. This and the corresponding code in fromUtf8 takes care
						// we can handle this without loosing information. This can happen with latin filenames
						// and a utf8 locale under Unix.
						if ( (u > 0x10fe00) && (u < 0x10ff00) )
						{
							*cursor++ = (u - 0x10fe00);
							++ch;
							continue;
						} 
						else 
						{
							*cursor++ = (unsigned char)0xf0 | ((unsigned char) (u >> 18));
							*cursor++ = (unsigned char)0x80 | (((unsigned char) (u >> 12)) & (unsigned char)0x3f);
						}
					} 
					else 
					{
						*cursor++ = (unsigned char)0xe0 | ((unsigned char) (u >> 12));
					}
					*cursor++ = (unsigned char)0x80 | (((unsigned char) (u >> 6)) & (unsigned char)0x3f);
				}
				*cursor++ = (unsigned char)0x80 | ((unsigned char) (u & (unsigned char)0x3f));
			}
			++ch;
		}
		stringLength = (int)(cursor - (unsigned char*)charsString);
	}
	return stringLength; 
}

int REStringUtilsPrivate::charsToWide(const char * charsString, int charsStringLengthOrNegative, wchar_t * wideString)
{
	if ( charsStringLengthOrNegative < 0)
	{
		charsStringLengthOrNegative = (int)strlen(charsString);
	}
	
	wchar_t uc = 0;
	int need = 0;
	int error = -1;
	unsigned char ch;
	int stringLength = 0;
	for (int i = 0; i < charsStringLengthOrNegative; ++i) 
	{
		ch = charsString[i];
		if (need) 
		{
			if ((ch&(unsigned char)0xc0) == (unsigned char)0x80) 
			{
				uc = (uc << 6) | (ch & (unsigned char)0x3f);
				need--;
				if (!need) 
				{
					if (uc > 0xffff) 
					{
						uc -= 0x10000;
						wchar_t high = (uc/0x400) + 0xd800;
						wchar_t low = (uc%0x400) + 0xdc00;
						*wideString++ = high;
						*wideString++ = low;
						stringLength++;
					} 
					else 
					{
						*wideString++ = uc;
						stringLength++;
					}
				}
			} 
			else 
			{
				i = error;
				*wideString++ = (wchar_t)0xdbff;
				*wideString++ = (wchar_t)0xde00 + ((unsigned char)charsString[i]);
				stringLength++;
				need = 0;
			}
		}
		else 
		{
			if (ch < 128) 
			{
				*wideString++ = ch;
				stringLength++;
			} 
			else if ((ch & 0xe0) == 0xc0) 
			{
				uc = ch & 0x1f;
				need = 1;
				error = i;
			} 
			else if ((ch & 0xf0) == 0xe0) 
			{
				uc = ch & 0x0f;
				need = 2;
				error = i;
			} 
			else if ((ch & 0xf8) == 0xf0) 
			{
				uc = ch & 0x07;
				need = 3;
				error = i;
			} 
			else 
			{
				*wideString++ = 0xdbff;
				*wideString++ = 0xde00 + ((unsigned char)charsString[i]);
				stringLength++;
			}
		}
	}
	if (need) 
	{
		for (int i = error; i < charsStringLengthOrNegative; ++i) 
		{
			*wideString++ = 0xdbff;
			*wideString++ = 0xde00 + (unsigned char)charsString[i];
			stringLength++;
		}
	}
	return stringLength;
}

REPtr<REBuffer> REStringUtilsPrivate::getAppendedWithPathComponent(const REPtr<REBuffer> & utf8Buffer, 
																   const char * comp, 
																   const REUInt32 compLen)
{
	const REUInt32 addLen = REStringUtilsPrivate::actualUTF8StringLength(comp, compLen);
	if (addLen == 0)
	{
		return utf8Buffer;
	}
	
	const char * addStr = addLen ? comp : NULL;
	
	const REUInt32 inLen = REStringUtilsPrivate::stringLengthFromUTF8Buffer(utf8Buffer);
	char * inStr = inLen ? (char *)utf8Buffer->getBuffer() : NULL;
	
	
	const char * startPtr = NULL;
	REUInt32 startLen = 0;
	bool isInLastSep = false;
	if (inLen)
	{
		startPtr = inStr;
		while (*inStr) 
		{
			switch (*inStr) 
			{
				case '/':
#ifdef __RE_OS_WINDOWS__					
					*inStr = '\\';
#endif					
					isInLastSep = true;
					break;
				case '\\':
#ifndef __RE_OS_WINDOWS__
					*inStr = '/';
#endif					
					isInLastSep = true;
					break;
				default:
					isInLastSep = false;
					break;
			}
			startLen++;
			inStr++;
		}
	}
	
	const char * addPtr = NULL;
	REUInt32 addCompLen = 0;
	if (addLen)
	{
		addPtr = addStr;
		
	}
	
	return REPtr<REBuffer>();
}
















