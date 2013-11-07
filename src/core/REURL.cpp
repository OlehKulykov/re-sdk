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
 

#include "../../include/recore/REURL.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REWideString.h"
#include "../../include/recore/RERange.h"


// http://en.wikipedia.org/wiki/URI_scheme

class REURLInternalString : public REString
{	
public:
	void fixSeparators()
	{
		if (this->isNotEmpty()) 
		{
			char * d = (char *)this->stringBuffer();
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
};

class REURLInternal 
{
private:
	void parseUserNameAndPassword(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseQuery(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseFragment(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseFileNameAndExtension(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseUserInfo(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseHostName(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parsePort(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseAuthority(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parsePath(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseHierarchicalPart(const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	void parseSchemeName(const char * stringValue, const REUInt32 location, const REUInt32 lenght);	
public:
	REBOOL parse();
	REURLInternalString string;
	
	REUInt32 queryLocation;
	REUInt32 queryLength;
	
	REUInt16 schemeNameLocation;
	REUInt16 schemeNameLength;
	
	REUInt16 userInfoLocation;
	REUInt16 userInfoLength;
	
	REUInt16 hostNameLocation;
	REUInt16 hostNameLength;
	
	REUInt16 portLocation;
	REUInt16 portLength;
	
	REUInt16 fragmentLocation;
	REUInt16 fragmentLength;
	
	REUInt16 authorityLocation;
	REUInt16 authorityLength;
	
	REUInt16 pathLocation;
	REUInt16 pathLength;
	
	REUInt16 hierarchicalPartLocation;
	REUInt16 hierarchicalPartLength;
	
	REUInt16 fileNameLocation;
	REUInt16 fileNameLength;
	
	REUInt16 extensionLocation;
	REUInt16 extensionLength;
	
	REUInt16 userNameLocation;
	REUInt16 userNameLength;
	
	REUInt16 passwordLocation;
	REUInt16 passwordLength;
	
	REURLInternal() :
		queryLocation(0),
		queryLength(0),
		schemeNameLocation(0),
		schemeNameLength(0),
		userInfoLocation(0),
		userInfoLength(0),
		hostNameLocation(0),
		hostNameLength(0),
		portLocation(0),
		portLength(0),
		fragmentLocation(0),
		fragmentLength(0),
		authorityLocation(0),
		authorityLength(0),
		pathLocation(0),
		pathLength(0),
		hierarchicalPartLocation(0),
		hierarchicalPartLength(0),
		fileNameLocation(0),
		fileNameLength(0),
		extensionLocation(0),
		extensionLength(0),
		userNameLocation(0),
		userNameLength(0),
		passwordLocation(0),
		passwordLength(0)
	{
		
	}
};

void REURLInternal::parseUserNameAndPassword(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
			userNameLocation = range.location;
			userNameLength = range.length;
			range.set(RENotFound, 0);
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
			passwordLocation = range.location;
			passwordLength = range.length;
		}
		else
		{
			userNameLocation = range.location;
			userNameLength = range.length;
		}
	}
}

void REURLInternal::parseQuery(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
					queryLocation = range.location;
					queryLength = range.length;
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

void REURLInternal::parseFragment(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
					fragmentLocation = range.location;
					fragmentLength = range.length;
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

void REURLInternal::parseFileNameAndExtension(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
{
	if (lenght) 
	{ 
		const char * d = (stringValue + location);
		RERange range(RENotFound, 0);
		REBOOL isFileNameDone = false;
		for (REUInt32 i = 0; i < lenght; i++) 
		{
			if (range.location == RENotFound) { range.location = (location + i); }
			if (*d == '/') { range.set(RENotFound, 0); }
			else if (*d == '.')
			{
				fileNameLocation = range.location;
				fileNameLength = range.length;
				range.set(RENotFound, 0);
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
				extensionLocation = range.location;
				extensionLength = range.length;
			}
			else
			{
				fileNameLocation = range.location;
				fileNameLength = range.length;
			}
		}
	}
}

void REURLInternal::parseUserInfo(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
		userInfoLocation = lastAtRange.location;
		userInfoLength = lastAtRange.length;
	}
}

void REURLInternal::parseHostName(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
		hostNameLocation = range.location;
		hostNameLength = range.length;
	}
}

void REURLInternal::parsePort(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
				portLocation = needLoc;
				portLength = len;
				return;
			}
			d--;
			dLoc--;
			leftLen--;
		}
	}
}

void REURLInternal::parseAuthority(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
		authorityLocation = location;
		authorityLength = len;
	}
}

void REURLInternal::parsePath(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
				pathLocation = dLoc;
				pathLength = len;
			}
			return;
		}
		d++;
		dLoc++;
		leftLen--;
	}
}

void REURLInternal::parseHierarchicalPart(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
					hierarchicalPartLocation = range.location;
					hierarchicalPartLength = range.length;
				}
				return;
				break;
			default: range.length++; break;
		}
		d++;
	}
	
	if (range.length && (range.location != RENotFound))
	{
		hierarchicalPartLocation = range.location;
		hierarchicalPartLength = range.length;
	}
}

void REURLInternal::parseSchemeName(const char * stringValue, const REUInt32 location, const REUInt32 lenght)
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
					schemeNameLocation = range.location;
					schemeNameLength = range.length;
				}
				return;
				break;
			default: range.length++; break;
		}
		d++;
	}
}

