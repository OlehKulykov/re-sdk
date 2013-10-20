

#include "MainMenu.h"
#include "MainMenuExitButton.h"

#define OPTIONS_VC_TAG (-562)
#define HSCOR_VC_TAG (-345)
#define EXIT_VC_TAG (-6373)
#define MAP_VC_TAG (-61041)
#define PROFILE_VC_TAG (-2761)


/// Called by controller when start it's loading in background. 
void MainMenu::OnBackgroundLoadingDidStart()
{
	_loadingIndicator = new Loading();
	if (_loadingIndicator) 
	{
		_loadingIndicator->LoadByName("load", false);
	}
}

/// Called by controller when background loading is finished.
void MainMenu::OnBackgroundLoadingDidEnd()
{
	RE_SAFE_RELEASE(_loadingIndicator);
}

void MainMenu::StartHeadAnimation(REObject * timerObject)
{
	if (birdHead.IsNotNull()) 
	{	
		const REFloat32 arroundX = 2.18455f;
		const REFloat32 arroundY = 2.374f;
		RERandomizer r;
		const REFloat32 angle = -r.GetFloatInRange(5.0f, 12.0f);
		REAffineTransform trans;
		trans.Rotate(DEG_TO_RAD(angle), arroundX, arroundY);
		if (REAnimation::Setup(NULL))
		{
			REAnimation::SetTime((RETimeInterval)r.GetFloatInRange(0.8f, 1.7f));
			REAnimation::SetLoopType(REAnimationLoopTypeFromEndToStart, 1);
			REAnimation::SetType(REAnimationTypeBounceEaseOut);
			birdHead.Get()->SetTransformAnimated(trans);
			REAnimation::Execute();
		}
		
		RE_SAFE_RELEASE(_birdHeadAnimationTimer);
		_birdHeadAnimationTimer = new RETimer();
		if (_birdHeadAnimationTimer) 
		{
			_birdHeadAnimationTimer->SetTrigerTargetMethod(NEW_CLASS_METHOD(MainMenu,this,StartHeadAnimation));
			_birdHeadAnimationTimer->SetTime((RETimeInterval)r.GetFloatInRange(3.8f, 5.8f));
			_birdHeadAnimationTimer->Start();
		}
	}
}

void MainMenu::StartExitAnimation()
{
	if (leftChaine.IsNotNull() && rightChaine.IsNotNull() && exitButton.IsNotNull()) 
	{
		REAffineTransform tr1(leftChaine.Get()->GetTransform());
		REAffineTransform tr2(rightChaine.Get()->GetTransform());
		
		tr1.Rotate(DEG_TO_RAD(19.0f), 0.776194f, 2.842f);
		tr2.Rotate(DEG_TO_RAD(25.0f), 1.73043f, 2.834f);
		
		if (REAnimation::Setup(NULL))
		{
			REAnimation::SetTime(4.0);
			REAnimation::SetLoopType(REAnimationLoopTypeFromEndToStart);
			REAnimation::SetType(REAnimationTypeSinusoidalEaseInOut);
			
			leftChaine.Get()->SetTransformAnimated(tr1);
			rightChaine.Get()->SetTransformAnimated(tr2);
			exitButton.Get()->SetFrameAnimated(RERect(0.490123f, 2.98f, 1.46237f, 0.75f));
			
			REAnimation::Execute();
		}
	}
}

void MainMenu::StartSkyAnimation()
{
	RETextureObject * texture = skyView.Get()->GetTexture();
	if (texture) 
	{
		RETetragon tetr(texture->GetFrame());
		const REFloat32 xInc = (1.0f / 3.0f) * 2.0f;
		tetr.topLeftX += xInc;
		tetr.topRightX = 1.0f;
		tetr.bottomLeftX += xInc;
		tetr.bottomRightX = 1.0f;
		
		if (REAnimation::Setup(NULL))
		{
			REAnimation::SetTime(40.0);
			REAnimation::SetLoopType(REAnimationLoopTypeFromStartToEnd);
			texture->SetFrameAnimated(tetr);
			REAnimation::Execute();
		}
	}
}

void MainMenu::CreateMedia()
{
	/*
	REAudioPlayer * pl = new REAudioPlayer();
	if (pl)
	{
		if (pl->InitFromFilePath(REString("media/sound1.wav")))
		{
			clickSound = pl;
		}
		pl->Release();
	}

	pl = new REAudioPlayer();
	if (pl)
	{
		if (pl->InitFromFilePath(REString("media/snd3.ogg")))
		{
			backgroundMusic = pl;
			pl->SetLooped(true);
			pl->Play();
		}
		pl->Release();
	}
	*/
}

