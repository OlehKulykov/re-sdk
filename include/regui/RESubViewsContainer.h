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


#ifndef __RESUBVIEWSCONTAINER_H__
#define __RESUBVIEWSCONTAINER_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REArrayObject.h"
#include "../recore/REThread.h"
#include "../recore/REMutex.h"

class REView;

/// Class using for storing views. 
/// All stored views is Retain()'ed. So you can create view, add to this container and than Release().
class __RE_PUBLIC_CLASS_API__ RESubViewsContainer
{
private:
	REArrayObject * _subViewsArray;
	RESubViewsContainer * _parentSubViewsContainer;
	REMutex _updateMutex;
protected:
	void lockUpdate();
	void unLockUpdate();
	
	/// Returns pointer to array contained subviews.
	/// If array not created it will creates.
	REArrayObject * getOrCreateAndGetSubViewsArray();
	
	/// Called when some 'view' added to container.
	virtual void onViewDidAdded(REView * view) { }
	
	/// Called when some 'view' removed from container.
	/// Before calling this method 'view' Retain()'s and after calling 'view' Release()'d.
	virtual void onViewDidRemoved(REView * view) { }
public:
	/// Return view that containes this container.
	/// Return view or NULL.
	REView * getParentSubViewsContainer() const;
	
	/// Return array of subviews.
	/// Return array pointer or NULL
	REArrayObject * getSubViewsArray() const { return _subViewsArray; }
	
	/// Check this container for childs view.
	REBOOL isHasSubViews() const;
	
	/// Adds view to this container.
	/// After adding view Retain will call and 'OnViewDidAdded(REView *)' to this object.
	REBOOL addSubView(REView * v);
	
	/// Inserts view above 'targetView'.
	/// After adding view Retain will call.
	/// Returns false if one of params is NULL or 'targetView' not containes.
	REBOOL insertSubViewAbove(REView * v, REView * targetView);
	
	/// Inserts view below 'targetView'.
	/// After adding view Retain will call.
	/// Returns false if one of params is NULL or 'targetView' not containes.
	REBOOL insertSubViewBelow(REView * v, REView * targetView);
	
	/// Removes view from this container.
	/// After removing view Release will call.
	REBOOL removeSubView(REView * v);
	
	/// Removes all views from this container.
	/// For all contained views Release will call.
	void removeAllSubViews();
	
	/// Replaces first view with second view.
	/// After replacing for first view will call Release and for second will call Retain.
	REBOOL replaceSubView(REView * v, REView * withView);
	
	/// Return first oqurence of view with 'viewTag'
	REView * findSubViewWithTag(const REInt32 viewTag);
	
	/// Return first oqurence of view with 'className'
	REView * findSubViewWithClassName(const char * className);
	
	
	RESubViewsContainer();
	virtual ~RESubViewsContainer();
};



#endif /* __RESUBVIEWSCONTAINER_H__ */


