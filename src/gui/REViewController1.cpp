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


#include "../../include/regui/REViewController1.h"
#include "../../include/recore/REMutableString.h"
#include "../../include/recore/REData.h"
#include "../../include/recore/REDictionary.h"

REBOOL REViewController::initWithName(const REString & name)
{
	if (name.isNotEmpty())
	{
		REString path = REString::createWithFormat("data/vc/%s/vc.json", name.UTF8String());
		REData data;
		if (data.initFromPath(path)) 
		{
			RETypedPtr d(new REDictionary(), REPtrTypeDictionary);
			REDictionary * dict = d.dictionary();
			if (dict) 
			{
				if (dict->initializeFromJSONData(data.bytes(), data.size())) 
				{
					data.clear();
					this->deserializeWithDictionary(d);
					return true;
				}
			}
		}
	}
	return false;
}

REViewController::REViewController()
{
	
}

REViewController::~REViewController()
{
	
}

RETypedPtr REViewController::serializeToDictionary() const
{
	RETypedPtr d(REView::serializeToDictionary());
	//TODO: 
	return d;
}

void REViewController::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	REView::deserializeWithDictionary(dictionary);
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{

	}
}

REViewController * REViewController::create()
{
	REViewController * vc = new REViewController();
	return vc;
}

