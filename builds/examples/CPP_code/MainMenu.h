

#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include <recore.h>
#include <regui.h>
#include <remedia.h>

#include "Loading.h"

class MainMenu : public REStackedViewController
{
protected:
	void OnStartButtonDown(REObject * sender);
	void OnProfileButtonDown(REObject * sender);
	void OnExitButtonDown(REObject * sender);
	void OnHightScoreButtonDown(REObject * sender);
	void OnOptionsButtonDown(REObject * sender);
	void OnExitFromApp(REObject * sender);
	
	/// Called by controller when start it's loading in background. 
	virtual void OnBackgroundLoadingDidStart();
	/// Called by controller when background loading is finished.
	virtual void OnBackgroundLoadingDidEnd();
	
	virtual void OnViewDidRemoved(REView * view);
	virtual void OnViewDidAdded(REView * view);
	virtual void OnLoadingDidEnd();
	virtual REObject * CreateXMLSerializableObject(const char * className, const char * key) const;
	
	void StartHeadAnimation(REObject * timerObject);
	void StartExitAnimation();
	void StartSkyAnimation();
	
	void ContinueGameWithCurrentUser();
	void CreateNewUserAndStartGame();
	
	void CreateMedia();
	
	RETimer * _birdHeadAnimationTimer;
	Loading * _loadingIndicator;
	
public:
	virtual REBOOL AcceptObjectParameter(const char * className, const char * key, REObject * value);
	virtual IREProperty * GetPropertyForKey(const char * key);
	virtual void Render();
	virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	RERetainProperty<REButton*> startButton; 
	RERetainProperty<RETransformedView *> birdHead; 
	RERetainProperty<REButton *> exitButton;
	RERetainProperty<RETransformedView *> leftChaine;
	RERetainProperty<RETransformedView *> rightChaine;
	RERetainProperty<REView *> skyView;
	RERetainProperty<REButton *> hightScoreButton;
	RERetainProperty<REButton *> profileButton;
	RERetainProperty<REButton *> optionsButton;
	
	RERetainProperty<REAudioPlayer *> clickSound;
	RERetainProperty<REAudioPlayer *> backgroundMusic;
	
	MainMenu();
	virtual ~MainMenu();
};


#endif /* __MAINMENU_H__ */
