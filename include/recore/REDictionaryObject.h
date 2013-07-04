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


#ifndef __REDICTIONARYOBJECT_H__
#define __REDICTIONARYOBJECT_H__

#include "RECommonHeader.h"
#include "REArray.h"
#include "REObject.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

/// Class for storing objects using by key value.
class __RE_PUBLIC_CLASS_API__ REDictionaryObject : public REObject
{
public:
	/// Structure of dictionary used for store key pairs.
	typedef struct _keyObjectStruct
	{
		/// Pointer to Retain()'ed object.
		REObject * keyValue;
		
		/// Pointer to Retain()'ed key object.
		REObject * objValue;
	} KeyObjectStruct;

private:
	REArray<REDictionaryObject::KeyObjectStruct> _pairs;
	REDictionaryObject::KeyObjectStruct * pairForKey(REObject * keyObj);

	static REBOOL isEqualByPairs(REObject * anotherObject, REArray<REDictionaryObject::KeyObjectStruct> * pairs);
	static REBOOL writePairsToFilePath(REArray<REDictionaryObject::KeyObjectStruct> * pairs, const REString & path);
	static REBOOL readPairsFromFilePath(REArray<REDictionaryObject::KeyObjectStruct> * pairs, const REString & path);

	
protected:
	/// Default constructor.
	REDictionaryObject();
	
	/// Destructor.
	virtual ~REDictionaryObject();
	
public:
	/// Return pointer for array of key object pairs.
	/// All objects is Retain()'ed.
	REArray<REDictionaryObject::KeyObjectStruct> * getPairs();
	
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier(); 
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL isEqual(REObject * anotherObject);
	virtual void onReleased();
	
	/// Writes dictionary using text plist format to file 'path'.
	/// All intermediate directories of 'path' must exists.
	/// If file for 'path' allready exists it's will be rewrited.
	REBOOL writeToFile(const REString & path);
	
	/// Initializes dictionary key object pairs from file 'path'.
	/// File must be in text plist format.
	REBOOL initFromFilePath(const REString & path);
	
	/// Reads all dictionary key objects to array 'keys'.
	/// Use this keys for accessing to dictionary objects.
	/// Returns 'false' if 'keys' is NULL or error adding occurred.
	REBOOL readAllKeys(REArray<REObject*> * keys);
	
	/// Removes all key pairs from dictionary.
	/// Each key and object of pair called Release().
	void clear();
	
	/// Setting object value for key.
	/// After successfull setting object and key both will be Retain()'ed.
	/// If object allready exists old object will be repleced with new one.
	/// In this case old object call Release() and new object call Retain().
	/// You must use REStringObject for key.
	REBOOL setObject(REObject * objValue, REObject * keyObject);
	
	/// Setting object value for key as in 'REBOOL SetObject(REObject * objValue, REObject * keyObject);'
	/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
	REBOOL setObject(REObject * objValue, const REString & stringKey);
	
	/// Setting object value for key as in 'REBOOL SetObject(REObject * objValue, REObject * keyObject);'
	/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
	REBOOL setObject(REObject * objValue, const char * stringKey);
	
	/// Return object or NULL for key.
	REObject * getObjectForKey(REObject * keyObject);
	
	/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
	/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
	REObject * getObjectForKey(const REString & stringKey);
	
	/// Return object or NULL for key as in 'REObject * GetObjectForKey(REObject * keyObject);'
	/// As 'keyObject' will use REStringObject initialized from 'stringKey'.
	REObject * getObjectForKey(const char * stringKey);
	
	/// Creates and return new dictionary object.
	static REDictionaryObject * create();
};

#endif /* __REDICTIONARYOBJECT_H__ */

