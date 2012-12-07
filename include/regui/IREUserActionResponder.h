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


#ifndef __IREUSERACTIONRESPONDER_H__
#define __IREUSERACTIONRESPONDER_H__

#include "../recore/RECommonHeader.h"

/// Class interface used for object that can receive user messages, clicks/touches.
/// User action moves down from top object to parent in object hierarchy.
/// For receiving events object and all parent objects must responds for user action IsRespondsForUserAction().
class __RE_PUBLIC_CLASS_API__ IREUserActionResponder
{
public:
	/// Return value that indicates that object intercepts/block user action from parent objects.
	/// Default value is 'true', in this case scaning objects will be breaked on this object.
	/// If this value is 'false' than user action will be directed through this object and directed to parent objects.
	virtual REBOOL IsInterceptsUserAction() const = 0;
	
	/// Set value that indicates that object intercepts/block user action from parent objects.
	virtual void SetInterceptsUserAction(REBOOL isInterceptsAction) = 0;
	
	/// Return value that indicate object responding.
	/// If this value if 'true' than object can handle user actions.
	virtual REBOOL IsRespondsForUserAction() const = 0;
	
	/// Set value that indicates object responding.
	virtual void SetRespondsForUserAction(REBOOL isRespondsForAction) = 0;
	
#ifdef __RE_OS_DESKTOP__
	virtual void UserCursorMoved(const REFloat32 coordX, const REFloat32 coordY) = 0;
	virtual REUInt32 GetUserCursorIdentifier() const = 0; /* {return 0;} */
#endif	
	/// Tell object about click/touch event is started in coordinates using screen coordinates.
	virtual void UserActionClickDidStart(const REFloat32 coordX, const REFloat32 coordY) = 0;
	
	/// Tell object about click/touch event is ended in coordinates using screen coordinates.
	virtual void UserActionClickDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) = 0;
	
	/// Tell object about click/touch event is moving in coordinates using screen coordinates.
	virtual void UserActionClickMoving(const REFloat32 startCoordX, const REFloat32 startCoordY, 
									   const REFloat32 currentCoordX, const REFloat32 currentCoordY) = 0;
	
	/// Tell object about moving click/touch event is ended in coordinates using screen coordinates.
	virtual void UserActionClickMovingDidEnd(const REFloat32 startCoordX, const REFloat32 startCoordY, 
											 const REFloat32 endCoordX, const REFloat32 endCoordY) = 0;
	
	virtual ~IREUserActionResponder() { }
};


#endif /* __IREUSERACTIONRESPONDER_H__ */