REBOOL REURLInternal::parse()
{
	if (string.isNotEmpty()) 
	{
		const char * stringValue = string.UTF8String();
		const REUInt32 lenght = string.length();
		
		this->parseSchemeName(stringValue, 0, lenght);
		this->parseHierarchicalPart(stringValue, 
									 schemeNameLocation + schemeNameLength, 
									 lenght - (schemeNameLocation + schemeNameLength));
		if (hierarchicalPartLength) 
		{
			this->parseAuthority(stringValue, hierarchicalPartLocation, hierarchicalPartLength);
			if (authorityLength) 
			{
				this->parseUserInfo(stringValue, authorityLocation, authorityLength);
				if (userInfoLength) 
				{
					this->parseUserNameAndPassword(stringValue, userInfoLocation, userInfoLength);
				}
				RERange hostRange(authorityLocation, authorityLength);
				if (userNameLength)
				{
					hostRange.location += userNameLength + 1; // skip separator
					hostRange.length -= userNameLength + 1; // skip separator
				}
				if (passwordLength)
				{
					hostRange.location += passwordLength + 1; // skip separator
					hostRange.length -= passwordLength + 1; // skip separator
				}
				this->parseHostName(stringValue, hostRange.location, hostRange.length);
				this->parsePort(stringValue, authorityLocation, authorityLength);
			}
			this->parsePath(stringValue, hierarchicalPartLocation, hierarchicalPartLength);
			if (pathLength) 
			{
				this->parseFileNameAndExtension(stringValue, pathLocation, pathLength);
			}
		}
		
		if (lenght > (hierarchicalPartLocation + hierarchicalPartLength + 1))
		{
			// can parse fragment and query
			this->parseFragment(stringValue, 0, lenght);
			this->parseQuery(stringValue, 0, lenght);
		}
		
		return true;
	}
	return false;
}

void REURL::parse()
{
	if (_i.isNotEmpty()) 
	{
		if (!_i->parse())
		{
			_i.release();
		}
	}
}

const char * REURL::UTF8String() const
{
	return _i.isNotEmpty() ? _i->string.UTF8String() : NULL;
}

const REUInt32 REURL::length() const
{
	return _i.isNotEmpty() ? _i->string.length() : 0;
}

void REURL::setWithLen(const char * stringValue, const REUInt32 lenght)
{
	_i.release();
	if (stringValue && lenght) 
	{
		REPtr<REURLInternal> newI(new REURLInternal());
		if (newI.isNotEmpty())
		{
			newI->string.setFromUTF8String(stringValue, lenght);
			_i = newI;
		}
	}
}

void REURL::setURLString(const char * stringValue, const REUInt32 length)
{
	_i.release();
	
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
					this->setWithLen(s, len);
					if (_i.isNotEmpty())
					{
						_i->string.fixSeparators();
					}
				}
			}
		}
	}
	
	this->parse();
}

REString REURL::schemeName() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->schemeNameLength) 
		{
			return REString(this->UTF8String(), RERange(_i->schemeNameLocation, _i->schemeNameLength));
		}
	}
	return REString();
}

REString REURL::userInfo() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->userInfoLength) 
		{
			return REString(this->UTF8String(), RERange(_i->userInfoLocation, _i->userInfoLength));
		}
	}
	return REString();
}

