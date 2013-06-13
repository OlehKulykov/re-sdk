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


#include "../../include/regui/REViewController.h"
#include "../../include/regui/private/REXMLSerializableReader.h"
#include "../../include/regui/REFramedTextureObject.h"
#include "../../include/regui/RETextureObject.h"
#include "../../include/regui/REParticleView.h"
#include "../../include/recore/REThread.h"


#define RE_VIEW_CONTROLLER_XML_SIZE_KEY_STRING "sizef"
#define RE_VIEW_CONTROLLER_XML_SIZE_FORMAT_STRING "%f;%f"


class REViewControllerCreateTextureMainThreadTaskPrivateBase : public REObject
{	
public:
	virtual void Create() = 0;
	REViewControllerCreateTextureMainThreadTaskPrivateBase() : REObject() { }
	virtual ~REViewControllerCreateTextureMainThreadTaskPrivateBase() { }
};

class REViewControllerCreateTextureMainThreadTaskPrivate : public REViewControllerCreateTextureMainThreadTaskPrivateBase
{
private:
	RETextureObject * _newTexture;
public:
	RETextureObject * GetNewTexture() { return _newTexture; }
	virtual void Create() 
	{
		_newTexture = RETextureObject::Create();
	}
	REViewControllerCreateTextureMainThreadTaskPrivate() : 
		REViewControllerCreateTextureMainThreadTaskPrivateBase(), _newTexture(NULL) { }
	virtual ~REViewControllerCreateTextureMainThreadTaskPrivate() { }
};

class REViewControllerCreateFramedTextureMainThreadTaskPrivate : public REViewControllerCreateTextureMainThreadTaskPrivateBase
{
private:
	REFramedTextureObject * _newTexture;
public:
	REFramedTextureObject * GetNewTexture() { return _newTexture; }
	virtual void Create() 
	{
		_newTexture = REFramedTextureObject::Create();
	}
	REViewControllerCreateFramedTextureMainThreadTaskPrivate() : 
		REViewControllerCreateTextureMainThreadTaskPrivateBase(), _newTexture(NULL) { }
	virtual ~REViewControllerCreateFramedTextureMainThreadTaskPrivate() { }
};

void REViewController::CreateNewTextureMainThreadMethod(REObject * mainThreadTask)
{
	REViewControllerCreateTextureMainThreadTaskPrivateBase * task = (REViewControllerCreateTextureMainThreadTaskPrivateBase*)mainThreadTask;
	if (task) 
	{
		task->Create();
	}
}

REGUIObject * REViewController::CreateXMLSerializableObjectForController(REViewController * controller, 
																		 const char * className, 
																		 const char * key)
{
	if (controller) 
	{
		return controller->CreateXMLSerializableObject(className, key);
	}
	return NULL;
}

REGUIObject * REViewController::NewSerializableObject(const char * className, const char * key)
{
    //TODO: create from class name
    if (strcmp(className, "REView") == 0)
    {
        REGUIObject * newObject = REView::Create();
        return newObject;
    }
    else if (strcmp(className, "REButton") == 0)
    {
        REGUIObject * newObject = REButton::Create();
        return newObject;
    }
    else if (strcmp(className, "RELabel") == 0)
    {
        REGUIObject * newObject = RELabel::Create();
        return newObject;
    }
    else if (strcmp(className, "REFontObject") == 0)
    {
        REGUIObject * newObject = REFontObject::Create();
        return newObject;
    }
    else if (strcmp(className, "RETransformedView") == 0)
    {
        RETransformedView * v = RETransformedView::Create();
        return v;
    }
    else if (strcmp(className, "RETextField") == 0)
    {
        REGUIObject * newObject = RETextField::Create();
        return newObject;
    }
	else if (strcmp(className, "REParticleView") == 0)
    {
        REGUIObject * newObject = REParticleView::Create();
        return newObject;
    }
	
    return NULL;
}

REGUIObject * REViewController::CreateXMLSerializableObject(const char * className, const char * key) const
{
	if (className) 
	{
        REGUIObject * newObject = REViewController::NewSerializableObject(className, key);
        if (newObject)
        {
            return newObject;
        }

        if (strcmp(className, "RETextureObject") == 0)
		{
			if (REThread::IsMainThread()) 
			{
				RETextureObject * t = RETextureObject::Create();
				return t;
			}
			else
			{
				REViewControllerCreateTextureMainThreadTaskPrivate * task = new REViewControllerCreateTextureMainThreadTaskPrivate();
				if (task) 
				{
					REThread::PerformMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, ((REViewController*)this), CreateNewTextureMainThreadMethod), task);
					RETextureObject * t = task->GetNewTexture();
					task->Release();
					return t;
				}
			}
		}
		else if (strcmp(className, "REFramedTextureObject") == 0)
		{
			if (REThread::IsMainThread()) 
			{
				REFramedTextureObject * t = REFramedTextureObject::Create();
				return t;
			}
			else
			{
				REViewControllerCreateFramedTextureMainThreadTaskPrivate * task = new REViewControllerCreateFramedTextureMainThreadTaskPrivate();
				if (task) 
				{
					REThread::PerformMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, ((REViewController*)this), CreateNewTextureMainThreadMethod), task);
					REFramedTextureObject * t = task->GetNewTexture();
					task->Release();
					return t;
				}
			}
		}
	}
	return NULL;
}

