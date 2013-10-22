

#ifndef __LOADING_H__
#define __LOADING_H__

#include <regui.h>

class Loading : public REViewController
{
protected:
	REView * _progressView;
	RERect _progressViewSourceFrame;
	
	virtual void OnViewDidAdded(REView * view);
public:
	void SetBackgroundLoadingProgress(const REFloat32 progress);
	
	Loading();
	virtual ~Loading();
};


#endif /* __LOADING_H__ */ 


