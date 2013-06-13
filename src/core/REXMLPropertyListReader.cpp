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
#include "../../include/recore/REObjectsDictionary.h"

#if defined(__RE_USING_ADITIONAL_TINYXML_LIBRARY__)
#include "../addlibs/tinyxml.h"

using namespace tinyxml2;

#elif defined(__RE_USING_SYSTEM_TINYXML_LIBRARY__)
#include <tinyxml.h>
#else
#define __RE_NO_XML_PARSER_PRIVATE__
#endif


void REXMLPropertyListReader::ClearPairs(REArray<REObjectsDictionary::KeyObjectStruct> * pairs)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	for (REUInt32 i = 0; i < pairs->Count(); i++) 
	{
		REObjectsDictionary::KeyObjectStruct * p = &((*pairs)[i]);
		if (p->keyValue) 
		{
			p->keyValue->Release();
		}
		if (p->objValue) 
		{
			p->objValue->Release();
		}
	}
#endif
}

REObject * REXMLPropertyListReader::NewObjectFromElement(void * elementObject)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
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
						RENumberObject * numberValue = RENumberObject::CreateWithStringPresentation(numberString);
						if (numberValue) 
						{
							return numberValue;
						}
					}
				}
				else if (strncmp(eVal, "data", 4) == 0)
				{
					REString base64String(elem->GetText());
					if (!base64String.IsEmpty()) 
					{
						REBufferObject * bufferValue = REBufferObject::Create();
						if (bufferValue) 
						{
							REBase64 b;
							if ( b.Base64StringToBuffer(base64String, bufferValue) )
							{
								return bufferValue;
							}
							else
							{
								bufferValue->Release();
							}
						}
					}
				}
				else if (strncmp(eVal, "dict", 4) == 0)
				{
					REObjectsDictionary * dictValue = REObjectsDictionary::Create();
					if (dictValue) 
					{
						REArray<REObjectsDictionary::KeyObjectStruct> * newPairs = dictValue->GetPairs();
						if (newPairs) 
						{
							if ( REXMLPropertyListReader::ParseDictionaryElement(newPairs, elem) ) 
							{
								return dictValue;
							}
							else
							{
								dictValue->Release();
							}
						}
						else
						{
							dictValue->Release();
						}
					}
				}
				else if (strncmp(eVal, "true", 4) == 0)
				{
					RENumberObject * numberValue = RENumberObject::CreateWithBool((REBOOL)true);
					if (numberValue) 
					{
						return numberValue;
					}
				}
				break;
				
				
			case 5:
				if (strncmp(eVal, "array", 5) == 0)
				{
					REObjectsArray * arr = REObjectsArray::Create();
					if (arr) 
					{
						for(XMLElement * arrElem = elem->FirstChildElement(); arrElem != NULL; arrElem = arrElem->NextSiblingElement() )
						{
							REObject * arrObjValue = REXMLPropertyListReader::NewObjectFromElement(arrElem);
							if (arrObjValue) 
							{
								arr->Add(arrObjValue);
							}
						}
					}
					return arr;
				}
				else if (strncmp(eVal, "false", 5) == 0)
				{
					RENumberObject * numberValue = RENumberObject::CreateWithBool((REBOOL)false);
					if (numberValue) 
					{
						return numberValue;
					}
				}
				break;
				
			case 6:
				if (strncmp(eVal, "string", 6) == 0)
				{
					REStringObject * stringValue = REStringObject::CreateWithChars(elem->GetText());
					return stringValue;
				}
				break;
				
				
			case 7:
				if (strncmp(eVal, "integer", 7) == 0)
				{
					const char * numberString = elem->GetText();
					if (numberString) 
					{
						RENumberObject * numberValue = RENumberObject::CreateWithStringPresentation(numberString);
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
#endif
	return NULL;
}

REBOOL REXMLPropertyListReader::ParseDictionaryElement(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, void * dictionaryElement)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__	
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
					keyString = REStringObject::CreateWithChars(elem->GetText());
					if (keyString) 
					{
						if (keyString->IsEmpty()) 
						{
							keyString->Release();
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
					REObjectsDictionary::KeyObjectStruct pair;
					pair.keyValue = keyString;
					pair.objValue = objValue;
					pairs->Add(pair);
					keyString = NULL;
				}
			}
			
		}
	}
	
	if (keyString) 
	{
		keyString->Release();
	}
#endif
	return true;
}

REBOOL REXMLPropertyListReader::ReadPropertyListElement(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, void * propListElement)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__	
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
#endif
	return false;
}

REBOOL REXMLPropertyListReader::ReadFromString(const char * listString, REArray<REObjectsDictionary::KeyObjectStruct> * pairs)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__	
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
#endif
	return false;
}

REBOOL REXMLPropertyListReader::ReadFromString(const REString & listString, REArray<REObjectsDictionary::KeyObjectStruct> * pairs)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	return this->ReadFromString(listString.UTF8String(), pairs);
#else
	return false;
#endif
}

REXMLPropertyListReader::REXMLPropertyListReader()
{
	
}

REXMLPropertyListReader::~REXMLPropertyListReader()
{
	
}



