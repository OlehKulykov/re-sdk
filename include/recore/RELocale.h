/*
 *   Copyright 2012 Kulykov Oleh
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


#ifndef __RELOCALE_H__
#define __RELOCALE_H__

#include "RECommonHeader.h"
#include "REString.h"
#include "RENotificationManager.h"

/// Type of avaiable locales.
typedef enum _reLocaleType
{
	RELocaleUndefined = 0,
	RELocaleRussianRussia = 1,
	RELocaleEnglishUnitedStates = 2,
	RELocaleFrenchFrance = 3,
	RELocaleItalianItaly = 4,
	RELocaleGermanGermany = 5,
	RELocalePortugueseBrazil = 6,
	RELocaleDanishDenmark = 7,
	RELocaleDutchNetherlands = 8,
	RELocaleJapaneseJapan = 9,
	RELocaleSpanishSpain = 10,
	RELocaleSwedishSweden = 11
}
/// Type of avaiable locales.
RELocaleType;

/// Class of locale
class __RE_PUBLIC_CLASS_API__ RELocale 
{
private:
	static RELocale * _defaultLocales;
	RELocaleType _currentLocaleType;
	RELocaleType SetCurrentLocaleFromString(const char * locString);
	void TryDetectLocale();
	RELocale();
	~RELocale();
public:
	/// Returns current locale type.
	RELocaleType GetCurrentLocaleType();
	
	/// Returns current locale string.
	const char * GetCurrentLocaleString();
	
	/// Assigning current locale type.
	void SetCurrentLocaleType(RELocaleType newLocaleType);
	
	/// Check is locale defined.
	REBOOL IsDefined();
	
	/// Returns notification name string when current locale changed.
	static const char * GetNotificationStringForCurrentLocaleChanged();
	
	/// Returns locale string from it's type.
	static const char * GetLocaleStringFromType(const RELocaleType type);
	
	/// Returns default locale class.
	static RELocale * GetDefaultLocale();
	
	/// Releases default locale class.
	static void ReleaseDefaultLocale();
};

#endif

