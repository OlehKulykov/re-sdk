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


#include "../../include/regui/private/REXMLSerializableReader.h"

#if defined(__RE_USING_ADITIONAL_TINYXML_LIBRARY__)
#include "../addlibs/tinyxml.h"
#elif defined(__RE_USING_SYSTEM_TINYXML_LIBRARY__)
#include <tinyxml.h>
#else
#define __RE_NO_XML_PARSER_PRIVATE__
#endif


REBOOL REXMLSerializableReader::ParseObject(void * objectElement, REGUIObject * obj, REGUIObject * fromCallBack)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	TiXmlElement * element = (TiXmlElement *)objectElement;

	for (TiXmlElement * childElem = element->FirstChildElement(); childElem != NULL; childElem = childElem->NextSiblingElement()) 
	{
		const char * nodeValue = childElem->Value();
		if (nodeValue) 
		{
			if (strcmp(nodeValue, "object") == 0) 
			{
				const char * className = NULL;
				const char * key = NULL;
				for (TiXmlAttribute * attrib = childElem->FirstAttribute(); attrib != NULL; attrib = attrib->Next()) 
				{
					const char * name = attrib->Name();
					if (name) 
					{
						if (strcmp(name, "class") == 0) 
						{
							className = attrib->Value();
						}
						else if (strcmp(name, "key") == 0)
						{
							key = attrib->Value();
						}
					}
					_processedElementsCount++;
				}
				REGUIObject * newObject = _callBack.CreateNewObject(_controller, className, key);
				if (newObject) 
				{
					REBOOL isAccepted = false;
					//REGUIObject * xmlSerializable = newObject->GetCasted<IREXMLSerializable>();
					//if (xmlSerializable) 
					//{
					newObject->OnPrepareGUIObjectForSetuping();
					this->ParseObject(childElem, newObject, newObject);
					isAccepted = obj->AcceptObjectParameter(className, key, newObject);
					newObject->OnSetupingGUIObjectFinished(isAccepted);
					//}
					if (!isAccepted)
					{
						newObject->Release();
					}
				}
				else 
				{
					return false;
				}
			}
			else
			{
				const char * key = NULL;
				for (TiXmlAttribute * attrib = childElem->FirstAttribute(); attrib != NULL; attrib = attrib->Next()) 
				{
					const char * name = attrib->Name();
					if (name) 
					{ 
						if (strcmp(name, "key") == 0) 
						{
							key = attrib->Value();
						}
					}
					_processedElementsCount++;
				}
				
				if (key) 
				{
					const char * value = childElem->GetText();
					if (value) 
					{
						if (strcmp(nodeValue, "property") == 0) 
						{
							IREObjectProperty * prop = obj->GetPropertyForKey(key);
							if (prop) 
							{
								int propIdentif = 0;
								if (sscanf(value, "%i", &propIdentif) == 1) 
								{
									REXMLSerializableReader::PropertyStruct newStruct;
									newStruct.property = prop;
									newStruct.editorid = (REInt32)propIdentif;
									_properties.Add(newStruct);
								}
							}
						}
						else if (strcmp(key, "editorid") == 0)
						{
							int propIdentif = 0;
							if (sscanf(value, "%i", &propIdentif) == 1) 
							{
								for (REUInt32 i = 0; i < _properties.Count(); i++) 
								{
									if (_properties[i].editorid == (REInt32)propIdentif)
									{
										IREObjectProperty * prop = _properties[i].property;
										prop->SetObject(fromCallBack);
										_properties.RemoveAt(i);
										break;
									}
								}
							}
						}
						else
						{
							obj->AcceptStringParameter(key, value);
						}
					}
				}
			}
			_processedElementsCount++;
		}
	}
#endif
	return true;
}

REUInt32 REXMLSerializableReader::CalculateElements(void * objectElement)
{
	REUInt32 count = 0;
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	TiXmlElement * element = (TiXmlElement *)objectElement;
	for (TiXmlElement * childElem = element->FirstChildElement(); childElem != NULL; childElem = childElem->NextSiblingElement()) 
	{
		const char * nodeValue = childElem->Value();
		if (nodeValue) 
		{
			for (TiXmlAttribute * attrib = childElem->FirstAttribute(); attrib != NULL; attrib = attrib->Next()) 
			{
				count++;
			}
			if (strcmp(nodeValue, "object") == 0) 
			{
				count += this->CalculateElements(childElem);
			}
			count++;
		}
	}
#endif
	return count;
}

const REFloat32 REXMLSerializableReader::GetProgress() const 
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	if (_totalElementsCount) 
	{
		return ((REFloat32)_processedElementsCount / (REFloat32)_totalElementsCount);
	}
#endif
	return 0.0f;
}

REBOOL REXMLSerializableReader::Read(const REString & xmlString)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__	
	_totalElementsCount = 0;
	_processedElementsCount = 0;
	_isError = false;
	if (_controller && _callBack.CreateNewObject) 
	{
		TiXmlDocument doc;
		doc.Parse(xmlString.UTF8String());
		
		if (doc.Error()) { return false; }
		
		TiXmlElement * root = doc.RootElement();
		if (root == NULL) { return false; }
		
		const char * rootVal = root->Value();
		if (rootVal) 
		{
			if (strcmp(rootVal, "viewcontroller") == 0) 
			{
				_totalElementsCount = this->CalculateElements(root);
				if ( this->ParseObject(root, _controller, _controller) ) 
				{
					return true;
				}
				_isError = true;
			}
		}
	}
#endif
	return false;
}

REXMLSerializableReader::REXMLSerializableReader(REGUIObject * (CreateNewObject)(REViewController *, const char *, const char *), REViewController * controller) :
	_controller(NULL),
	_totalElementsCount(0),
	_processedElementsCount(0),
	_isError(false)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	_controller = controller;
	_callBack.CreateNewObject = CreateNewObject;
#endif
}

REXMLSerializableReader::~REXMLSerializableReader()
{
	
}

