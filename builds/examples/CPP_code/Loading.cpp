

#include "Loading.h"

void Loading::OnViewDidAdded(REView * view)
{
	if (view->GetTag() == 1) 
	{
		_progressView = view;
		_progressViewSourceFrame = view->GetFrame();
	}
}

void Loading::SetBackgroundLoadingProgress(const REFloat32 progress)
{
	if (_progressView) 
	{
		RETextureObject * texture = _progressView->GetTexture();
		if (texture) 
		{
			RETetragon textureFrame(texture->GetFrame());
			textureFrame.topRightX = textureFrame.bottomRightX = progress;
			texture->SetFrame(textureFrame);
		}
		
		RERect progressFrame(_progressViewSourceFrame);
		progressFrame.width *= progress;
		_progressView->SetFrame(progressFrame);
	}
}

Loading::Loading() : REViewController(),
	_progressView(NULL)
{
	
}

Loading::~Loading()
{
	
}



