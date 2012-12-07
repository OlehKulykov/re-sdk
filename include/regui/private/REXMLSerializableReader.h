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


#ifndef __REXMLSERIALIZABLEREADER_H__
#define __REXMLSERIALIZABLEREADER_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REString.h"
#include "../../recore/REArray.h"
#include "../REGUIObject.h"
#include "../REView.h"
#include "../REViewController.h"
#include "../RETextureObject.h"


class __RE_PUBLIC_CLASS_API__ REXMLSerializableReader
{
private:
	typedef struct _creteCallBack
	{
		REGUIObject * (*CreateNewObject)(REViewController * controller, const char * className, const char * key);
	} CreteCallBack;
	typedef struct _propertyStruct
	{
		IREObjectProperty * property;
		REInt32 editorid;
	} PropertyStruct;
	REViewController * _controller;
	REXMLSerializableReader::CreteCallBack _callBack;
	REArray<REXMLSerializableReader::PropertyStruct> _properties;
	REUInt32 _totalElementsCount;
	REUInt32 _processedElementsCount;
	REBOOL _isError;
	REBOOL ParseObject(void * objectElement, REGUIObject * obj, REGUIObject * fromCallBack);
	REUInt32 CalculateElements(void * objectElement);
public:
	REBOOL IsError() const { return _isError; }
	const REFloat32 GetProgress() const;
	REBOOL Read(const REString & xmlString);
	REXMLSerializableReader(REGUIObject * (CreateNewObject)(REViewController *, const char *, const char *), REViewController * controller);
	~REXMLSerializableReader();
};


#endif /* __REXMLSERIALIZABLEREADER_H__ */


