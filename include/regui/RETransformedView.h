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


#ifndef __RETRANSFORMEDVIEW_H__
#define __RETRANSFORMEDVIEW_H__

#include "../recore/RECommonHeader.h"
#include "../recore/RETetragon.h"
#include "../recore/REAffineTransform.h"
#include "../recore/IREAffineTransformed.h"
#include "REView.h"
#include "IREAnimationRespondent.h"

/// Class of irregular view used for display.
class __RE_PUBLIC_CLASS_API__ RETransformedView : public REView
{
protected:
	RETetragon _transformedFrame;
	REAffineTransform _transform;
	
	RETransformedView();
	virtual ~RETransformedView();
public:
	const REAffineTransform & GetTransform() const;
	void setTransform(const REAffineTransform & newTransform);
	void setTransformAnimated(const REAffineTransform & newTransform);
	
	const RETetragon & getTransformedFrame() const;
	void setTransformedFrame(const RETetragon & newFrame);
	void setTransformedFrameAnimated(const RETetragon & newFrame);
	
	/* REView */
	virtual void setFrame(const RERect & newViewFrame);
	virtual void setFrameAnimated(const RERect & newViewFrame);
	
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	
	/* IRERenderable */
	virtual void render();
	virtual void renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);
	
	/// Creates and returns new transformed view object.
	static RETransformedView * create();
};

#endif /* __RETRANSFORMEDVIEW_H__ */

