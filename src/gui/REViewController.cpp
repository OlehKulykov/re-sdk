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
#include "../../include/regui/RETexture.h"
#include "../../include/regui/REParticleView.h"
#include "../../include/recore/REThread.h"
#include "../../include/recore/REMutableString.h"

#if 0

#define RE_VIEW_CONTROLLER_XML_SIZE_KEY_STRING "sizef"


class REViewControllerCreateTextureMainThreadTaskPrivateBase : public REObject
{	
public:
	virtual void Create() = 0;
	REViewControllerCreateTextureMainThreadTaskPrivateBase() : REObject() { }
	virtual ~REViewControllerCreateTextureMainThreadTaskPrivateBase() { }
};

#if 0
class REViewControllerCreateTextureMainThreadTaskPrivate : public REViewControllerCreateTextureMainThreadTaskPrivateBase
{
private:
	RETexture * _newTexture;
public:
	RETexture * GetNewTexture() { return _newTexture; }
	virtual void Create() 
	{
		_newTexture = RETexture::create();
	}
	REViewControllerCreateTextureMainThreadTaskPrivate() : 
		REViewControllerCreateTextureMainThreadTaskPrivateBase(), _newTexture(NULL) { }
	virtual ~REViewControllerCreateTextureMainThreadTaskPrivate() { }
};
#endif

#if 0
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
#endif

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
#if 0			
			if (REThread::isMainThread()) 
			{
				RETexture * t = RETexture::Create();
				return t;
			}
			else
			{
				REViewControllerCreateTextureMainThreadTaskPrivate * task = new REViewControllerCreateTextureMainThreadTaskPrivate();
				if (task) 
				{
					REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, ((REViewController*)this), CreateNewTextureMainThreadMethod), task);
					RETextureObject * t = task->GetNewTexture();
					task->release();
					return t;
				}
			}
#endif			
		}
		else if (strcmp(className, "REFramedTextureObject") == 0)
		{
#if 0			
			if (REThread::isMainThread()) 
			{
				REFramedTextureObject * t = REFramedTextureObject::Create();
				return t;
			}
			else
			{
				REViewControllerCreateFramedTextureMainThreadTaskPrivate * task = new REViewControllerCreateFramedTextureMainThreadTaskPrivate();
				if (task) 
				{
					REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, ((REViewController*)this), CreateNewTextureMainThreadMethod), task);
					REFramedTextureObject * t = task->GetNewTexture();
					task->release();
					return t;
				}
			}
#endif			
		}
	}
	return NULL;
}

/* IREXMLSerializable */
REBOOL REViewController::acceptStringParameter(const char * key, const char * value)
{
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
		if (REThread::isMainThread())
		{
			vc->OnBackgroundLoadingDidStart();
		}
		else
		{
			REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, vc, CallBackgroundLoadingDidStartMainThreadMethod), NULL);
		}
	}
	
	REXMLSerializableReader xmlReader(REViewController::CreateXMLSerializableObjectForController, vc);
	vc->_xmlReader = &xmlReader;
	
	xmlReader.Read(xmlString);
	
	vc->_xmlReader = NULL;
	
	if (isLoadInBackground)
	{
		if (REThread::isMainThread())
		{
			vc->OnBackgroundLoadingDidEnd();
		}
		else
		{
			REThread::performMethodOnMainThreadAndWaitUntilDone(NEW_CLASS_METHOD(REViewController, vc, CallBackgroundLoadingDidEndMainThreadMethod), NULL);
		}
	}
	
	vc->OnLoadingDidEnd();
}


void REViewController::LoadByNameThreadMethod(REObject * dataFilePathStringObject)
{
	REStringObject * strObj = (REStringObject*)dataFilePathStringObject;
	REData xmlData;
	if (xmlData.initFromPath(*strObj)) 
	{
		REString xmlString((const char*)xmlData.bytes());
		REViewController::LoadVCFromXMLString(this, xmlString, true);
	}
}

REBOOL REViewController::loadByName(const REString & name, REBOOL isLoadInBackground)
{	
	REString path = REString::createWithFormat("data/vc/%s/vc.xml", name.UTF8String());
	REBOOL loadResult = false;
	if (path.length() > 0) 
	{
		if (isLoadInBackground) 
		{
			REStringObject * strObj = REStringObject::createWithString(path);
			REThread::detachNewThreadWithMethod(NEW_CLASS_METHOD(REViewController, this, LoadByNameThreadMethod), strObj);
			loadResult = true;
			strObj->release();
		}
		else
		{
			REData xmlData;
			if (xmlData.initFromPath(path)) 
			{
				REString xmlString((const char *)xmlData.bytes(), xmlData.size());
				xmlData.clear();
				REViewController::LoadVCFromXMLString(this, xmlString, false);
				loadResult = true;
			}
		}
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
	REArrayObject * arr = const_cast<REArrayObject *>(this->GetSubViewsArray());
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
void REViewController::render()
{
	REList<REView *>::Iterator iter(this->subviewsIterator());
	while (iter.next()) 
	{
		iter.value()->render();
	}
}

void REViewController::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	REList<REView *>::Iterator iter(this->subviewsIterator());
	while (iter.next()) 
	{
		iter.value()->renderWithOffset(offsetX, offsetY);
	}
}

REViewController::REViewController() : REView(),
    _xmlReader(NULL)
{
	this->setRespondsForUserAction(true);
    this->setInterceptsUserAction(true);
}

REViewController::~REViewController()
{
	
}

REViewController * REViewController::create()
{
	REViewController * newViewController = new REViewController();
	return newViewController;
}

const char * REViewController::getXMLSizeKeyString()
{
	return RE_VIEW_CONTROLLER_XML_SIZE_KEY_STRING;
}

#endif

