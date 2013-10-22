

#ifndef __MYGUIAPPLICATION_H__
#define __MYGUIAPPLICATION_H__

#include <regui.h>

class MyGUIApplication : public REGUIApplication
{
protected:
	REBOOL _isStarted;
	
	void CreateUIInCode();
	
	void LoadViewControllerFromXML();
	
	void LoadViewControllerFromXMLWithLoading();
public:
	REBOOL IsStarted() const { return _isStarted; };
	void Start();
	virtual REString GetName() const;
	
	MyGUIApplication(REViewController * rootViewController, void * hWND = NULL);
	virtual ~MyGUIApplication();
};


#endif /* __MYGUIAPPLICATION_H__ */