void MainMenu::OnLoadingDidEnd()
{
	if (startButton.IsNotNull()) 
	{
		startButton.Get()->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MainMenu,this,OnStartButtonDown));
	}
	if (exitButton.IsNotNull())
	{
		exitButton.Get()->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MainMenu,this,OnExitButtonDown));
	}
	if (hightScoreButton.IsNotNull())
	{
		hightScoreButton.Get()->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MainMenu,this,OnHightScoreButtonDown));
	}
	if (profileButton.IsNotNull()) 
	{
		profileButton.Get()->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MainMenu,this,OnProfileButtonDown));
	}
	if (optionsButton.IsNotNull()) 
	{
		optionsButton.Get()->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MainMenu,this,OnOptionsButtonDown));
	}
	this->StartHeadAnimation(NULL);
	this->StartExitAnimation();
	if (skyView.IsNotNull()) 
	{
		this->StartSkyAnimation();
	}
	
	this->CreateMedia();
}

void MainMenu::ContinueGameWithCurrentUser()
{
	
}

void MainMenu::CreateNewUserAndStartGame()
{
	
}

void MainMenu::OnStartButtonDown(REObject * sender)
{	
	if (clickSound.IsNotNull()) 
	{
		clickSound.Get()->Play();
	}
}

REObject * MainMenu::CreateXMLSerializableObject(const char * className, const char * key) const
{
	REObject * newObj = REStackedViewController::CreateXMLSerializableObject(className, key);
	if (newObj)
	{
		return newObj;
	}
	
	if (strcmp(className, "MainMenuExitButton") == 0) 
	{
		MainMenuExitButton * newBtn = new MainMenuExitButton();
		return newBtn;
	}
	
	return NULL;
}

REBOOL MainMenu::AcceptObjectParameter(const char * className, const char * key, REObject * value)
{
	if (REStackedViewController::AcceptObjectParameter(className, key,value)) 
	{
		return true;
	}
	
	if (strcmp(className, "MainMenuExitButton") == 0) 
	{
		MainMenuExitButton * btn = value->GetCasted<MainMenuExitButton>();
		if (btn) 
		{
			this->AddSubView(btn);
			btn->Release();
			return true;
		}
	}
	
	return false;
}

IREProperty * MainMenu::GetPropertyForKey(const char * key)
{
	if (strcmp(key, "startButton") == 0) { return (&startButton); }
	else if (strcmp(key, "birdHead") == 0) { return (&birdHead); }
	else if (strcmp(key, "exitButton") == 0) { return (&exitButton); }
	else if (strcmp(key, "leftChaine") == 0) { return (&leftChaine); }
	else if (strcmp(key, "rightChaine") == 0) { return (&rightChaine); }
	else if (strcmp(key, "skyView") == 0) { return (&skyView); }
	else if (strcmp(key, "hightScoreButton") == 0) { return (&hightScoreButton); }
	else if (strcmp(key, "profileButton") == 0) { return (&profileButton); }
	else if (strcmp(key, "optionsButton") == 0) { return (&optionsButton); }
	return NULL;
}

void MainMenu::OnProfileButtonDown(REObject * sender)
{
	
}

void MainMenu::OnExitButtonDown(REObject * sender)
{
	
}

void MainMenu::OnExitFromApp(REObject * sender)
{
	
}

void MainMenu::OnHightScoreButtonDown(REObject * sender)
{
	
}

void MainMenu::OnOptionsButtonDown(REObject * sender)
{
	
}

void MainMenu::OnViewDidRemoved(REView * view)
{
	REStackedViewController::OnViewDidRemoved(view);
}

void MainMenu::OnViewDidAdded(REView * view)
{
	REStackedViewController::OnViewDidAdded(view);
}


void MainMenu::Render()
{
	if (_loadingIndicator) 
	{
		_loadingIndicator->SetBackgroundLoadingProgress(this->GetBackgroundLoadingProgress());
		_loadingIndicator->Render();
	}
	else
	{
		REStackedViewController::Render();
	}
}

void MainMenu::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	if (_loadingIndicator) 
	{
		_loadingIndicator->SetBackgroundLoadingProgress(this->GetBackgroundLoadingProgress());
		_loadingIndicator->RenderWithOffset(offsetX, offsetY);
	}
	else
	{
		REStackedViewController::RenderWithOffset(offsetX, offsetY);
	}
}

MainMenu::MainMenu() : REStackedViewController(),
	_birdHeadAnimationTimer(NULL),
	_loadingIndicator(NULL)
{
	
}

MainMenu::~MainMenu()
{
	if (skyView.IsNotNull()) 
	{
		RETextureObject * texture = skyView.Get()->GetTexture();
		if (texture) 
		{
			texture->StopAnimation(REAnimationStopTypeImmediately, false);
		}
	}
	
	RE_SAFE_RELEASE(_birdHeadAnimationTimer);
	RE_SAFE_RELEASE(_loadingIndicator);
}



