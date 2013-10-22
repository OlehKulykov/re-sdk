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
#include "../recore/REArray.h"
#include "../recore/REList.h"
#include "../recore/REArrayObject.h"
#include "../recore/REThread.h"
#include "../recore/REMutex.h"

class REView;

/// Class using for storing views. 
/// All stored views is Retain()'ed. So you can create view, add to this container and than Release().
class __RE_PUBLIC_CLASS_API__ RESubviewsContainer
{
private:
	static REList<REView *>::NodePtr newNode(REView* const & newValue);
	static void deleteNode(REList<REView *>::NodePtr node);
	
	REList<REView *> _subviews;
	RESubviewsContainer * _parentSubviewsContainer;
public:
	REList<REView *>::Iterator subviewsIterator() const;
	
	/// Return view that containes this container.
	/// Return view or NULL.
	RESubviewsContainer * parentSubviewsContainer() const;
	
	/// Check this container for childs view.
	REBOOL isHasSubviews() const;
	
	/// Adds view to this container.
	/// After adding view Retain will call and 'OnViewDidAdded(REView *)' to this object.
	REBOOL addSubview(REView * v);
		
	/// Removes view from this container.
	/// After removing view Release will call.
	REBOOL removeSubview(REView * v);
	
	/// Removes all views from this container.
	/// For all contained views Release will call.
	void removeAllSubviews();
	
	/// Return first oqurence of view with 'viewTag'
	REView * subviewWithTag(const REInt32 viewTag);
	
	RESubviewsContainer();
	virtual ~RESubviewsContainer();
};



#endif /* __RESUBVIEWSCONTAINER_H__ */