REString REURL::hostName() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->hostNameLength) 
		{
			return REString(this->UTF8String(), RERange(_i->hostNameLocation, _i->hostNameLength));
		}
	}
	return REString();
}

RENumber REURL::port() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->portLength) 
		{
			REString s(this->UTF8String(), RERange(_i->portLocation, _i->portLength));
			if (!s.isEmpty()) 
			{
				return RENumber::fromString(s);
			}
		}
	}
	return RENumber();
}

REString REURL::query() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->queryLength) 
		{
			return REString(this->UTF8String(), RERange(_i->queryLocation, _i->queryLength));
		}
	}
	return REString();
}

REString REURL::fragment() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->fragmentLength) 
		{
			return REString(this->UTF8String(), RERange(_i->fragmentLocation, _i->fragmentLength));
		}
	}
	return REString();
}

REString REURL::authority() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->authorityLength) 
		{
			return REString(this->UTF8String(), RERange(_i->authorityLocation, _i->authorityLength));
		}
	}
	return REString();
}

REString REURL::path() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->pathLength) 
		{
			return REString(this->UTF8String(), RERange(_i->pathLocation, _i->pathLength));
		}
	}
	return REString();
}

REString REURL::hierarchicalPart() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->hierarchicalPartLength) 
		{
			return REString(this->UTF8String(), RERange(_i->hierarchicalPartLocation, _i->hierarchicalPartLength));
		}
	}
	return REString();
}

REString REURL::fileName() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->fileNameLength) 
		{
			return REString(this->UTF8String(), RERange(_i->fileNameLocation, _i->fileNameLength));
		}
	}
	return REString();
}

REString REURL::extension() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->extensionLength) 
		{
			return REString(this->UTF8String(), RERange(_i->extensionLocation, _i->extensionLength));
		}
	}
	return REString();
}

REString REURL::fileNameWithExtension() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->fileNameLength) 
		{
			if (_i->extensionLocation && _i->extensionLength) 
			{
				REUInt32 len = _i->fileNameLength + _i->extensionLength;
				len += _i->extensionLocation - (_i->fileNameLocation + _i->fileNameLength);
				return REString(this->UTF8String(), RERange(_i->fileNameLocation, len));
			}
			else
			{
				return REString(this->UTF8String(), RERange(_i->fileNameLocation, _i->fileNameLength));
			}
		}
	}
	return REString();
}

REString REURL::userName() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->userNameLength) 
		{
			return REString(this->UTF8String(), RERange(_i->userNameLocation, _i->userNameLength));
		}
	}
	return REString();
}

REString REURL::password() const
{
	if (_i.isNotEmpty()) 
	{
		if (_i->passwordLength) 
		{
			return REString(this->UTF8String(), RERange(_i->passwordLocation, _i->passwordLength));
		}
	}
	return REString();
}

REBOOL REURL::isFileURL() const
{
	if (this->length() > 7) 
	{
		return (strncmp(this->UTF8String(), "file://", 7) == 0);
	}
	return false;
}

REURL & REURL::operator=(const REURL & urlValue)
{
	_i = urlValue._i;
	return (*this);
}

REURL & REURL::operator=(const REString & stringValue)
{
	this->setURLString(stringValue.UTF8String(), stringValue.length());
	return (*this);
}

REURL & REURL::operator=(const wchar_t * stringValue)
{
	REString s(REWideString(stringValue).string());
	this->setURLString(s.UTF8String(), s.length());
	return (*this);
}

REURL & REURL::operator=(const char * stringValue)
{
	this->setURLString(stringValue);
	return (*this);
}

REURL::REURL(const REURL & urlValue)
{
	_i = urlValue._i;
}

REURL::REURL(const REString & stringValue)
{
	this->setURLString(stringValue.UTF8String(), stringValue.length());
}

REURL::REURL(const wchar_t * wideString, const REUInt32 wideStringLength)
{
	REString s(REWideString(wideString, wideStringLength).string());
	this->setURLString(s.UTF8String(), s.length());
}

REURL::REURL(const char * utf8String, const REUInt32 utf8StringLength)
{
	this->setURLString(utf8String, utf8StringLength);
}

REURL::REURL()
{
	
}

REURL::~REURL()
{
	
}


