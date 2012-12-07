/*
 *   Copyright 2012 Kulikov Oleg
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


#include "../../include/renetwork/REURL.h"
#include "../../include/recore/REMem.h"

// http://en.wikipedia.org/wiki/URI_scheme

#ifdef __RE_USING_UINT16_FOR_URL_MAX_LENGTH__

#define REURL_SET_COMPS_VAL(v) (REUInt16)(v)
#define REURL_COMPS_VAL_TYPE REUInt16

#else

#define REURL_SET_COMPS_VAL(v) (REUInt32)(v)
#define REURL_COMPS_VAL_TYPE REUInt32

#endif


typedef struct _reURLCompsRngsStrtPrivate
{
	REURL_COMPS_VAL_TYPE schemeNameLocation;
	REURL_COMPS_VAL_TYPE schemeNameLength;
	
	REURL_COMPS_VAL_TYPE userInfoLocation;
	REURL_COMPS_VAL_TYPE userInfoLength;
	
	REURL_COMPS_VAL_TYPE hostNameLocation;
	REURL_COMPS_VAL_TYPE hostNameLength;
	
	REURL_COMPS_VAL_TYPE portLocation;
	REURL_COMPS_VAL_TYPE portLength;
	
	REURL_COMPS_VAL_TYPE queryLocation;
	REURL_COMPS_VAL_TYPE queryLength;
	
	REURL_COMPS_VAL_TYPE fragmentLocation;
	REURL_COMPS_VAL_TYPE fragmentLength;
	
	REURL_COMPS_VAL_TYPE authorityLocation;
	REURL_COMPS_VAL_TYPE authorityLength;
	
	REURL_COMPS_VAL_TYPE pathLocation;
	REURL_COMPS_VAL_TYPE pathLength;
	
	REURL_COMPS_VAL_TYPE hierarchicalPartLocation;
	REURL_COMPS_VAL_TYPE hierarchicalPartLength;
	
	REURL_COMPS_VAL_TYPE fileNameLocation;
	REURL_COMPS_VAL_TYPE fileNameLength;
	
	REURL_COMPS_VAL_TYPE extensionLocation;
	REURL_COMPS_VAL_TYPE extensionLength;
	
	REURL_COMPS_VAL_TYPE userNameLocation;
	REURL_COMPS_VAL_TYPE userNameLength;
	
	REURL_COMPS_VAL_TYPE passwordLocation;
	REURL_COMPS_VAL_TYPE passwordLength;
	
} REURLCompsRngsStrtPrivate;

void REURL::ParseUserNameAndPassword(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	const char * d = stringValue;
	d += location;
	REUInt32 leftLen = lenght;
	REUInt32 dPos = location;
	RERange range(RENotFound, 0);
	REBOOL isUser = false;
	while (*d && leftLen) 
	{
		if (range.location == RENotFound) 
		{
			range.location = dPos;
		}
		if (*d == ':') 
		{
			((REURLCompsRngsStrtPrivate *)info)->userNameLocation = REURL_SET_COMPS_VAL(range.location);
			((REURLCompsRngsStrtPrivate *)info)->userNameLength = REURL_SET_COMPS_VAL(range.length);
			range.Set(RENotFound, 0);
			isUser = true;
		}
		else
		{
			range.length++;
		}
		
		leftLen--;
		dPos++;
		d++;
	}
	
	if (range.length) 
	{
		if (isUser) 
		{
			((REURLCompsRngsStrtPrivate *)info)->passwordLocation = REURL_SET_COMPS_VAL(range.location);
			((REURLCompsRngsStrtPrivate *)info)->passwordLength = REURL_SET_COMPS_VAL(range.length);
		}
		else
		{
			((REURLCompsRngsStrtPrivate *)info)->userNameLocation = REURL_SET_COMPS_VAL(range.location);
			((REURLCompsRngsStrtPrivate *)info)->userNameLength = REURL_SET_COMPS_VAL(range.length);
		}
	}
}

void REURL::ParseQuery(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	if (lenght) 
	{
		REUInt32 leftLen = lenght - 1;
		const char * d = stringValue;
		REUInt32 dLoc = location + lenght - 1;
		d += dLoc;
		RERange range(0, 0);
		while (*d && leftLen) 
		{
			if (*d == '#') 
			{
				range.length = 0;
			}
			else if (*d == '?')
			{
				if (range.length) 
				{
					((REURLCompsRngsStrtPrivate *)info)->queryLocation = REURL_SET_COMPS_VAL(range.location);
					((REURLCompsRngsStrtPrivate *)info)->queryLength = REURL_SET_COMPS_VAL(range.length);
				}
				return;
			}
			else
			{
				range.location = dLoc;
				range.length++;
			}
			d--;
			dLoc--;
			leftLen--;
		}
	}
}

void REURL::ParseFragment(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	if (lenght) 
	{
		REUInt32 leftLen = lenght - 1;
		const char * d = stringValue;
		REUInt32 dLoc = location + lenght - 1;
		d += dLoc;
		RERange range(0, 0);
		while (*d && leftLen) 
		{
			if (*d == '#') 
			{
				if (range.length) 
				{
					((REURLCompsRngsStrtPrivate *)info)->fragmentLocation = REURL_SET_COMPS_VAL(range.location);
					((REURLCompsRngsStrtPrivate *)info)->fragmentLength = REURL_SET_COMPS_VAL(range.length);
				}
				return;
			}
			else
			{
				range.location = dLoc;
				range.length++;
			}
			d--;
			dLoc--;
			leftLen--;
		}
	}
}

void REURL::ParseFileNameAndExtension(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	if (lenght) 
	{ 
		const char * d = (stringValue + location);
		RERange range(RENotFound, 0);
		REBOOL isFileNameDone = false;
		for (REUInt32 i = 0; i < lenght; i++) 
		{
			if (range.location == RENotFound) { range.location = (location + i); }
			if (*d == '/') { range.Set(RENotFound, 0); }
			else if (*d == '.')
			{
				((REURLCompsRngsStrtPrivate *)info)->fileNameLocation = REURL_SET_COMPS_VAL(range.location);
				((REURLCompsRngsStrtPrivate *)info)->fileNameLength = REURL_SET_COMPS_VAL(range.length);
				range.Set(RENotFound, 0);
				isFileNameDone = true;
			}
			else if (*d == 0) { break; }
			else { range.length++; }
			d++;
		}
		
		if (range.length && (range.location != RENotFound)) 
		{
			if (isFileNameDone) 
			{
				((REURLCompsRngsStrtPrivate *)info)->extensionLocation = REURL_SET_COMPS_VAL(range.location);
				((REURLCompsRngsStrtPrivate *)info)->extensionLength = REURL_SET_COMPS_VAL(range.length);
			}
			else
			{
				((REURLCompsRngsStrtPrivate *)info)->fileNameLocation = REURL_SET_COMPS_VAL(range.location);
				((REURLCompsRngsStrtPrivate *)info)->fileNameLength = REURL_SET_COMPS_VAL(range.length);
			}
		}
	}
}

void REURL::ParseUserInfo(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	REUInt32 leftLen = lenght;
	REUInt32 len = 0;
	RERange lastAtRange(0, 0);
	const char * d = stringValue;
	d += location;
	while (*d && leftLen) 
	{
		if (*d == '@') 
		{
			lastAtRange.location = location;
			lastAtRange.length = len;
		}
		d++;
		leftLen--;
		len++;
	}
	if (lastAtRange.length) 
	{
		((REURLCompsRngsStrtPrivate *)info)->userInfoLocation = REURL_SET_COMPS_VAL(lastAtRange.location);
		((REURLCompsRngsStrtPrivate *)info)->userInfoLength = REURL_SET_COMPS_VAL(lastAtRange.length);
	}
}

void REURL::ParseHostName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	const char * d = (stringValue + location);
	RERange range(RENotFound, 0);
	for (REUInt32 i = 0; i < lenght; i++) 
	{
		if (range.location == RENotFound) { range.location = (location + i); }
		if ((*d == ':') || (*d == '/') || (*d == 0)) { break; }
		else { range.length++; }
		d++;
	}
	if (range.length && (range.location != RENotFound))
	{
		((REURLCompsRngsStrtPrivate *)info)->hostNameLocation = REURL_SET_COMPS_VAL(range.location);
		((REURLCompsRngsStrtPrivate *)info)->hostNameLength = REURL_SET_COMPS_VAL(range.length);
	}
}

void REURL::ParsePort(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	if (lenght) 
	{
		REUInt32 leftLen = lenght - 1;
		const char * d = stringValue;
		REUInt32 dLoc = location + lenght - 1;
		d += dLoc;
		REUInt32 len = 0;
		REUInt32 needLoc = 0;
		while (*d && leftLen) 
		{
			if (isdigit((int)*d)) 
			{
				needLoc = dLoc;
				len++;
			}
			else if (len && (*d == ':'))
			{
				((REURLCompsRngsStrtPrivate *)info)->portLocation = REURL_SET_COMPS_VAL(needLoc);
				((REURLCompsRngsStrtPrivate *)info)->portLength = REURL_SET_COMPS_VAL(len);
				return;
			}
			d--;
			dLoc--;
			leftLen--;
		}
	}
}

void REURL::ParseAuthority(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	REUInt32 leftLen = lenght;
	REUInt32 len = 0;
	const char * d = stringValue;
	d += location;
	while (*d && leftLen) 
	{
		if ((*d == '/') || (*d == '?')) 
		{
			break;
		}
		d++;
		leftLen--;
		len++;
	}
	if (len) 
	{
		((REURLCompsRngsStrtPrivate *)info)->authorityLocation = REURL_SET_COMPS_VAL(location);
		((REURLCompsRngsStrtPrivate *)info)->authorityLength = REURL_SET_COMPS_VAL(len);
	}
}

void REURL::ParsePath(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	REUInt32 leftLen = lenght;
	REUInt32 dLoc = location;
	const char * d = stringValue;
	d += location;
	while (*d && leftLen) 
	{
		if (*d == '/') 
		{
			d++;
			dLoc++;
			leftLen--;
			REUInt32 len = 0;
			while (*d && leftLen) 
			{
				d++;
				len++;
				leftLen--;
			}
			if (len) 
			{
				((REURLCompsRngsStrtPrivate *)info)->pathLocation = REURL_SET_COMPS_VAL(dLoc);
				((REURLCompsRngsStrtPrivate *)info)->pathLength = REURL_SET_COMPS_VAL(len);
			}
			return;
		}
		d++;
		dLoc++;
		leftLen--;
	}
}

void REURL::ParseHierarchicalPart(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	RERange startRange(location, lenght);
	const char * d = stringValue + location;
	REUInt16 slashCount = 0;
	while (*d && startRange.length && ((*d == '/') || (*d == ':'))) 
	{
		if (*d == '/') { slashCount++; }
		else { slashCount = 0; }
		startRange.location++; 
		startRange.length--;
		d++;
		if (slashCount == 2) { break; }
	}
	
	RERange range(RENotFound, 0);
	for (REUInt32 i = 0; i < startRange.length; i++) 
	{
		if (range.location == RENotFound) { range.location = (startRange.location + i); }
		switch (*d) 
		{
			case 0:
			case '?':
				if (range.length && (range.location != RENotFound))
				{
					((REURLCompsRngsStrtPrivate *)info)->hierarchicalPartLocation = REURL_SET_COMPS_VAL(range.location);
					((REURLCompsRngsStrtPrivate *)info)->hierarchicalPartLength = REURL_SET_COMPS_VAL(range.length);
				}
				return;
				break;
			default: range.length++; break;
		}
		d++;
	}
	
	if (range.length && (range.location != RENotFound))
	{
		((REURLCompsRngsStrtPrivate *)info)->hierarchicalPartLocation = REURL_SET_COMPS_VAL(range.location);
		((REURLCompsRngsStrtPrivate *)info)->hierarchicalPartLength = REURL_SET_COMPS_VAL(range.length);
	}
}

void REURL::ParseSchemeName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	const char * d = stringValue + location;
	RERange range(RENotFound, 0);
	for (REUInt32 i = 0; i < lenght; i++) 
	{
		if (range.location == RENotFound) { range.location = (location + i); }
		switch (*d) 
		{
			case 0: 
			case '@':	
			case '.':
			case '/':
			case '?':
			case '#':
				return; 
				break;
			case ':':
				if (range.length && (range.location != RENotFound))
				{
					((REURLCompsRngsStrtPrivate *)info)->schemeNameLocation = REURL_SET_COMPS_VAL(range.location);
					((REURLCompsRngsStrtPrivate *)info)->schemeNameLength = REURL_SET_COMPS_VAL(range.length);
				}
				return;
				break;
			default: range.length++; break;
		}
		d++;
	}
}

REBOOL REURL::Parse(void * info, const char * stringValue, const REUInt32 lenght)
{
	if (info && stringValue && lenght) 
	{
		REURL::ParseSchemeName(info, stringValue, 0, lenght);
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate *)info;
		REURL::ParseHierarchicalPart(info, 
									 stringValue, 
									 urlStruct->schemeNameLocation + urlStruct->schemeNameLength, 
									 lenght - (urlStruct->schemeNameLocation + urlStruct->schemeNameLength));
		if (urlStruct->hierarchicalPartLength) 
		{
			REURL::ParseAuthority(info, stringValue, urlStruct->hierarchicalPartLocation, urlStruct->hierarchicalPartLength);
			if (urlStruct->authorityLength) 
			{
				REURL::ParseUserInfo(info, stringValue, urlStruct->authorityLocation, urlStruct->authorityLength);
				if (urlStruct->userInfoLength) 
				{
					REURL::ParseUserNameAndPassword(info, stringValue, urlStruct->userInfoLocation, urlStruct->userInfoLength);
				}
				RERange hostRange(urlStruct->authorityLocation, urlStruct->authorityLength);
				if (urlStruct->userNameLength)
				{
					hostRange.location += urlStruct->userNameLength + 1; // skip separator
					hostRange.length -= urlStruct->userNameLength + 1; // skip separator
				}
				if (urlStruct->passwordLength)
				{
					hostRange.location += urlStruct->passwordLength + 1; // skip separator
					hostRange.length -= urlStruct->passwordLength + 1; // skip separator
				}
				REURL::ParseHostName(info, stringValue, hostRange.location, hostRange.length);
				REURL::ParsePort(info, stringValue, urlStruct->authorityLocation, urlStruct->authorityLength);
			}
			REURL::ParsePath(info, stringValue, urlStruct->hierarchicalPartLocation, urlStruct->hierarchicalPartLength);
			if (urlStruct->pathLength) 
			{
				REURL::ParseFileNameAndExtension(info, stringValue, urlStruct->pathLocation, urlStruct->pathLength);
			}
		}
		
		if (lenght > (urlStruct->hierarchicalPartLocation + urlStruct->hierarchicalPartLength + 1))
		{
			// can parse fragment and query
			REURL::ParseFragment(info, stringValue, 0, lenght);
			REURL::ParseQuery(info, stringValue, 0, lenght);
		}
		
		return true;
	}
	return false;
}

void REURL::Parse()
{
	if (_ranges) 
	{
		memset(_ranges, 0, sizeof(REURLCompsRngsStrtPrivate));
	}
	else
	{
		void * newRanges = malloc(sizeof(REURLCompsRngsStrtPrivate));
		if (newRanges) 
		{
			memset(newRanges, 0, sizeof(REURLCompsRngsStrtPrivate));
			_ranges = newRanges;
		}
	}
	
	if (_ranges) 
	{
		if (!REURL::Parse(_ranges, this->UTF8String(), this->Length())) 
		{
			memset(_ranges, 0, sizeof(REURLCompsRngsStrtPrivate));
		}
	}
}

const char * REURL::UTF8String() const
{
	return (const char *)_buffer.GetBuffer();
}

const REUInt32 REURL::Length() const
{
	const REUInt32 len = _buffer.GetSize();
	return ((len > 0) ? (len - 1) : 0);
}

void REURL::SetWithLen(const char * stringValue, const REUInt32 lenght)
{
	_buffer.Clear();
	if (stringValue && lenght) 
	{
		if (_buffer.Resize(lenght + 1, false))
		{
			char * b = (char *)_buffer.GetBuffer();
			memcpy(b, stringValue, lenght);
			b[lenght] = 0;
		}
	}
}

void REURL::SetURLString(const char * stringValue, const REUInt32 length)
{
	_buffer.Clear();
	if (_ranges) 
	{
		free(_ranges);
		_ranges = NULL;
	}
	
	if (stringValue) 
	{
		REUInt32 len = (length != RENotFound) ? length : (REUInt32)strlen(stringValue);
		if (len) 
		{
			const char * s = stringValue;
			const char * e = &stringValue[len - 1];
			while (*s && len) 
			{
				if ((*s == ' ') || (*s == '\t')) 
				{
					len--;
					s++;
				}
				else { break; }
			}
			
			if (len) 
			{
				while (len) 
				{
					if ((*e == ' ') || (*e == '\t') || (*e == '\n') || (*e == '\r') || (*e == '/')) 
					{
						len--;
						e--;
					}
					else { break; }
				}
				
				if (len) 
				{
					this->SetWithLen(s, len);
					if (_buffer.GetBuffer()) 
					{
						char * d = (char *)_buffer.GetBuffer();
						while (*d) 
						{
							if (*d == '\\') { *d++ = '/'; }
							else 
							{
								//*d++ = (char)tolower((int)*d); 
								d++;
							}
						}
					}
				}
			}
		}
	}
	
	this->Parse();
}

REString REURL::GetSchemeName() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->schemeNameLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->schemeNameLocation, urlStruct->schemeNameLength));
		}
	}
	return REString("");
}

REString REURL::GetUserInfo() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->userInfoLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->userInfoLocation, urlStruct->userInfoLength));
		}
	}
	return REString("");
}

REString REURL::GetHostName() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->hostNameLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->hostNameLocation, urlStruct->hostNameLength));
		}
	}
	return REString("");
}

RENumber REURL::GetPort() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->portLength) 
		{
			REString s(this->UTF8String(), RERange(urlStruct->portLocation, urlStruct->portLength));
			if (!s.IsEmpty()) 
			{
				RENumber n;
				n.SetValueFromString(s);
				return n;
			}
		}
	}
	return RENumber();
}

REString REURL::GetQuery() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->queryLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->queryLocation, urlStruct->queryLength));
		}
	}
	return REString("");
}

REString REURL::GetFragment() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->fragmentLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->fragmentLocation, urlStruct->fragmentLength));
		}
	}
	return REString("");
}

REString REURL::GetAuthority() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->authorityLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->authorityLocation, urlStruct->authorityLength));
		}
	}
	return REString("");
}

REString REURL::GetPath() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->pathLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->pathLocation, urlStruct->pathLength));
		}
	}
	return REString("");
}

REString REURL::GetHierarchicalPart() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->hierarchicalPartLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->hierarchicalPartLocation, urlStruct->hierarchicalPartLength));
		}
	}
	return REString("");
}

REString REURL::GetFileName() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->fileNameLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->fileNameLocation, urlStruct->fileNameLength));
		}
	}
	return REString("");
}

REString REURL::GetExtension() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->extensionLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->extensionLocation, urlStruct->extensionLength));
		}
	}
	return REString("");
}

REString REURL::GetFileNameWithExtension() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->fileNameLength) 
		{
			if (urlStruct->extensionLocation && urlStruct->extensionLength) 
			{
				REUInt32 len = urlStruct->fileNameLength + urlStruct->extensionLength;
				len += urlStruct->extensionLocation - (urlStruct->fileNameLocation + urlStruct->fileNameLength);
				return REString(this->UTF8String(), RERange(urlStruct->fileNameLocation, len));
			}
			else
			{
				return REString(this->UTF8String(), RERange(urlStruct->fileNameLocation, urlStruct->fileNameLength));
			}
		}
	}
	return REString("");
}

REString REURL::GetUserName() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->userNameLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->userNameLocation, urlStruct->userNameLength));
		}
	}
	return REString("");
}

REString REURL::GetPassword() const
{
	if (_ranges) 
	{
		REURLCompsRngsStrtPrivate * urlStruct = (REURLCompsRngsStrtPrivate*)_ranges;
		if (urlStruct->passwordLength) 
		{
			return REString(this->UTF8String(), RERange(urlStruct->passwordLocation, urlStruct->passwordLength));
		}
	}
	return REString("");
}

REBOOL REURL::IsFileURL() const
{
	if (this->Length() > 7) 
	{
		return (strncmp(this->UTF8String(), "file://", 7) == 0);
	}
	return false;
}

void REURL::SetURL(const REURL & url)
{
	_buffer.Clear();
	if (url.Length() && url._ranges) 
	{
		if (_ranges) 
		{
			memset(_ranges, 0, sizeof(REURLCompsRngsStrtPrivate));
		}
		else
		{
			void * newRanges = malloc(sizeof(REURLCompsRngsStrtPrivate));
			if (newRanges) 
			{
				memset(newRanges, 0, sizeof(REURLCompsRngsStrtPrivate));
				_ranges = newRanges;
			}
		}
		
		if (_ranges) 
		{
			_buffer.Set(url._buffer.GetBuffer(), url._buffer.GetSize());
			memcpy(_ranges, url._ranges, sizeof(REURLCompsRngsStrtPrivate));
		}
	}
}

REURL & REURL::operator=(const REURL & urlValue)
{
	this->SetURL(urlValue);
	return (*this);
}

REURL & REURL::operator=(const REString & stringValue)
{
	this->SetURLString(stringValue.UTF8String(), stringValue.Length());
	return (*this);
}

REURL & REURL::operator=(const wchar_t * stringValue)
{
	REStringPresentation p(stringValue);
	this->SetURLString(p.UTF8String(), p.GetUTF8Length());
	return (*this);
}

REURL & REURL::operator=(const char * stringValue)
{
	this->SetURLString(stringValue);
	return (*this);
}

REURL::REURL(const REURL & urlValue) : _ranges(NULL)
{
	this->SetURL(urlValue);
}

REURL::REURL(const REString & stringValue) : _ranges(NULL)
{
	this->SetURLString(stringValue.UTF8String(), stringValue.Length());
}

REURL::REURL(const wchar_t * stringValue) : _ranges(NULL)
{
	REStringPresentation p(stringValue);
	this->SetURLString(p.UTF8String(), p.GetUTF8Length());
}

REURL::REURL(const char * stringValue) : _ranges(NULL)
{
	this->SetURLString(stringValue);
}

REURL::REURL() : _ranges(NULL)
{
	
}

REURL::~REURL()
{
	if (_ranges) 
	{
		free(_ranges);
	}
}


