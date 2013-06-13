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

class RELocalePrivate 
{	
public:
	static const char ** AvaiableLocalesStrings();
};

const char ** RELocalePrivate::AvaiableLocalesStrings()
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

RELocaleType RELocale::SetCurrentLocaleFromString(const char * locString)
{
	if (locString)
	{
		REString loc(locString);
		loc.Replace("-", "_");
		locString = loc.UTF8String();
		const char ** arr = RELocalePrivate::AvaiableLocalesStrings();
		
		size_t compareLenght = 0;
		if (loc.Length() >= 5)
		{
			compareLenght = 5;
		}
		else if (loc.Length() >= 2)
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
				this->SetCurrentLocaleType((RELocaleType)i);
				return _currentLocaleType;
			}
		}
	}
	
	return _currentLocaleType;
}

#ifdef __RE_OS_WINDOWS__
void RELocale::TryDetectLocale()
{

}
#endif

#ifdef __RE_OS_LINUX__
void RELocale::TryDetectLocale()
{

}
#endif

RELocaleType RELocale::GetCurrentLocaleType()
{
	return _currentLocaleType;
}

const char * RELocale::GetCurrentLocaleString()
{
	unsigned int localeIndex = (unsigned int)_currentLocaleType;
	const char ** arr = RELocalePrivate::AvaiableLocalesStrings();
	return arr[localeIndex];
}

REBOOL RELocale::IsDefined()
{
	return (_currentLocaleType != RELocaleUndefined);
}

void RELocale::SetCurrentLocaleType(RELocaleType newLocaleType)
{
	if (_currentLocaleType != newLocaleType) 
	{
		_currentLocaleType = newLocaleType;
		const char * notCString = RELocale::GetNotificationStringForCurrentLocaleChanged();
		RENotificationManager::PostNotificationName(REString(notCString));
	}
}

RELocale::RELocale() : 
	_currentLocaleType(RELocaleUndefined)
{
	
}

RELocale::~RELocale()
{
	
}

const char * RELocale::GetNotificationStringForCurrentLocaleChanged()
{
	return "RELocales_curLocChanged";
}

const char * RELocale::GetLocaleStringFromType(const RELocaleType type)
{
	unsigned int localeIndex = (unsigned int)type;
	const char ** arr = RELocalePrivate::AvaiableLocalesStrings();
	return arr[localeIndex];
}

RELocale * RELocale::GetDefaultLocale()
{
	if (_defaultLocales == NULL) 
	{
		_defaultLocales = new RELocale();
		if (_defaultLocales) 
		{
			_defaultLocales->TryDetectLocale();
		}
	}
	return _defaultLocales;
}

void RELocale::ReleaseDefaultLocale()
{
	RE_SAFE_DELETE(_defaultLocales);
}


