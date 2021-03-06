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


#ifndef __REDBRESULTSET_H__
#define __REDBRESULTSET_H__


#include "RECommonHeader.h"
#include "REArray.h"
#include "REString.h"

/// Base class of data base result sets.
class __RE_PUBLIC_CLASS_API__ REDBResultSet
{	
public:
	virtual REBOOL isOK() const;
	
	virtual ~REDBResultSet();
};


#endif /* __REDBRESULTSET_H__ */ 

