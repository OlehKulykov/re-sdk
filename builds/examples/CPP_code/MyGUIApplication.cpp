

#include "MyGUIApplication.h"
#include "MyRootViewController.h"
#include "MainMenu.h"

#define MAIN_MENU_CUSTOM_VIEW_TAG -2552

void MyGUIApplication::CreateUIInCode()
{
	REViewController * rvc = this->GetRootViewController();
	MyRootViewController * rootViewController = rvc->GetCasted<MyRootViewController>();
	if ((rvc == NULL) || (rootViewController == NULL)) 
	{
		return;
	}
	
	REView * view1 = new REView();
	if (view1) 
	{
		view1->SetFrame(RERect(0.0f, 0.0, 0.5f, 0.5f));
		view1->SetColor(REColor(1.0f, 0.0f, 0.0f, 1.0f));
		rvc->AddSubView(view1);
	}
	RE_SAFE_RELEASE(view1);
	
	
	REView * texturedView = new REView();
	if (texturedView) 
	{
		RETextureObject * texture1 = RETextureObject::CreateWithImageFilePath(REString("data/vc/load/img/back.png"));
		texturedView->SetFrame(RERect(0.5f, 0.0f, 1.5f, 1.5f));
		texturedView->SetTexture(texture1);
		RE_SAFE_RELEASE(texture1);
		rvc->AddSubView(texturedView);
	}
	RE_SAFE_RELEASE(texturedView);
	
	REButton * button1 = new REButton();
	if (button1) 
	{
		button1->SetFrame(RERect(0.0f, 0.5f, 1.0f, 0.5f));
		
		RETextureObject * normal = RETextureObject::CreateWithImageFilePath(REString("data/vc/mainmenu/img/exit.png"));
		button1->SetTextureForState(normal, REButton::StateDefault);
		RE_SAFE_RELEASE(normal);
		
		RETextureObject * pressed = RETextureObject::CreateWithImageFilePath(REString("data/vc/mainmenu/img/exith.png"));
		button1->SetTextureForState(pressed, REButton::StatePressed);
		RE_SAFE_RELEASE(pressed);
		
		RETextureObject * disabled = RETextureObject::CreateWithImageFilePath(REString("data/vc/mainmenu/img/exit.png"));
		button1->SetTextureForState(disabled, REButton::StateDisabled);
		RE_SAFE_RELEASE(disabled);
		
		rvc->AddSubView(button1);
		button1->SetRespondsForUserAction(true);
		button1->SetButtonDownTargetMethod(NEW_CLASS_METHOD(MyRootViewController, rootViewController, OnTestButtonDown));
	}
	RE_SAFE_RELEASE(button1);
	
	RELabel * label1 = new RELabel();
	if (label1) 
	{
		label1->SetFrame(RERect(0.0f, 1.7f, 1.5f, 0.1f));
		REFontObject * font = REFontObject::CreateWithPath(REString("data/fnt/monaco.ttf"), label1->GetFrame().height);
		label1->SetFont(font);
		RE_SAFE_RELEASE(font);
		label1->SetText(REString("Hello label"));
		label1->SetTextAlignment(RETextAlignmentCenter);
		label1->SetColor(REColor(0.0f, 1.0f, 0.0f, 1.0f));
		label1->SetShadowColor(REColor(1.0f, 0.0f, 0.0f, 1.0f));
		label1->SetShadowOffset(REPoint2(0.02f, 0.02f));
		label1->SetUsingShadow(true);
		rvc->AddSubView(label1);
		rootViewController->testLabel = label1;
	}
	RE_SAFE_RELEASE(label1);
	
	RESystemInfo::PrintfTypesSizes();
	
	REParticleView * particle = new REParticleView();
	if (particle)
	{
		particle->SetFrame(RERect(1, 1, 0.5, 0.5));
		if (particle->InitFromFilePath(REString("data/part/pyrodemo.pyro")))
		{
			rvc->AddSubView(particle);
		}
		particle->Release();
	}
	
}

void MyGUIApplication::LoadViewControllerFromXML()
{
	REViewController * rvc = this->GetRootViewController();
	MyRootViewController * rootViewController = rvc->GetCasted<MyRootViewController>();
	if ((rvc == NULL) || (rootViewController == NULL)) 
	{
		return;
	}
	
	MainMenu * mainMenu = new MainMenu();
	if (mainMenu) 
	{
		mainMenu->SetTag(MAIN_MENU_CUSTOM_VIEW_TAG);
		mainMenu->SetRespondsForUserAction(true);

		rootViewController->AddSubView(mainMenu);

		mainMenu->LoadByName(REString("mainmenu"), false);
		mainMenu->Release();
	}
	
	//rootViewController->LoadByName(REString("mainmenu"), false);
}

void MyGUIApplication::LoadViewControllerFromXMLWithLoading()
{
	REViewController * rvc = this->GetRootViewController();
	MyRootViewController * rootViewController = rvc->GetCasted<MyRootViewController>();
	if ((rvc == NULL) || (rootViewController == NULL)) 
	{
		return;
	}
	

	MainMenu * mainMenu = new MainMenu();
	if (mainMenu) 
	{
		mainMenu->SetTag(MAIN_MENU_CUSTOM_VIEW_TAG);
		mainMenu->SetRespondsForUserAction(true);
		
		rootViewController->AddSubView(mainMenu);
		
		mainMenu->LoadByName(REString("mainmenu"), true);
		mainMenu->Release();
	}
}

void MyGUIApplication::Start()
{
	if (!_isStarted) 
	{
		// Any start actions
		REViewController * rvc = this->GetRootViewController();
		MyRootViewController * rootViewController = rvc->GetCasted<MyRootViewController>();
		if ((rvc == NULL) || (rootViewController == NULL)) 
		{
			return;
		}
		
		RERenderDevice * device = RERenderDevice::GetDefaultDevice();
		rvc->SetFrame(RERect(REPoint2(0.0f, 0.0f), device->GetRenderSize()));
		rootViewController->SetRespondsForUserAction(true);
		
		RETimer * timer = new RETimer();// RETimer::Create();
		//timer->SetTrigerTargetMethod(NEW_CLASS_METHOD(MyGUIApplication, this, M1));
		//timer->SetTrigerTargetMethod(NEW_CLASS_METHOD(MyGUIApplication, this, M1));
		//timer->SetTrigerTargetMethod(NULL);
		REMainLoopObjectsStorage::GetDefaultStorage()->RemoveObject(timer);
		REAutoReleasePool * pool = REAutoReleasePool::GetDefaultPool();
		pool->Update(0);
		timer->Release();
		pool->Update(0);

		REStringObject * s = REStringObject::CreateWithChars("dsadsa");
		s->Release();
		pool->Update(0);

		RETextureObject * text1 = RETextureObject::Create();
		text1->Release();
		pool->Update(0); 

		MainMenu * mm = new MainMenu();
		mm->Release();
		pool->Update(0);

		this->CreateUIInCode();
		
		//this->LoadViewControllerFromXML();
		
		//this->LoadViewControllerFromXMLWithLoading();
			
		_isStarted = true;
	}
}

REString MyGUIApplication::GetName() const
{
	return REString("MyGUIApplication");
}

MyGUIApplication::MyGUIApplication(REViewController * rootViewController, void * hWND) : 
	REGUIApplication(),
	_isStarted(false)
{
	this->SetRootViewController(rootViewController);
}

MyGUIApplication::~MyGUIApplication()
{
	
}


