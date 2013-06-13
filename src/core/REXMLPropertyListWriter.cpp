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

REBOOL REXMLPropertyListWriter::WriteNumber(const REString & prefixString, RENumberObject * number)
{
	if (number->GetType() == RENumberTypeBool) 
	{
		if (number->GetBoolValue()) { _xmlStr->AppendFormat("\n%s<true/>", prefixString.UTF8String()); }
		else { _xmlStr->AppendFormat("\n%s<false/>", prefixString.UTF8String()); }
		return true;
	}
	else if (number->IsInteger()) 
	{
		_xmlStr->AppendFormat("\n%s<integer>%s</integer>", prefixString.UTF8String(), number->GetStringValue().UTF8String());
		return true;
	}
	else if (number->IsReal())
	{
		_xmlStr->AppendFormat("\n%s<real>%s</real>", prefixString.UTF8String(), number->GetStringValue().UTF8String());
		return true;
	}
	
	return false;
}

REBOOL REXMLPropertyListWriter::WriteDictionary(const REString & prefixString, REObjectsDictionary * dict)
{
	_xmlStr->AppendFormat("\n%s<dict>", prefixString.UTF8String());
	
	REArray<REObjectsDictionary::KeyObjectStruct> * pairs = dict->GetPairs();
	if (pairs) 
	{
		REString newPrefix(prefixString);
		newPrefix.Append("\t");
		
		for (REUInt32 i = 0; i < pairs->Count(); i++) 
		{
			REObjectsDictionary::KeyObjectStruct p = (*pairs)[i];
			if (p.keyValue && p.objValue) 
			{
				if ( !this->WritePair(newPrefix, &p) )
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
	_xmlStr->AppendFormat("\n%s</dict>", prefixString.UTF8String());
	
	return true;
}

REBOOL REXMLPropertyListWriter::WriteArray(const REString & prefixString, REObjectsArray * arrObj)
{
	_xmlStr->AppendFormat("\n%s<array>", prefixString.UTF8String());
	
	REString newPrefix(prefixString);
	newPrefix.Append("\t");
	
	for (REUInt32 i = 0; i < arrObj->Count(); i++) 
	{
		this->WriteObject(newPrefix, (*arrObj)[i]);
	}
	
	_xmlStr->AppendFormat("\n%s</array>", prefixString.UTF8String());
	return true;
}

REBOOL REXMLPropertyListWriter::WriteString(const REString & prefixString, REStringObject * strObj)
{
	_xmlStr->AppendFormat("\n%s<string>", prefixString.UTF8String());
	
	REString clearString(*strObj);
	clearString.Replace("&", "&amp;");
	clearString.Replace("<", "&lt;");
	clearString.Replace(">", "&gt;");
	clearString.Replace("\"", "&quot;");
	clearString.Replace("'", "&apos;");
	
	_xmlStr->Append(clearString.UTF8String());
	_xmlStr->Append("</string>");
	return true;
}

REBOOL REXMLPropertyListWriter::WriteBuffer(const REString & prefixString, REBufferObject * buff)
{
	_xmlStr->AppendFormat("\n%s<data>", prefixString.UTF8String());
	
	REBase64 b;
	REString s;
	b.BufferToBase64String(*buff, &s);
	_xmlStr->Append(s.UTF8String());
	_xmlStr->Append("</data>");
	
	return true;
}

REBOOL REXMLPropertyListWriter::WriteObject(const REString & prefixString, REObject * obj)
{
	const REUInt32 objClassIdentifier = obj->GetClassIdentifier();
	if (objClassIdentifier == REStringObject::ClassIdentifier()) 
	{
		return this->WriteString(prefixString, (REStringObject*)obj);
	}
	else if (objClassIdentifier == REObjectsArray::ClassIdentifier())
	{
		return this->WriteArray(prefixString, (REObjectsArray*)obj);
	}
	else if (objClassIdentifier == REObjectsDictionary::ClassIdentifier())
	{
		return this->WriteDictionary(prefixString, (REObjectsDictionary*)obj);
	}
	else if (objClassIdentifier == RENumberObject::ClassIdentifier())
	{
		return this->WriteNumber(prefixString, (RENumberObject*)obj);
	}
	else if (objClassIdentifier == REBufferObject::ClassIdentifier())
	{
		return this->WriteBuffer(prefixString, (REBufferObject*)obj);
	}
	return false;
}

REBOOL REXMLPropertyListWriter::WritePair(const REString & prefixString, REObjectsDictionary::KeyObjectStruct * pair)
{
	if (pair->keyValue->GetClassIdentifier() != REStringObject::ClassIdentifier())
	{
		_errorString = "Dictionary key object must be a string";
		return false;
	}
	
	REStringObject * keyString = (REStringObject*)pair->keyValue;
	_xmlStr->AppendFormat("\n%s<key>%s</key>", prefixString.UTF8String(), keyString->UTF8String());
	
	return this->WriteObject(prefixString, pair->objValue);
}

REBOOL REXMLPropertyListWriter::WriteToString(REArray<REObjectsDictionary::KeyObjectStruct> * pairs, REString * listString)
{
	_errorString.Clear();
	
	if (pairs && listString) 
	{
		_xmlStr = listString;
		_xmlStr->Set("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n<plist version=\"1.0\">\n<dict>");
		
		REString prefixString("\t");
		
		for (REUInt32 i = 0; i < pairs->Count(); i++) 
		{
			REObjectsDictionary::KeyObjectStruct p = (*pairs)[i];
			if (p.keyValue && p.objValue) 
			{
				if ( !this->WritePair(prefixString, &p) )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		
		_xmlStr->Append("\n</dict>\n</plist>\n");
		
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


