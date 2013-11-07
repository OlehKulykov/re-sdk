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


#include "../../include/recore/REString.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REBuffer.h"

#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#include "../../include/recore/REData.h"
#endif

//#ifdef __RE_OS_BADA__
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <wctype.h>
//#endif /* __RE_OS_BADA__ */

#ifndef __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__

#define RESTRINGHTMLSPECIALCHARACTERSPRIVATE_MAPSIZE 253

class REStringHTMLSpecialCharacterPrivate 
{	
public:
	typedef struct _mapStruct
	{
		const char * strValue;
		wchar_t ucharValue;
	} MapStruct;
	
	const REStringHTMLSpecialCharacterPrivate::MapStruct * GetForWChar(const wchar_t wch) const
	{
		const REStringHTMLSpecialCharacterPrivate::MapStruct * map = this->GetMap();
		REUInt32 left = 0;
		REUInt32 right = RESTRINGHTMLSPECIALCHARACTERSPRIVATE_MAPSIZE - 1;
		while (left <= right) 
		{
			REUInt32 middle = (left + right) / 2;
			const REStringHTMLSpecialCharacterPrivate::MapStruct * middleValue = &map[middle];
			if ( middleValue->ucharValue == wch )
			{
				return middleValue;
			}
			else if (middleValue->ucharValue > wch)
			{
				if (middle) 
				{
					right = middle - 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				left = middle + 1;
			}
		}
		return NULL;
	}
	
	const REStringHTMLSpecialCharacterPrivate::MapStruct * GetFromStr(const char * strV, const REUInt32 len) const
	{
		const REStringHTMLSpecialCharacterPrivate::MapStruct * map = this->GetMap();
		for (REUInt32 i = 0; i < RESTRINGHTMLSPECIALCHARACTERSPRIVATE_MAPSIZE; i++) 
		{
			const REStringHTMLSpecialCharacterPrivate::MapStruct * mapStruct = &map[i];
			const REUInt32 elLen = (REUInt32)strlen(mapStruct->strValue);
			if (elLen <= len) 
			{
				if (strncmp(strV, mapStruct->strValue, elLen) == 0) 
				{
					return mapStruct;
				}
			}
		}
		return NULL;
	}
	
	static const REStringHTMLSpecialCharacterPrivate::MapStruct * StaticMap();
	
	const REStringHTMLSpecialCharacterPrivate::MapStruct * GetMap() const 
	{
		return REStringHTMLSpecialCharacterPrivate::StaticMap(); 
	}
	
	const REUInt32 GetMapElementsCount() const 
	{
		return RESTRINGHTMLSPECIALCHARACTERSPRIVATE_MAPSIZE; 
	}
};

const REStringHTMLSpecialCharacterPrivate::MapStruct * REStringHTMLSpecialCharacterPrivate::StaticMap()
{
	static const REStringHTMLSpecialCharacterPrivate::MapStruct map[RESTRINGHTMLSPECIALCHARACTERSPRIVATE_MAPSIZE] = 
	{
		// A.2.2. Special characters
		{"&quot;", 34},	{"&amp;", 38}, {"&apos;", 39}, {"&lt;", 60}, {"&gt;", 62},
		// A.2.1. Latin-1 characters
		{"&nbsp;", 160}, {"&iexcl;", 161}, {"&cent;", 162},
		{"&pound;", 163}, {"&curren;", 164}, {"&yen;", 165}, {"&brvbar;", 166},
		{"&sect;", 167}, {"&uml;", 168}, {"&copy;", 169}, {"&ordf;", 170},
		{"&laquo;", 171}, {"&not;", 172}, {"&shy;", 173}, {"&reg;", 174},
		{"&macr;", 175}, {"&deg;", 176}, {"&plusmn;", 177}, {"&sup2;", 178},
		{"&sup3;", 179}, {"&acute;", 180}, {"&micro;", 181}, {"&para;", 182},
		{"&middot;", 183}, {"&cedil;", 184}, {"&sup1;", 185}, {"&ordm;", 186},
		{"&raquo;", 187}, {"&frac14;", 188}, {"&frac12;", 189}, {"&frac34;", 190},
		{"&iquest;", 191}, {"&Agrave;", 192}, {"&Aacute;", 193}, {"&Acirc;", 194},
		{"&Atilde;", 195}, {"&Auml;", 196}, {"&Aring;", 197}, {"&AElig;", 198},
		{"&Ccedil;", 199}, {"&Egrave;", 200}, {"&Eacute;", 201}, {"&Ecirc;", 202},
		{"&Euml;", 203}, {"&Igrave;", 204}, {"&Iacute;", 205}, {"&Icirc;", 206},
		{"&Iuml;", 207}, {"&ETH;", 208}, {"&Ntilde;", 209}, {"&Ograve;", 210},
		{"&Oacute;", 211}, {"&Ocirc;", 212}, {"&Otilde;", 213}, {"&Ouml;", 214},
		{"&times;", 215}, {"&Oslash;", 216}, {"&Ugrave;", 217}, {"&Uacute;", 218},
		{"&Ucirc;", 219}, {"&Uuml;", 220}, {"&Yacute;", 221}, {"&THORN;", 222},
		{"&szlig;", 223}, {"&agrave;", 224}, {"&aacute;", 225}, {"&acirc;", 226},
		{"&atilde;", 227}, {"&auml;", 228}, {"&aring;", 229}, {"&aelig;", 230},
		{"&ccedil;", 231}, {"&egrave;", 232}, {"&eacute;", 233}, {"&ecirc;", 234},
		{"&euml;", 235}, {"&igrave;", 236}, {"&iacute;", 237}, {"&icirc;", 238},
		{"&iuml;", 239}, {"&eth;", 240}, {"&ntilde;", 241}, {"&ograve;", 242},
		{"&oacute;", 243}, {"&ocirc;", 244}, {"&otilde;", 245}, {"&ouml;", 246}, {"&divide;", 247},
		{"&oslash;", 248}, {"&ugrave;", 249}, {"&uacute;", 250}, {"&ucirc;", 251},
		{"&uuml;", 252}, {"&yacute;", 253}, {"&thorn;", 254}, {"&yuml;", 255},
		// A.2.2. Special characters cont'd
		{"&OElig;", 338}, {"&oelig;", 339}, {"&Scaron;", 352}, {"&scaron;", 353}, {"&Yuml;", 376},
		// A.2.3. Symbols
		{"&fnof;", 402},
		// A.2.2. Special characters cont'd
		{"&circ;", 710}, {"&tilde;", 732},
		// A.2.3. Symbols cont'd
		{"&Alpha;", 913}, {"&Beta;", 914}, {"&Gamma;", 915}, {"&Delta;", 916},
		{"&Epsilon;", 917}, {"&Zeta;", 918}, {"&Eta;", 919}, {"&Theta;", 920},
		{"&Iota;", 921}, {"&Kappa;", 922}, {"&Lambda;", 923}, {"&Mu;", 924},
		{"&Nu;", 925}, {"&Xi;", 926}, {"&Omicron;", 927}, {"&Pi;", 928},
		{"&Rho;", 929}, {"&Sigma;", 931}, {"&Tau;", 932}, {"&Upsilon;", 933},
		{"&Phi;", 934}, {"&Chi;", 935}, {"&Psi;", 936}, {"&Omega;", 937},
		{"&alpha;", 945}, {"&beta;", 946}, {"&gamma;", 947}, {"&delta;", 948},
		{"&epsilon;", 949}, {"&zeta;", 950}, {"&eta;", 951}, {"&theta;", 952},
		{"&iota;", 953}, {"&kappa;", 954}, {"&lambda;", 955}, {"&mu;", 956},
		{"&nu;", 957}, {"&xi;", 958}, {"&omicron;", 959}, {"&pi;", 960},
		{"&rho;", 961}, {"&sigmaf;", 962}, {"&sigma;", 963}, {"&tau;", 964},
		{"&upsilon;", 965}, {"&phi;", 966}, {"&chi;", 967}, {"&psi;", 968},
		{"&omega;", 969}, {"&thetasym;", 977}, {"&upsih;", 978}, {"&piv;", 982},
		// A.2.2. Special characters cont'd
		{"&ensp;", 8194}, {"&emsp;", 8195}, {"&thinsp;", 8201}, {"&zwnj;", 8204}, {"&zwj;", 8205},
		{"&lrm;", 8206}, {"&rlm;", 8207}, {"&ndash;", 8211}, {"&mdash;", 8212},
		{"&lsquo;", 8216}, {"&rsquo;", 8217}, {"&sbquo;", 8218}, {"&ldquo;", 8220},
		{"&rdquo;", 8221}, {"&bdquo;", 8222}, {"&dagger;", 8224}, {"&Dagger;", 8225},
		// A.2.3. Symbols cont'd
		{"&bull;", 8226}, {"&hellip;", 8230},
		// A.2.2. Special characters cont'd
		{"&permil;", 8240},
		// A.2.3. Symbols cont'd
		{"&prime;", 8242}, {"&Prime;", 8243},
		// A.2.2. Special characters cont'd
		{"&lsaquo;", 8249}, {"&rsaquo;", 8250},
		// A.2.3. Symbols cont'd
		{"&oline;", 8254}, {"&frasl;", 8260},
		// A.2.2. Special characters cont'd
		{"&euro;", 8364},
		// A.2.3. Symbols cont'd
		{"&image;", 8465}, {"&weierp;", 8472}, {"&real;", 8476}, {"&trade;", 8482}, {"&alefsym;", 8501},
		{"&larr;", 8592}, {"&uarr;", 8593}, {"&rarr;", 8594}, {"&darr;", 8595},
		{"&harr;", 8596}, {"&crarr;", 8629}, {"&lArr;", 8656}, {"&uArr;", 8657},
		{"&rArr;", 8658}, {"&dArr;", 8659}, {"&hArr;", 8660}, {"&forall;", 8704},
		{"&part;", 8706}, {"&exist;", 8707}, {"&empty;", 8709}, {"&nabla;", 8711},
		{"&isin;", 8712}, {"&notin;", 8713}, {"&ni;", 8715}, {"&prod;", 8719},
		{"&sum;", 8721}, {"&minus;", 8722}, {"&lowast;", 8727}, {"&radic;", 8730},
		{"&prop;", 8733}, {"&infin;", 8734}, {"&ang;", 8736}, {"&and;", 8743},
		{"&or;", 8744}, {"&cap;", 8745}, {"&cup;", 8746}, {"&int;", 8747},
		{"&there4;", 8756}, {"&sim;", 8764}, {"&cong;", 8773}, {"&asymp;", 8776},
		{"&ne;", 8800}, {"&equiv;", 8801}, {"&le;", 8804}, {"&ge;", 8805},
		{"&sub;", 8834}, {"&sup;", 8835}, {"&nsub;", 8836}, {"&sube;", 8838},
		{"&supe;", 8839}, {"&oplus;", 8853}, {"&otimes;", 8855}, {"&perp;", 8869},
		{"&sdot;", 8901}, {"&lceil;", 8968}, {"&rceil;", 8969}, {"&lfloor;", 8970},
		{"&rfloor;", 8971}, {"&lang;", 9001}, {"&rang;", 9002}, {"&loz;", 9674},
		{"&spades;", 9824}, {"&clubs;", 9827}, {"&hearts;", 9829}, {"&diams;", 9830}
	};
	return map;
}

#endif /* __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ */

void REString::replaceWithLen(const char * charsStringValue, const char * withCharsStringValue, const REUInt32 firstLen, const REUInt32 secondLen)
{
	if (firstLen > 0) 
	{
		REUInt32 ocurencesCount = 0;
		char * mainString = _data;
		while ( (mainString = strstr(mainString, charsStringValue)) ) 
		{
			ocurencesCount++;
			mainString += firstLen;
		}
		
		if (ocurencesCount > 0) 
		{
			const REUInt32 resultLen = (_length + (ocurencesCount * secondLen)) - (ocurencesCount * firstLen);
			char * newData = (char *)REMem::Malloc(resultLen + 1);
			if (newData) 
			{
				mainString = _data;
				char * srcString = _data;
				char * dstString = newData;
				while ( (mainString = strstr(mainString, charsStringValue)) ) 
				{
					while (srcString != mainString) 
					{
						(*dstString) = (*srcString);
						dstString++;
						srcString++;
					}
					REUByte * addString = (REUByte *)withCharsStringValue;
					REUInt32 addLen = secondLen;
					while (addLen) 
					{
						(*dstString) = (*addString);
						dstString++;
						addString++;
						addLen--;
					}
					mainString += firstLen;
					srcString += firstLen;
				}
				while ((*srcString)) 
				{
					(*dstString) = (*srcString);
					dstString++;
					srcString++;
				}
				newData[resultLen] = 0;
				REMem::Free(_data);
				_data = newData;
				_length = (REUInt32)resultLen;
			}
		}
	}
}

REString & REString::replace(const char * charsStringValue, const char * withCharsStringValueOrNULL)
{
	if (charsStringValue && _data && _length) 
	{
		const REUInt32 firstLen = (REUInt32)strlen(charsStringValue);
		const REUInt32 secondLen = (withCharsStringValueOrNULL != 0) ? (REUInt32)strlen(withCharsStringValueOrNULL) : 0;
		this->replaceWithLen(charsStringValue, withCharsStringValueOrNULL, firstLen, secondLen);
	}
	return (*this);
}

REString & REString::replace(const wchar_t * wideStringValue, const wchar_t * withWideStringValueOrNULL)
{
	REStringPresentation p1(wideStringValue);
	REStringPresentation p2(withWideStringValueOrNULL);
	this->replaceWithLen(p1.UTF8String(), p2.UTF8String(), p1.getUTF8Length(), p2.getUTF8Length());
	return (*this);
}

REBOOL REString::isContaines(const char * charsStringValue) const
{
	if (charsStringValue && _data) 
	{
		return ( strstr(_data, charsStringValue) != 0);
	}
	return false;
}

REBOOL REString::isContaines(const wchar_t * wideStringValue) const
{
	REStringPresentation p(wideStringValue);
	return this->isContaines(p.UTF8String());
}

REString & REString::setWithLen(const char * charsStringValue, const REUInt32 stringLength)
{
    this->clear();
    
    if (charsStringValue) 
    {
        _data = (char *)REMem::Malloc(stringLength + 1);
        if (_data) 
        {
            REMem::Memcpy(_data, charsStringValue, stringLength);
            _data[stringLength] = 0;
            _length = stringLength;
        }
    }
    return (*this);
}

REString & REString::operator=(const char * charsStringValue)
{
	if (charsStringValue) 
	{
		return this->setWithLen(charsStringValue, (REUInt32)strlen(charsStringValue));
	}
    return (*this);
}

REString & REString::operator=(const wchar_t * wideStringValue)
{
	REStringPresentation p(wideStringValue);
	return this->setWithLen(p.UTF8String(), p.getUTF8Length());
}

REString & REString::operator=(const REString & anotherString)
{
	return this->set(anotherString);
}

REString & REString::operator=(const REStringPresentation & anotherStringPresentation)
{
	return this->setWithLen(anotherStringPresentation.UTF8String(), anotherStringPresentation.getUTF8Length());
}

void REString::appendWithLen(const char * charsStringValue, const REUInt32 len)
{
	const REUInt32 newLen = len + _length;
	char * newData = (char *)REMem::Malloc(newLen + 1);
	if (newData) 
	{
		if (_length > 0) { REMem::Memcpy(newData, _data, _length); }
		REMem::Memcpy(&newData[_length], charsStringValue, len);
		newData[newLen] = 0;
		if (_data) { REMem::Free(_data); }
		_data = newData;
		_length = newLen;
	}
}

REString & REString::append(const char * charsStringValue)
{
	if (charsStringValue) 
	{
		this->appendWithLen(charsStringValue, (REUInt32)strlen(charsStringValue));
	}
	return (*this);
}

REString & REString::append(const wchar_t * wideStringValue)
{
	REStringPresentation p(wideStringValue);
	if (p.UTF8String()) 
	{
		this->appendWithLen(p.UTF8String(), p.getUTF8Length());
	}
	return (*this);
}

REString & REString::appendFormat(const char * format, ...)
{
	if (format)
	{
		va_list args;
		va_start(args, format);
		char strBuff[512];
		int writed = vsprintf(strBuff, format, args);
		if (writed > 0)
		{
			this->appendWithLen(strBuff, (REUInt32)writed);
		}
		va_end(args);
	}
	return (*this);
}

REBOOL REString::isEqualWithLen(const char * charsStringValue, const REUInt32 stringLength) const
{
	if (_length == stringLength)
	{
		return _length ? (memcmp(_data, charsStringValue, stringLength) == 0) : true;
	}
	return false;
}

REBOOL REString::isEqual(const char * charsStringValue) const
{
	if (charsStringValue) 
	{
		return this->isEqualWithLen(charsStringValue, strlen(charsStringValue));
	}
	return false;
}

REBOOL REString::isEqual(const wchar_t * wideStringValue) const
{
	REStringPresentation p(wideStringValue);
	return this->isEqualWithLen(p.UTF8String(), p.getUTF8Length());
}

REBOOL REString::isEqual(const REString & anotherString) const
{
	return this->isEqualWithLen(anotherString._data, anotherString._length);
}

/// Compare two C strings ignore case. While checking strings not tested for wide chars avaiability.
/// Both strings shoudl containe only ascii chars.
/// Return 0 if equal or number of first ocurence of incompatible char.
REUInt32 REString::compareIgnoreCase(const char * firstString, const char * secondString)
{
	if (firstString && secondString) 
	{
		REUInt32 compareResult = 1;
		const char * s1 = firstString;
		const char * s2 = secondString;
		while ( (*s1) && (*s2) ) 
		{
			if (tolower(*s1) != tolower(*s2))
			{
				return compareResult; 
			}
			s1++;
			s2++;
			compareResult++;
		}
		if ( ((*s1) == 0) && ((*s2) == 0) ) 
		{
			return 0;
		}
		return compareResult;
	}
	return 1;
}

/// Compare two wide chars strings ignoring case.
REUInt32 REString::compareIgnoreCase(const wchar_t * firstString, const wchar_t * secondString)
{
	if (firstString && secondString) 
	{
		REUInt32 compareResult = 1;
		const wchar_t * s1 = firstString;
		const wchar_t * s2 = secondString;
		while ( (*s1) && (*s2) ) 
		{
			if (towlower(*s1) != towlower(*s2))
			{
				return compareResult; 
			}
			s1++;
			s2++;
			compareResult++;
		}
		if ( ((*s1) == 0) && ((*s2) == 0) ) 
		{
			return 0;
		}
		return compareResult;
	}
	return 1;
}

REBOOL REString::isEqualIgnoreCase(const char * charsStringValue) const
{
	if (_data && charsStringValue) 
	{
		REBOOL isNeedConversion = false;
		if (this->isNonASCIICharsPresent()) 
		{
			isNeedConversion = true;
		}
		else if (REString::isContainesNonASCIIChararacters(charsStringValue))
		{
			isNeedConversion = true;
		}
		else
		{
			return (REString::compareIgnoreCase(_data, charsStringValue) == 0);
		}
		
		if (isNeedConversion) 
		{
			REStringPresentation p1(_data);
			REStringPresentation p2(charsStringValue);
			return (REString::compareIgnoreCase(p1.wideString(), p2.wideString()) == 0);
		}
	}
	return false;
}

REBOOL REString::isEqualIgnoreCase(const wchar_t * wideStringValue) const
{
	if (_data && wideStringValue) 
	{
		REStringPresentation p1(_data);
		return (REString::compareIgnoreCase(p1.wideString(), wideStringValue) == 0);
	}
	return false;
}

REBOOL REString::isEqualIgnoreCase(const REString & anotherString) const
{
	if (_length == anotherString._length) 
	{
		return this->isEqualIgnoreCase(anotherString._data);
	}
	return false;
}

const char * REString::UTF8String() const
{
	return _data;
}

REString & REString::set(const char * charsStringValue)
{
	if (charsStringValue) 
	{
		return this->setWithLen(charsStringValue, (REUInt32)strlen(charsStringValue));
	}
	
	this->clear();
	return (*this);
}

REString & REString::set(const wchar_t * wideStringValue)
{
	this->clear();
	if (wideStringValue) 
	{
		this->set(wideStringValue, (REUInt32)wcslen(wideStringValue));
	}
	
	return (*this);
}

REString & REString::set(const REString & anotherString)
{
	if (anotherString._data && anotherString._length) 
	{
		return this->setWithLen(anotherString._data, anotherString._length);
	}
	
	this->clear();
	return (*this);
}

REString & REString::set(const char * charsStringValue, const REUInt32 stringLength)
{
	this->setWithLen(charsStringValue, stringLength);
	return (*this);
}

REString & REString::set(const wchar_t * wideStringValue, const REUInt32 stringLength)
{
	this->clear();
	
	if (wideStringValue) 
	{
		const int stringValueLen = (int)stringLength;
		_data = (char *)REMem::Malloc((stringValueLen + 1) * sizeof(wchar_t));
		if (_data) 
		{
			const int cLen = REStringPresentation::wideToChars(wideStringValue, stringValueLen, _data);
			if (cLen > 0) 
			{
				_length = (REUInt32)cLen;
				_data[cLen] = 0;
			}
			else
			{
				this->clear();
			}
		}
	}
	
	return (*this);
}

REString & REString::set(const REString & anotherString, const REUInt32 stringLength)
{
	this->setWithLen(anotherString.UTF8String(), anotherString.length());
	return (*this);
}

REString & REString::toLower()
{
	if (_data) 
	{
		if (this->isNonASCIICharsPresent()) 
		{
			REStringPresentation p(_data);
			wchar_t * s = const_cast<wchar_t *>(p.wideString());
			if (s) 
			{
				while (*s) 
				{
					*s = (wchar_t)towlower(*s);
					s++;
				}
			}
			this->set((const wchar_t *)s);
		}
		else
		{
			char * s = _data;
			while (*s) 
			{
				*s = (char)tolower(*s);
				s++;
			}
		}
	}
	return (*this);
}

REString & REString::toUpper()
{
	if (_data) 
	{
		if (this->isNonASCIICharsPresent()) 
		{
			REStringPresentation p(_data);
			wchar_t * s = const_cast<wchar_t *>(p.wideString());
			if (s) 
			{
				while (*s) 
				{
					*s = (wchar_t)towupper(*s);
					s++;
				}
			}
			this->set((const wchar_t *)s);
		}
		else
		{
			char * s = _data;
			while (*s) 
			{
				*s = (char)toupper(*s);
				s++;
			}
		}
	}
	return (*this);
}

REBOOL REString::isEmpty() const
{
	return (_length == 0);
}

REBOOL REString::isDigit(const char * charsStringValue)
{
	int dig = (*charsStringValue);
	while ( dig )
	{
		if (!isdigit(dig))
		{
			if ((dig != '+') || (dig != '-') || (dig != '.'))
			{
				return false;
			}
		}
		charsStringValue++;
		dig = (int)(*charsStringValue);
	}
	return true;
}

REBOOL REString::isDigit() const
{
	if (_data) 
	{
		return REString::isDigit((const char *)_data);
	}
	return false;
}

/// Check C string for containing of wide character.
REBOOL REString::isContainesNonASCIIChararacters(const char * someString)
{
	if (someString) 
	{
		unsigned char * ch = (unsigned char *)someString;
		while (*ch) 
		{
			const wchar_t u = (*ch);
			if (u >= ((unsigned char)0x80))
			{
				return true;
			}
			ch++;
		}
	}
	return false;
}

REBOOL REString::isNonASCIICharsPresent() const
{
	return REString::isContainesNonASCIIChararacters(_data);	
}

const REUInt32 REString::length() const
{
	return _length;
}

REString & REString::clear()
{
	if (_data) 
	{
		REMem::Free(_data);
		_data = NULL;
	}
	_length = 0;
	return (*this);
}

REString::REString(const wchar_t * wideStringValue) : _data(NULL), _length(0)
{
	this->set(wideStringValue);
}

REString::REString(const wchar_t * wideStringValue, const REUInt32 stringLength) : _data(NULL), _length(0)
{
	this->set(wideStringValue, stringLength);
}

REString::REString(const char * charsStringValue, const REUInt32 stringLength) : _data(NULL), _length(0)
{
    if (charsStringValue) 
	{
		this->setWithLen(charsStringValue, stringLength);
	}
}

REString::REString(const char * charsStringValue, const RERange & rangeFromString) : _data(NULL), _length(0)
{
	if (charsStringValue && rangeFromString.isExists()) 
	{
		const REUInt32 len = (REUInt32)strlen(charsStringValue);
		if (len >= (rangeFromString.location + rangeFromString.length)) 
		{
			this->setWithLen(&charsStringValue[rangeFromString.location], rangeFromString.length);
		}
	}
}

REString::REString(const char * charsStringValue) : _data(NULL), _length(0)
{
	if (charsStringValue) 
	{
		this->setWithLen(charsStringValue, (REUInt32)strlen(charsStringValue));
	}
}

REString::REString(const REString & anotherString) : _data(NULL), _length(0)
{
	if (anotherString._data && anotherString._length) 
	{
		this->setWithLen(anotherString._data, anotherString._length);
	}
}

REString::REString(const REStringPresentation & anotherStringPresentation) : _data(NULL), _length(0)
{
	if (anotherStringPresentation.getUTF8Length()) 
	{
		this->set(anotherStringPresentation.UTF8String(), anotherStringPresentation.getUTF8Length());
	}
}

REString::REString() : _data(NULL), _length(0)
{
	
}

REString::~REString()
{
	if (_data) { REMem::Free(_data); }
}



void REStringPresentation::clear()
{
	if (_c) 
	{
		REMem::Free(_c);
		_c = NULL;
	}
	if (_w) 
	{
		REMem::Free(_w);
		_w = NULL;
	}
	_cLen = 0;
	_wLen = 0;
}

void REStringPresentation::setCWithLen(const char * stringValue, const REUInt32 len)
{
	this->clear();
	
	if (stringValue) 
	{
		const int stringValueLen = (int)len;
		_w = (wchar_t *)REMem::Malloc((stringValueLen + 1) * sizeof(wchar_t));
		_c = (char *)REMem::Malloc(stringValueLen + 1);
		if (_w && _c) 
		{
			const int wideLen = REStringPresentation::charsToWide(stringValue, stringValueLen, _w);
			if (wideLen > 0) 
			{
				_cLen = (REUInt32)stringValueLen;
				REMem::Memcpy(_c, stringValue, _cLen);
				_c[_cLen] = 0;
				
				_wLen = (REUInt32)wideLen;
				_w[_wLen] = 0;
			}
			else 
			{
				this->clear();
			}
		}
		else
		{
			this->clear();
		}
	}
}

void REStringPresentation::setWWithLen(const wchar_t * stringValue, const REUInt32 len)
{
	this->clear();
	if (stringValue) 
	{
		const int stringValueLen = (int)len;
		_w = (wchar_t *)REMem::Malloc((stringValueLen + 1) * sizeof(wchar_t));
		_c = (char *)REMem::Malloc((stringValueLen + 1) * sizeof(wchar_t));
		if (_w && _c) 
		{
			const int cLen = REStringPresentation::wideToChars(stringValue, stringValueLen, _c);
			if (cLen > 0) 
			{
				_cLen = (REUInt32)cLen;
				_c[_cLen] = 0;
				
				REMem::Memcpy(_w, stringValue, stringValueLen * sizeof(wchar_t));
				_wLen = (REUInt32)stringValueLen;
				_w[_wLen] = 0;
			}
			else
			{
				this->clear();
			}
		}
		else 
		{
			this->clear();
		}
	}
}

void REStringPresentation::setC(const char * stringValue)
{
	if (stringValue) 
	{
		const REUInt32 stringValueLen = (REUInt32)strlen(stringValue);
		this->setCWithLen(stringValue, stringValueLen);
	}
	else
	{
		this->clear();
	}
}

void REStringPresentation::setW(const wchar_t * stringValue)
{
	if (stringValue) 
	{
		const REUInt32 stringValueLen = (REUInt32)wcslen(stringValue);
		this->setWWithLen(stringValue, stringValueLen);
	}
	else
	{
		this->clear();
	}
}

const char * REStringPresentation::UTF8String() const { return _c; }
const REUInt32 REStringPresentation::getUTF8Length() const { return _cLen; }
const wchar_t * REStringPresentation::wideString() const { return _w; }
const REUInt32 REStringPresentation::getWideLength() const { return _wLen; }
REStringPresentation & REStringPresentation::operator=(const REStringPresentation & stringValue)
{
	this->setC(stringValue._c);
	return (*this);
}

REStringPresentation & REStringPresentation::operator=(const REString & stringValue)
{
	this->setC(stringValue.UTF8String());
	return (*this);
}

REStringPresentation::REStringPresentation(const char * stringValue) : 
_c(NULL),
_w(NULL),
_cLen(0),
_wLen(0)
{
	this->setC(stringValue);
}

REStringPresentation::REStringPresentation(const wchar_t * stringValue) : 
_c(NULL),
_w(NULL),
_cLen(0),
_wLen(0)
{
	this->setW(stringValue);
}

REStringPresentation::REStringPresentation(const REStringPresentation & stringValue) : 
_c(NULL),
_w(NULL),
_cLen(0),
_wLen(0)
{
	this->setC(stringValue._c);
}

REStringPresentation::REStringPresentation(const REString & stringValue) : 
_c(NULL),
_w(NULL),
_cLen(0),
_wLen(0)
{
	this->setC(stringValue.UTF8String());
}

REStringPresentation::REStringPresentation() : 
_c(NULL),
_w(NULL),
_cLen(0),
_wLen(0)
{
	
}

REStringPresentation::~REStringPresentation()
{
	this->clear();
}

int REStringPresentation::wideToChars(const wchar_t * wideString, int wideStringLength, char * charsString)
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

int REStringPresentation::charsToWide(const char * charsString, int charsStringLengthOrNegative, wchar_t * wideString)
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

void REString::appendPathComponentToString(const char * pComponent, REString & string)
{
	const char * component = pComponent;
	REUInt32 componentLen = (REUInt32)strlen(component);
	if (componentLen == 0) { return; }
	
	while ( (*component) == '/' )
	{
		component++;
		componentLen--;
	}
	
	bool allreadyWithSeparator = false;
	if (string._data && string._length)
	{
		allreadyWithSeparator = (string._data[string._length - 1] == '/');
	}
	
	if (allreadyWithSeparator)
	{
		string.appendWithLen(component, componentLen);
	}
	else
	{
		string.appendFormat("/%s", component);
	}
	
	if (string._data && string._length)
	{
		if (string._data[string._length - 1] == '/')
		{
			string._data[string._length - 1] = 0;
			string._length--;
		}
	}
}

REString & REString::appendPathComponent(const char * pComponent)
{
	if (this->isEmpty()) 
	{
		this->set(pComponent);
		return (*this);
	}
	
	if (pComponent) 
	{
		REString::appendPathComponentToString(pComponent, *this);
	}
	return (*this);
}

void REString::extractLastPathComponent(const REString & source, REString & lastPathComponent)
{
	REUInt32 srcLen = source.length();
	if (srcLen)
	{
		const char * src = &source.UTF8String()[srcLen - 1];
		if ( (*src) == '/') { srcLen--; src--; }
		REUInt32 needLen = 0;
		while (srcLen)
		{
			if ( (*src) != '/' )
			{
				src--;
				srcLen--;
				needLen++;
			}
			else
			{
				if (needLen) src++;
				lastPathComponent.setWithLen(src, needLen);
				return;
			}
		}
	}
	lastPathComponent.set("");
}

REString REString::getLastPathComponent() const
{
	REString lastPathComponent;
	REString::extractLastPathComponent(*this, lastPathComponent);
	return lastPathComponent;
}

void REString::extractPathExtension(const REString & source, REString & pathExtension)
{
	REUInt32 srcLen = source.length();
	if (srcLen)
	{
		const char * src = &source.UTF8String()[srcLen - 1];
		if ( (*src) == '/') { srcLen--; src--; }
		REUInt32 needLen = 0;
		while (srcLen)
		{
			if ( (*src) != '.' )
			{
				src--;
				srcLen--;
				needLen++;
			}
			else
			{
				if (needLen) src++;
				pathExtension.setWithLen(src, needLen);
				return;
			}
		}
	}
	pathExtension.set("");
}

REString REString::getPathExtension() const
{
	REString pathExtension;
	REString::extractPathExtension(*this, pathExtension);
	return pathExtension;
}

REString & REString::removePathExtension()
{
	if (_data && _length) 
	{
		REUInt32 srcLen = _length;
		char * src = &_data[srcLen - 1];
		
		if ( (*src) == '/') { srcLen--; src--; }
		
		while (srcLen) 
		{
			if ( (*src) != '.' )
			{
				src--;
				srcLen--;
			}
			else
			{
				(*src) = 0;
				_length = srcLen - 1;
				return (*this);
			}
		}
	}
	return (*this);
}

REBOOL REString::initFromURLString(const REString & urlString)
{
	this->clear();
	
#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
	REURL url(urlString);
	if (url.IsFileURL()) 
	{
		REString hiPart(url.GetHierarchicalPart());
		REData data;
		if (data.InitFromPath(hiPart)) 
		{
			this->SetWithLen((const char *)data.GetBytes(), data.GetSize());
			return true;
		}
		return false;
	}
	else
	{
		REURLRequestObject * request = REURLRequestObject::CreateWithURL(url);
		if (request) 
		{
			REBuffer downlBuff;
			const REBOOL isSended = REURLConnectionObject::SendRequest(request, &downlBuff, NULL);
			request->release();
			if (isSended)
			{
				if (downlBuff.GetSize()) 
				{
					this->SetWithLen((const char *)downlBuff.GetBuffer(), downlBuff.GetSize());
				}
				return true;
			}
		}
	}
#endif	
	return false;
}

REString & REString::decodeSpecialHTMLCharacters()
{

#ifndef __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ 	
	
	if (this->length() < 3) { return (*this); }
	
	REStringPresentation p(*this);
	REUInt32 len = p.getUTF8Length();
	if (len < 3) { return (*this); }
	
	const char * stringPtr = p.UTF8String();
	REBuffer buff(len + 1);
	if (buff.getSize() != (len + 1)) { return (*this); }
	
	REStringHTMLSpecialCharacterPrivate htmlCh;
	char * dataString = (char *)buff.getBuffer();
	while (len) 
	{
		REBOOL notReplaced = true;
		if (*stringPtr == '&')
		{
			const REStringHTMLSpecialCharacterPrivate::MapStruct * elem = htmlCh.GetFromStr(stringPtr, len);
			if (elem) 
			{
				if (elem->ucharValue <= 255) 
				{
					*dataString++ = (char)elem->ucharValue;
				}
				else 
				{
					char repC[sizeof(wchar_t) * 2] = { 0 };
					wchar_t repW[2] = { elem->ucharValue, 0 };
					REStringPresentation::wideToChars(repW, 2, repC);
					const char * pr = repC;
					while (*pr) { *dataString++ = *pr++; }
				}
				const REUInt32 l = (REUInt32)strlen(elem->strValue);
				stringPtr += l;
				len -= l;
				notReplaced = false;
			}
		}
		if (notReplaced) 
		{
			*dataString++ = *stringPtr++;
			len--;
		}
	}
	
	p.clear();
	
	*dataString = 0;
	
	this->setWithLen((const char *)buff.getBuffer(), buff.getSize() - 1);

#endif /* __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ */	
	
	return (*this);
}

REString & REString::encodeWithSpecialHTMLCharacters()
{
	
#ifndef __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ 
	
	if (this->isEmpty()) { return (*this); }
	
	REStringPresentation p(*this);
	if (p.getWideLength() == 0) { return (*this); }
	
	
	REUInt32 buffSize = (p.getUTF8Length() + 256) * sizeof(wchar_t);
	REUInt32 dSize = 0;
	REBuffer buff(buffSize);
	if (buff.getSize() != buffSize) { return (*this); }
	
	REStringHTMLSpecialCharacterPrivate htmlCh;
	const wchar_t * s = p.wideString();
	wchar_t * d = (wchar_t *)buff.getBuffer();
	while (*s) 
	{
		const REStringHTMLSpecialCharacterPrivate::MapStruct * elem = htmlCh.GetForWChar(*s);
		if (elem) 
		{
			REStringPresentation pr(elem->strValue);
			const wchar_t * prW = pr.wideString();
			while (*prW) 
			{
				if (dSize == buffSize) 
				{
					buffSize += (512 * sizeof(wchar_t));
					if (!buff.resize(buffSize, true)) { return (*this); }
					wchar_t * d1 = (wchar_t *)buff.getBuffer();
					d = &d1[dSize / sizeof(wchar_t)];
				}
				*d++ = *prW++;
				dSize += sizeof(wchar_t);
			}
			s++;
		}
		else
		{
			if (dSize == buffSize) 
			{
				buffSize += (512 * sizeof(wchar_t));
				if (!buff.resize(buffSize, true)) { return (*this); }
				wchar_t * d1 = (wchar_t *)buff.getBuffer();
				d = &d1[dSize / sizeof(wchar_t)];
			}
			*d++ = *s++;
			dSize += sizeof(wchar_t);
		}
	}
	
	p.clear();
	
	if (dSize == buffSize) 
	{
		buffSize += (2 * sizeof(wchar_t));
		if (!buff.resize(buffSize, true)) { return (*this); }
		wchar_t * d1 = (wchar_t *)buff.getBuffer();
		d = &d1[dSize / sizeof(wchar_t)];
	}
	
	*d = 0;
	
	REStringPresentation prF((const wchar_t *)buff.getBuffer());
	
	this->setWithLen(prF.UTF8String(), prF.getUTF8Length());

#endif /* __RE_RECORE_NO_STRING_DECODE_ENCODE_SPECIAL_HTML_CHARACTERS__ */	
	
	return (*this);
}

