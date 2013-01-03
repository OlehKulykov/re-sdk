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

#ifndef __RESTACKEDVIEWCONTROLLER_H__
#define __RESTACKEDVIEWCONTROLLER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REArray.h"
#include "REViewController.h"

///Class of stacked view controller.
class __RE_PUBLIC_CLASS_API__ REStackedViewController : public REViewController
{
protected:
	REArray<REObjectsArray *> * _stackedSubViewsArray;
	
	REStackedViewController();
	virtual ~REStackedViewController();
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual void OnReleased();
	
	/// Pushes all subviews and removes or not current subview.
	REBOOL PushSubViews(const REBOOL isRemoveCurrentSubViews = true);
	
	/// Pops back pushed subview.
	REBOOL PopSubViews();
	
	/// Creates and returns new object of stacked view controller.
	static REStackedViewController * Create();
};


#endif /* __RESTACKEDVIEWCONTROLLER_H__ */


