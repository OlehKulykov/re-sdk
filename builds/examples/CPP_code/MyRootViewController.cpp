

#include "MyRootViewController.h"
#include <recore.h>

void MyRootViewController::OnTestButtonDownAnimationStart(REObject * animationInfoObject)
{
	printf("\n OnTestButtonDownAnimationStart");
	REAnimationInfo * animationInfo = REObject::GetCasted<REAnimationInfo>(animationInfoObject);
	if (animationInfo) 
	{
		// Can read animation information
		if (animationInfo->GetCustomData()) 
		{
			RENumberObject * number = (RENumberObject*)animationInfo->GetCustomData();
			number->Release();
		}
	}
}

void MyRootViewController::OnTestButtonDownAnimationEnd(REObject * animationInfoObject)
{
	printf("\n OnTestButtonDownAnimationEnd");
	REAnimationInfo * animationInfo = REObject::GetCasted<REAnimationInfo>(animationInfoObject);
	if (animationInfo) 
	{
		// Can read animation information
	}
}

void MyRootViewController::OnTestButtonDown(REObject * buttonObject)
{
	REButton * button = REObject::GetCasted<REButton>(buttonObject);
	if (button) 
	{
		if (button->IsAnimating()) 
		{
			button->StopAnimation(REAnimationStopTypeImmediatelyToStart, true, false);
		}
		
		RENumberObject * anyDataOrNULL = RENumberObject::CreateWithStringPresentation("-312.14f");
		const REUIdentifier animationIdentifier = REAnimation::Setup(anyDataOrNULL);
		if (animationIdentifier)
		{
			RERect frame = button->GetFrame();
			REColor color = button->GetColor();
			
			frame.y += 0.5f;
			frame.x += 0.5f;
			color.alpha = 0.3f;
			color.red = 0.0f;
			
			REAnimation::SetTime(1.5);
			REAnimation::SetStartDelay(0.1);
			REAnimation::SetType(REAnimationTypeBounceEaseOut);
			REAnimation::SetLoopType(REAnimationLoopTypeFromEndToStart, 1);
			REAnimation::SetDidStartMethod(NEW_CLASS_METHOD(MyRootViewController, this, OnTestButtonDownAnimationStart));
			REAnimation::SetDidStopMethod(NEW_CLASS_METHOD(MyRootViewController, this, OnTestButtonDownAnimationEnd));
			
			button->SetFrameAnimated(frame);
			button->SetColorAnimated(color);
			
			REAnimation::Execute();
		}
	}
}

MyRootViewController::MyRootViewController() : REViewController()
{
	
}

MyRootViewController::~MyRootViewController()
{
	
}



