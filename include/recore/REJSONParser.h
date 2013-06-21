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


#ifndef __REJSONPARSER_H__
#define __REJSONPARSER_H__

#include "RECommonHeader.h"
#include "REBufferObject.h"
#include "REObjectsDictionary.h"
#include "REData.h"


class __RE_PUBLIC_CLASS_API__ REJSONParser 
{
private:
	REBufferObject * _buffer;
	REObject * _rootObject;
public:
	REObject * getRootObject() const;
	REBOOL parse();
	REJSONParser(const REData & data);
	REJSONParser(REBufferObject * buffer);
	REJSONParser(REObjectsDictionary * dictionary, REBufferObject * buffer);
	~REJSONParser();
};


#endif /* __REJSONPARSER_H__ */

