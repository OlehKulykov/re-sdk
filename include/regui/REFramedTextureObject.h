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


#ifndef __REFRAMEDTEXTUREOBJECT_H__
#define __REFRAMEDTEXTUREOBJECT_H__

#include "../recore/RECommonHeader.h"
#include "../recore/RERect.h"
#include "../recore/REMainLoopUpdatable.h"
#include "RETextureObject.h"
#include "REAnimation.h"


/// Class of framed texture that used xml format for storing frames information.
/// This class requared existens of two files: <texture image>.<extension> and <texture image>.xml for frames
class __RE_PUBLIC_CLASS_API__ REFramedTextureObject : public RETextureObject, public IREAnimationRespondent
{
private:
	static REBOOL InitFramesForTexture(const char * data, REFramedTextureObject * texture);

protected:
	REArray<RERectStruct> _frames;
	REFloat32 _frameIndexParam;
	REUInt16 _animationsCount;
	
	REBOOL InitFrames(const char * data);
	REBOOL InitFramedTextureFromPath(const REString & path);
	
	REFramedTextureObject();
	/// Destructor.
	virtual ~REFramedTextureObject();
	
public:
	/// Returns 0 or 'RENotFound' if no frames avaiable.
	const REUInt32 GetFirstFrameIndex() const;
	
	/// Returns 0 or 'RENotFound' if no frames avaiable.
	const REUInt32 GetLastFrameIndex() const;
	
	/// Set current texture frame index.
	REBOOL SetFrameIndex(const REUInt32 newIndex);
	
	/// Set current texture frame index.
	/// Can be animated using REAnimation, call this method between REAnimation::Setup() and REAnimation::Execute().
	/// If animation is not settuping !REAnimation::IsSetuping() than SetFrameIndex() will be called.
	REBOOL SetFrameIndexAnimated(const REUInt32 newIndex);
	
	/// Checks is view params is animating.
	REBOOL IsAnimating() const { return (_animationsCount > 0); }
	
	/// Stopes all animation of view with stop type.
	/// Value of parameter 'isNeedCallDelegate' indicates than need inform animation delegate about stoping animation.
	void StopAnimation(const REAnimationStopType stopType, const REBOOL isNeedCallDelegate);
	
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/* REGUIObject */
	virtual REBOOL AcceptStringParameter(const char * key, const char * value);
	
	/* IREAnimationRespondent */
	virtual void REAnimationRespondentDidAddedToAnimation(const REUIdentifier animationIdentifier) { }
	virtual void REAnimationRespondentAnimationDidStarted(const REUIdentifier animationIdentifier) { }
	virtual void REAnimationRespondentAnimationDidFinished(const REUIdentifier animationIdentifier) { }
	virtual void REAnimationRespondentAnimationDidStep(const REFloat32 progress);
		
	
	/// Creates and returns new empty 'REFramedTextureObject' or NULL on error.
	static REFramedTextureObject * Create();
	
	/// Creates and returns new 'REFramedTextureObject' initialized with image data from 'image' and
	/// frames from xml file with same file name.
	/// File path 'filePath' must be path to texture image and for xml frames file extension should differ only
	/// Example: texture path = <folder/textureImage.png> and xml file must be: <folder/textureImage.xml>
	static REFramedTextureObject * CreateWithFilePath(const REString & filePath, 
													  const RETextureFilterType filterType = RETextureFilterLinear);
	
};

#endif /* __REFRAMEDTEXTUREOBJECT_H__ */


