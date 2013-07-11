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


#include "../../include/recore/REXMLPropertyListReader.h"
#include "../../include/recore/REDictionaryObject.h"

#include "../addlibs/tinyxml.h"

using namespace tinyxml2;


void REXMLPropertyListReader::ClearPairs(REArray<REDictionaryObject::KeyObjectStruct> * pairs)
{
	for (REUInt32 i = 0; i < pairs->count(); i++) 
	{
		REDictionaryObject::KeyObjectStruct * p = &((*pairs)[i]);
		if (p->keyValue) 
		{
			p->keyValue->release();
		}
		if (p->objValue) 
		{
			p->objValue->release();
		}
	}
}

REObject * REXMLPropertyListReader::NewObjectFromElement(void * elementObject)
{
	XMLElement * elem = (XMLElement *)elementObject;
	const char * eVal = elem->Value();
	if (eVal) 
	{
		const REUInt32 valLen = (REUInt32)strlen(eVal);
		switch (valLen) 
		{
			case 4:
				if (strncmp(eVal, "real", 4) == 0)
				{
					const char * numberString = elem->GetText();
					if (numberString) 
					{
						RENumberObject * numberValue = RENumberObject::createWithStringPresentation(numberString);
						if (numberValue) 
						{
							return numberValue;
						}
					}
				}
				else if (strncmp(eVal, "data", 4) == 0)
				{
					REString base64String(elem->GetText());
					if (!base64String.isEmpty()) 
					{
						REBufferObject * bufferValue = REBufferObject::create();
						if (bufferValue) 
						{
							REBase64 b;
							if ( b.base64StringToBuffer(base64String, bufferValue) )
							{
								return bufferValue;
							}
							else
							{
								bufferValue->release();
							}
						}
					}
				}
				else if (strncmp(eVal, "dict", 4) == 0)
				{
					REDictionaryObject * dictValue = REDictionaryObject::create();
					if (dictValue) 
					{
						REArray<REDictionaryObject::KeyObjectStruct> * newPairs = dictValue->getPairs();
						if (newPairs) 
						{
							if ( REXMLPropertyListReader::ParseDictionaryElement(newPairs, elem) ) 
							{
								return dictValue;
							}
							else
							{
								dictValue->release();
							}
						}
						else
						{
							dictValue->release();
						}
					}
				}
				else if (strncmp(eVal, "true", 4) == 0)
				{
					RENumberObject * numberValue = RENumberObject::createWithBool((REBOOL)true);
					if (numberValue) 
					{
						return numberValue;
					}
				}
				break;
				
				
			case 5:
				if (strncmp(eVal, "array", 5) == 0)
				{
					REArrayObject * arr = REArrayObject::create();
					if (arr) 
					{
						for(XMLElement * arrElem = elem->FirstChildElement(); arrElem != NULL; arrElem = arrElem->NextSiblingElement() )
						{
							REObject * arrObjValue = REXMLPropertyListReader::NewObjectFromElement(arrElem);
							if (arrObjValue) 
							{
								arr->add(arrObjValue);
							}
						}
					}
					return arr;
				}
				else if (strncmp(eVal, "false", 5) == 0)
				{
					RENumberObject * numberValue = RENumberObject::createWithBool((REBOOL)false);
					if (numberValue) 
					{
						return numberValue;
					}
				}
				break;
				
			case 6:
				if (strncmp(eVal, "string", 6) == 0)
				{
					REStringObject * stringValue = REStringObject::createWithChars(elem->GetText());
					return stringValue;
				}
				break;
				
				
			case 7:
				if (strncmp(eVal, "integer", 7) == 0)
				{
					const char * numberString = elem->GetText();
					if (numberString) 
					{
						RENumberObject * numberValue = RENumberObject::createWithStringPresentation(numberString);
						if (numberValue) 
						{
							return numberValue;
						}
					}
				}
				break;
				
				
			default:
				break;
		}
	}
	return NULL;
}

REBOOL REXMLPropertyListReader::ParseDictionaryElement(REArray<REDictionaryObject::KeyObjectStruct> * pairs, void * dictionaryElement)
{
	REStringObject * keyString = NULL;
	XMLElement * dictElement = (XMLElement *)dictionaryElement;
	for(XMLElement * elem = dictElement->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement() )
	{
		const char * eVal = elem->Value();
		if (eVal) 
		{
			const REUInt32 valLen = (REUInt32)strlen(eVal);
			if (valLen == 3) 
			{
				if (strncmp(eVal, "key", 3) == 0) 
				{
					RE_SAFE_RELEASE(keyString);
					keyString = REStringObject::createWithChars(elem->GetText());
					if (keyString) 
					{
						if (keyString->isEmpty()) 
						{
							keyString->release();
							return false;
						}
					}
				}
			}
			else if (keyString)
			{
				REObject * objValue = REXMLPropertyListReader::NewObjectFromElement(elem);
				if (objValue) 
				{
					REDictionaryObject::KeyObjectStruct pair;
					pair.keyValue = keyString;
					pair.objValue = objValue;
					pairs->add(pair);
					keyString = NULL;
				}
			}
			
		}
	}
	
	if (keyString) 
	{
		keyString->release();
	}
	return true;
}

REBOOL REXMLPropertyListReader::ReadPropertyListElement(REArray<REDictionaryObject::KeyObjectStruct> * pairs, void * propListElement)
{
	XMLElement * element = (XMLElement *)propListElement;
	
	for (XMLElement * dictElem = element->FirstChildElement(); dictElem != NULL; dictElem = dictElem->NextSiblingElement()) 
	{
		const char * nodeValue = dictElem->Value();
		if (nodeValue) 
		{
			if (strcmp(nodeValue, "dict") == 0) 
			{
				return REXMLPropertyListReader::ParseDictionaryElement(pairs, dictElem);
			}
		}
	}
	return false;
}

REBOOL REXMLPropertyListReader::ReadFromString(const char * listString, REArray<REDictionaryObject::KeyObjectStruct> * pairs)
{
	if ((listString == NULL) || (pairs == NULL) ) { return false; }
	
	REXMLPropertyListReader::ClearPairs(pairs);
	
	XMLDocument doc;
	doc.Parse(listString);
	
	if (doc.Error()) { return false; }
	
	XMLElement * root = doc.RootElement();
	if (root == NULL) { return false; }
	
	const char * rootVal = root->Value();
	
	if (rootVal) 
	{
		if (strcmp(rootVal, "plist") == 0) 
		{
			return REXMLPropertyListReader::ReadPropertyListElement(pairs, root);
		}
	}
	return false;
}

REBOOL REXMLPropertyListReader::ReadFromString(const REString & listString, REArray<REDictionaryObject::KeyObjectStruct> * pairs)
{
	return this->ReadFromString(listString.getChars(), pairs);
}

REXMLPropertyListReader::REXMLPropertyListReader()
{
	
}

REXMLPropertyListReader::~REXMLPropertyListReader()
{
	
}



