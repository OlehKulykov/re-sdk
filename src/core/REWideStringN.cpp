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


#include "../../include/recore/REWideStringN.h"
#include "../../include/recore/REStringN.h"
#include "../../include/recore/REMutableStringN.h"

#include "../../include/recore/private/REStringUtilsPrivate.h"

REStringN REWideStringN::getString() const
{
	return REStringN( REStringUtilsPrivate::getUTF8FromWide(this->getWideChars(), 
															this->getLength()) );
}

REMutableStringN REWideStringN::getMutableString() const
{
	return REMutableStringN( REStringUtilsPrivate::getUTF8FromWide(this->getWideChars(), 
																   this->getLength()) );
}

const REUInt32 REWideStringN::getLength() const
{
	return REStringUtilsPrivate::stringLengthFromWideBuffer(_p);
}

const wchar_t * REWideStringN::getWideChars() const
{
	const REBuffer * b = _p;
	return b ? (const wchar_t *)b->getBuffer() : NULL;
}

REWideStringN & REWideStringN::operator=(const char * utf8String)
{
	_p = REStringUtilsPrivate::getWideFromUTF8(utf8String, 
											   REStringUtilsPrivate::UTF8StringLength(utf8String));
	return (*this);
}

REWideStringN & REWideStringN::operator=(const wchar_t * wideString)
{
	_p = REStringUtilsPrivate::newBuffForWideString(wideString);
	return (*this);
}

REWideStringN & REWideStringN::operator=(const REWideStringN & anotherString)
{
	_p = anotherString._p;
	return (*this);
}

REWideStringN & REWideStringN::operator=(const REStringN & anotherString)
{
	_p = REStringUtilsPrivate::getWideFromUTF8(anotherString.getChars(), anotherString.getLength());
	return (*this);
}

REWideStringN & REWideStringN::operator=(const REMutableStringN & anotherString)
{
	_p = REStringUtilsPrivate::getWideFromUTF8(anotherString.getChars(), anotherString.getLength());
	return (*this);
}

REWideStringN::REWideStringN()
{
	
}

REWideStringN::REWideStringN(const char * utf8String, 
							 const REUInt32 utf8StringLength) :
	REStringBase(utf8String, utf8StringLength, REStringTypeWide)
{
	
}

REWideStringN::REWideStringN(const wchar_t * wideString, 
							 const REUInt32 wideStringLength) :
	REStringBase(wideString, wideStringLength, REStringTypeWide)
{
	
}

REWideStringN::REWideStringN(const REWideStringN & anotherString) : 
	REStringBase(anotherString._p)
{
	
}

REWideStringN::REWideStringN(const REStringN & anotherString) : 
	REStringBase(anotherString.getChars(), anotherString.getLength(), REStringTypeWide)
{
	
}

REWideStringN::REWideStringN(const REMutableStringN & anotherString) : 
	REStringBase(anotherString.getChars(), anotherString.getLength(), REStringTypeWide)
{
	
}

REWideStringN::REWideStringN(const REPtr<REBuffer> & wideStringBuffer) :
	REStringBase(wideStringBuffer)
{
	
}

REWideStringN::~REWideStringN()
{
	
}

