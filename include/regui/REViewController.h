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


#ifndef __REVIEWCONTROLLER_H__
#define __REVIEWCONTROLLER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"
#include "../recore/REArrayObject.h"
#include "RESubViewsContainer.h"
#include "IRERenderable.h"
#include "REView.h"
#include "RELabel.h"
#include "REFontObject.h"
#include "REButton.h"
#include "RETransformedView.h"
#include "RETextField.h"

/// Class of view controller.
class __RE_PUBLIC_CLASS_API__ REViewController : public REView
{
private:
	static REGUIObject * CreateXMLSerializableObjectForController(REViewController * controller, 
																  const char * className, 
																  const char * key);
	void CallBackgroundLoadingDidStartMainThreadMethod(REObject * sender);
	void CallBackgroundLoadingDidEndMainThreadMethod(REObject * sender);
	void LoadByNameThreadMethod(REObject * dataFilePathStringObject);
	static void LoadVCFromXMLString(REViewController * vc, const REString & xmlString, REBOOL isLoadInBackground);
	void CreateNewTextureMainThreadMethod(REObject * mainThreadTask);
	void * _xmlReader;
    static REGUIObject * NewSerializableObject(const char * className, const char * key);
protected:
	/// Creates and return object that inherits 'REGUIObject' class.
	/// This new class object is equal to 'className' for 'key' needs.
	virtual REGUIObject * CreateXMLSerializableObject(const char * className, const char * key) const;
	
	/// Called by controller when start it's loading in background. 
	/// Called from main thread.
	virtual void OnBackgroundLoadingDidStart() { }
	
	/// Called by controller when background loading is finished.
	/// Called from main thread.
	virtual void OnBackgroundLoadingDidEnd() { }
	
	/// Called by controller when loading done.
	virtual void OnLoadingDidEnd() { }
	
	REViewController();
	virtual ~REViewController();
public:
	/* REGUIObject */
	virtual REBOOL acceptStringParameter(const char * key, const char * value);

	/// Loads view controller using it's name
	/// Loading can process in current or aditional thread.
    virtual REBOOL loadByName(const REString & name, REBOOL isLoadInBackground);
	
	/// Returns loading process.
	const REFloat32 GetBackgroundLoadingProgress() const;
	
	/// Check is view controller loading.
	REBOOL IsLoading() const;
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	

	/// Creates and returns new view controller object.
	static REViewController * create();
	
	/// Returns XML key string for size.
	static const char * getXMLSizeKeyString();
};


#endif /* __REVIEWCONTROLLER_H__ */


