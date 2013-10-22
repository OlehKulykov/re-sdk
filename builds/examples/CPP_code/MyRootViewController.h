

#ifndef __MYROOTVIEWCONTROLLER_H__
#define __MYROOTVIEWCONTROLLER_H__

#include <regui.h>

class MyRootViewController : public REViewController
{
private:
	void OnTestButtonDownAnimationStart(REObject * animationInfoObject);
	void OnTestButtonDownAnimationEnd(REObject * animationInfoObject);
	
public:
	void OnTestButtonDown(REObject * buttonObject);
	
	RERetainProperty<RELabel *> testLabel;
	
	MyRootViewController();
	virtual ~MyRootViewController();
};


#endif /* __MYROOTVIEWCONTROLLER_H__ */
