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


#include "../../include/recore/RELocale.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REMutableString.h"

class RELocalePrivate 
{	
public:
	static const char ** avaiableLocalesStrings();
};

const char ** RELocalePrivate::avaiableLocalesStrings()
{
	static const char * arr[12] = 
	{ 
		"", 
		"ru_RU", 
		"en_US", 
		"fr_FR", 
		"it_IT", 
		"de_DE", 
		"pt_BR",
		"da_DK",
		"nl_NL",
		"ja_JP",
		"es_ES",
		"sv_SE"
	};
	
	return arr;
}

__RE_PUBLIC_CLASS_API__ RELocale * RELocale::_defaultLocales = NULL;

RELocaleType RELocale::setCurrentLocaleFromString(const char * locString)
{
	if (locString)
	{
		REMutableString loc(locString);
		loc.replace("-", "_");
		locString = loc.UTF8String();
		const char ** arr = RELocalePrivate::avaiableLocalesStrings();
		
		size_t compareLenght = 0;
		if (loc.length() >= 5)
		{
			compareLenght = 5;
		}
		else if (loc.length() >= 2)
		{
			compareLenght = 2;
		}
		else
		{
			return _currentLocaleType;
		}
		
		for (REUInt32 i = (REUInt32)RELocaleRussianRussia; i <= (REUInt32)RELocaleSwedishSweden; i++)
		{
			if (strncmp(arr[i], locString, compareLenght) == 0)
			{
				this->setCurrentLocaleType((RELocaleType)i);
				return _currentLocaleType;
			}
		}
	}
	
	return _currentLocaleType;
}

#ifdef __RE_OS_WINDOWS__
void RELocale::tryDetectLocale()
{

}
#endif

#ifdef __RE_OS_LINUX__
void RELocale::tryDetectLocale()
{

}
#endif

RELocaleType RELocale::currentLocaleType()
{
	return _currentLocaleType;
}

const char * RELocale::currentLocaleString()
{
	unsigned int localeIndex = (unsigned int)_currentLocaleType;
	const char ** arr = RELocalePrivate::avaiableLocalesStrings();
	return arr[localeIndex];
}

REBOOL RELocale::isDefined()
{
	return (_currentLocaleType != RELocaleUndefined);
}

void RELocale::setCurrentLocaleType(RELocaleType newLocaleType)
{
	if (_currentLocaleType != newLocaleType) 
	{
		_currentLocaleType = newLocaleType;
		const char * notCString = RELocale::notificationStringForCurrentLocaleChanged();
		RENotificationManager::postNotificationName(REString(notCString));
	}
}

RELocale::RELocale() : 
	_currentLocaleType(RELocaleUndefined)
{
	
}

RELocale::~RELocale()
{
	
}

const char * RELocale::notificationStringForCurrentLocaleChanged()
{
	return "RELocales_curLocChanged";
}

const char * RELocale::localeStringFromType(const RELocaleType type)
{
	unsigned int localeIndex = (unsigned int)type;
	const char ** arr = RELocalePrivate::avaiableLocalesStrings();
	return arr[localeIndex];
}

RELocale * RELocale::defaultLocale()
{
	if (_defaultLocales == NULL) 
	{
		_defaultLocales = new RELocale();
		if (_defaultLocales) 
		{
			_defaultLocales->tryDetectLocale();
		}
	}
	return _defaultLocales;
}

void RELocale::releaseDefaultLocale()
{
	RE_SAFE_DELETE(_defaultLocales);
}


