/*
 *   Copyright 2012 Kulykov Oleh
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
#include "../recore/REObjectsArray.h"
#include "../recore/REThread.h"
#include "../recore/REMutex.h"

class REView;

/// Class using for storing views. 
/// All stored views is Retain()'ed. So you can create view, add to this container and than Release().
class __RE_PUBLIC_CLASS_API__ RESubViewsContainer
{
private:
	REObjectsArray * _subViewsArray;
	RESubViewsContainer * _parentSubViewsContainer;
	REMutex _updateMutex;
protected:
	void LockUpdate();
	void UnLockUpdate();
	
	/// Returns pointer to array contained subviews.
	/// If array not created it will creates.
	REObjectsArray * GetOrCreateAndGetSubViewsArray();
	
	/// Called when some 'view' added to container.
	virtual void OnViewDidAdded(REView * view) { }
	
	/// Called when some 'view' removed from container.
	/// Before calling this method 'view' Retain()'s and after calling 'view' Release()'d.
	virtual void OnViewDidRemoved(REView * view) { }
public:
	/// Return view that containes this container.
	/// Return view or NULL.
	REView * GetParentSubViewsContainer() const;
	
	/// Return array of subviews.
	/// Return array pointer or NULL
	REObjectsArray * GetSubViewsArray() const { return _subViewsArray; }
	
	/// Check this container for childs view.
	REBOOL IsHasSubViews() const;
	
	/// Adds view to this container.
	/// After adding view Retain will call and 'OnViewDidAdded(REView *)' to this object.
	REBOOL AddSubView(REView * v);
	
	/// Inserts view above 'targetView'.
	/// After adding view Retain will call.
	/// Returns false if one of params is NULL or 'targetView' not containes.
	REBOOL InsertSubViewAbove(REView * v, REView * targetView);
	
	/// Inserts view below 'targetView'.
	/// After adding view Retain will call.
	/// Returns false if one of params is NULL or 'targetView' not containes.
	REBOOL InsertSubViewBelow(REView * v, REView * targetView);
	
	/// Removes view from this container.
	/// After removing view Release will call.
	REBOOL RemoveSubView(REView * v);
	
	/// Removes all views from this container.
	/// For all contained views Release will call.
	void RemoveAllSubViews();
	
	/// Replaces first view with second view.
	/// After replacing for first view will call Release and for second will call Retain.
	REBOOL ReplaceSubView(REView * v, REView * withView);
	
	/// Return first oqurence of view with 'viewTag'
	REView * FindSubViewWithTag(const REInt32 viewTag);
	
	/// Return first oqurence of view with 'className'
	REView * FindSubViewWithClassName(const char * className);
	
	
	RESubViewsContainer();
	virtual ~RESubViewsContainer();
};



#endif /* __RESUBVIEWSCONTAINER_H__ */