/* IREXMLSerializable */
REBOOL REViewController::AcceptStringParameter(const char * key, const char * value)
{
	if (strcmp(key, RE_VIEW_CONTROLLER_XML_SIZE_KEY_STRING) == 0)
	{
		if (value) 
		{
			RESize s;
			if (sscanf(value, RE_VIEW_CONTROLLER_XML_SIZE_FORMAT_STRING, &s.width, &s.height) == 2) 
			{
				_frame.SetSize(s);
				return true;
			}
		}
	}
	
	return false;
}

void REViewController::CallBackgroundLoadingDidStartMainThreadMethod(REObject * sender)
{
	this->OnBackgroundLoadingDidStart();
}

void REViewController::CallBackgroundLoadingDidEndMainThreadMethod(REObject * sender)
{
	this->OnBackgroundLoadingDidEnd();
}

void REViewController::LoadVCFromXMLString(REViewController * vc, const REString & xmlString, REBOOL isLoadInBackground)
{
	if (isLoadInBackground)
	{
		if (REThread::IsMainThread())
		{
			vc->OnBackgroundLoadingDidStart();
		}
		else
		{
			REThread::PerformMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, vc, CallBackgroundLoadingDidStartMainThreadMethod), NULL);
		}
	}
	
	REXMLSerializableReader xmlReader(REViewController::CreateXMLSerializableObjectForController, vc);
	vc->_xmlReader = &xmlReader;
	
	xmlReader.Read(xmlString);
	
	vc->_xmlReader = NULL;
	
	if (isLoadInBackground)
	{
		if (REThread::IsMainThread())
		{
			vc->OnBackgroundLoadingDidEnd();
		}
		else
		{
			REThread::PerformMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, vc, CallBackgroundLoadingDidEndMainThreadMethod), NULL);
		}
	}
	
	vc->OnLoadingDidEnd();
}


void REViewController::LoadByNameThreadMethod(REObject * dataFilePathStringObject)
{
	REStringObject * strObj = (REStringObject*)dataFilePathStringObject;
	REData xmlData;
	if (xmlData.InitFromPath(*strObj)) 
	{
		REString xmlString((const char*)xmlData.GetBytes());
		REViewController::LoadVCFromXMLString(this, xmlString, true);
	}
}

REBOOL REViewController::LoadByName(const REString & name, REBOOL isLoadInBackground)
{	
	REBOOL loadResult = false;
	REStringObject * strObj = REStringObject::CreateWithChars("data/vc/");
	if (strObj) 
	{
		strObj->AppendFormat("%s/vc.xml", name.UTF8String());
		if (isLoadInBackground) 
		{
			REThread::DetachNewThreadWithMethod(NEW_CLASS_METHOD(REViewController, this, LoadByNameThreadMethod), strObj);
			loadResult = true;
		}
		else
		{
			REData xmlData;
			if (xmlData.InitFromPath(*strObj)) 
			{
				REString xmlString((const char*)xmlData.GetBytes());
				REViewController::LoadVCFromXMLString(this, xmlString, false);
				loadResult = true;
			}
		}
		strObj->Release();
	}	
	return loadResult;
}

const REFloat32 REViewController::GetBackgroundLoadingProgress() const
{
	if (_xmlReader) 
	{
		REXMLSerializableReader * reader = (REXMLSerializableReader*)_xmlReader;
		return reader->GetProgress();
	}
	return 1.0f;
}

REBOOL REViewController::IsLoading() const
{
	return ((_xmlReader) ? true : false);
}
/*
const RERect & REViewController::GetFrame() const
{
	REObjectsArray * arr = const_cast<REObjectsArray *>(this->GetSubViewsArray());
	if (arr) 
	{
		RERect f(0.0f, 0.0f, 0.0f, 0.0f);
		for (REUInt32 i = 0; i < arr->Count(); i++) 
		{
			f = f.GetUnion(((REView*)((*arr)[i]))->GetFrame());
		}
		RERect * frame = const_cast<RERect *>(&_frame);
		(*frame) = f;
	}
	
	return _frame;
}

RERect REViewController::GetScreenFrame() const
{
	return this->GetFrame();
}
*/
void REViewController::Render()
{
	REObjectsArray * arr = this->GetSubViewsArray();
	if (arr) 
	{
		for (REUInt32 i = 0; i < arr->Count(); i++) 
		{
			((REView*)(*arr)[i])->Render();
		}
	}
}

void REViewController::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	REObjectsArray * arr = this->GetSubViewsArray();
	if (arr) 
	{
		for (REUInt32 i = 0; i < arr->Count(); i++) 
		{
			((REView*)(*arr)[i])->RenderWithOffset(offsetX, offsetY);
		}
	}
}

const REUInt32 REViewController::GetClassIdentifier() const
{
	return REViewController::ClassIdentifier();
}

const REUInt32 REViewController::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REViewController");
	return clasIdentif;
}

REBOOL REViewController::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REViewController::ClassIdentifier() == classIdentifier) ||
			REView::IsImplementsClass(classIdentifier));
}

const REObjectsArray * REViewController::GetViews() const
{
	return this->GetSubViewsArray();
}

REViewController::REViewController() : REView(),
    _xmlReader(NULL)
{
	this->SetRespondsForUserAction(true);
    this->SetInterceptsUserAction(true);
}

REViewController::~REViewController()
{
	
}

REViewController * REViewController::Create()
{
	REViewController * newViewController = new REViewController();
	return newViewController;
}

const char * REViewController::GetXMLSizeKeyString()
{
	return RE_VIEW_CONTROLLER_XML_SIZE_KEY_STRING;
}

const char * REViewController::GetXMLSizeFormatString()
{
	return RE_VIEW_CONTROLLER_XML_SIZE_FORMAT_STRING;
}

