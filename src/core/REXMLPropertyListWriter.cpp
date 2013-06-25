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


#include "../../include/recore/REXMLPropertyListWriter.h"
#include "../../include/recore/REBase64.h"

REBOOL REXMLPropertyListWriter::writeNumber(const REString & prefixString, RENumberObject * number)
{
	if (number->getType() == RENumberTypeBool) 
	{
		if (number->getBoolValue()) { _xmlStr->appendFormat("\n%s<true/>", prefixString.getChars()); }
		else { _xmlStr->appendFormat("\n%s<false/>", prefixString.getChars()); }
		return true;
	}
	else if (number->isInteger()) 
	{
		_xmlStr->appendFormat("\n%s<integer>%s</integer>", prefixString.getChars(), number->getStringValue().getChars());
		return true;
	}
	else if (number->isReal())
	{
		_xmlStr->appendFormat("\n%s<real>%s</real>", prefixString.getChars(), number->getStringValue().getChars());
		return true;
	}
	
	return false;
}

REBOOL REXMLPropertyListWriter::writeDictionary(const REString & prefixString, REObjectsDictionary * dict)
{
	_xmlStr->appendFormat("\n%s<dict>", prefixString.getChars());
	
	REArray<REObjectsDictionary::KeyObjectStruct> * pairs = dict->getPairs();
	if (pairs) 
	{
		REMutableString newPrefix(prefixString);
		newPrefix.append("\t");
		
		for (REUInt32 i = 0; i < pairs->count(); i++) 
		{
			REObjectsDictionary::KeyObjectStruct p = (*pairs)[i];
			if (p.keyValue && p.objValue) 
			{
				if ( !this->writePair(newPrefix, &p) )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	_xmlStr->appendFormat("\n%s</dict>", prefixString.getChars());
	
	return true;
}

REBOOL REXMLPropertyListWriter::writeArray(const REString & prefixString, REObjectsArray * arrObj)
{
	_xmlStr->appendFormat("\n%s<array>", prefixString.getChars());
	
	REMutableString newPrefix(prefixString);
	newPrefix.append("\t");
	
	for (REUInt32 i = 0; i < arrObj->count(); i++) 
	{
		this->writeObject(newPrefix, (*arrObj)[i]);
	}
	
	_xmlStr->appendFormat("\n%s</array>", prefixString.getChars());
	return true;
}

REBOOL REXMLPropertyListWriter::writeString(const REString & prefixString, REStringObject * strObj)
{
	_xmlStr->appendFormat("\n%s<string>", prefixString.getChars());
	
	REMutableString clearString(*strObj);
	clearString.replace("&", "&amp;");
	clearString.replace("<", "&lt;");
	clearString.replace(">", "&gt;");
	clearString.replace("\"", "&quot;");
	clearString.replace("'", "&apos;");
	
	_xmlStr->append(clearString.getChars());
	_xmlStr->append("</string>");
	return true;
}

REBOOL REXMLPropertyListWriter::writeBuffer(const REString & prefixString, REBufferObject * buff)
{
	_xmlStr->appendFormat("\n%s<data>", prefixString.getChars());
	
	REBase64 b;
	REString s;
	b.bufferToBase64String(*buff, &s);
	_xmlStr->append(s.getChars());
	_xmlStr->append("</data>");
	
	return true;
}

REBOOL REXMLPropertyListWriter::writeObject(const REString & prefixString, REObject * obj)
{
	const REUInt32 objClassIdentifier = obj->getClassIdentifier();
	if (objClassIdentifier == REStringObject::classIdentifier()) 
	{
		return this->writeString(prefixString, (REStringObject *)obj);
	}
	else if (objClassIdentifier == REObjectsArray::classIdentifier())
	{
		return this->writeArray(prefixString, (REObjectsArray *)obj);
	}
	else if (objClassIdentifier == REObjectsDictionary::classIdentifier())
	{
		return this->writeDictionary(prefixString, (REObjectsDictionary *)obj);
	}
	else if (objClassIdentifier == RENumberObject::classIdentifier())
	{
		return this->writeNumber(prefixString, (RENumberObject *)obj);
	}
	else if (objClassIdentifier == REBufferObject::classIdentifier())
	{
		return this->writeBuffer(prefixString, (REBufferObject *)obj);
	}
	return false;
}

REBOOL REXMLPropertyListWriter::writePair(const REString & prefixString, REObjectsDictionary::KeyObjectStruct * pair)
{
	if (pair->keyValue->getClassIdentifier() != REStringObject::classIdentifier())
	{
		_errorString = "Dictionary key object must be a string";
		return false;
	}
	
	REStringObject * keyString = (REStringObject*)pair->keyValue;
	_xmlStr->appendFormat("\n%s<key>%s</key>", prefixString.getChars(), keyString->getChars());
	
	return this->writeObject(prefixString, pair->objValue);
}

REBOOL REXMLPropertyListWriter::writeToString(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, REMutableString * listString)
{
	_errorString.clear();
	
	if (pairs && listString) 
	{
		_xmlStr = listString;
		*_xmlStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n<plist version=\"1.0\">\n<dict>";
		
		REString prefixString("\t");
		
		for (REUInt32 i = 0; i < pairs->count(); i++) 
		{
			REObjectsDictionary::KeyObjectStruct p = (*pairs)[i];
			if (p.keyValue && p.objValue) 
			{
				if ( !this->writePair(prefixString, &p) )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		
		_xmlStr->append("\n</dict>\n</plist>\n");
		
		return true;
	}
	return false;
}

REXMLPropertyListWriter::REXMLPropertyListWriter() :
_xmlStr(NULL)
{
	
}

REXMLPropertyListWriter::~REXMLPropertyListWriter()
{
	
}


