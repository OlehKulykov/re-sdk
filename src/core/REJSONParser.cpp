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


#include "../../include/recore/REJSONParser.h"
#include "../../include/recore/RENumberObject.h"
#include "../../include/recore/RENULLObject.h"
#include "../../include/recore/REObjectsArray.h"

#include <inttypes.h>
#include <stdlib.h>

/// For PC better 32bit char
//#define CHAR_TYPE uint32_t

/// For device better 16bit char(16bit registers)
#define CHAR_TYPE uint16_t

//#define CHAR_TYPE char


//#define OBJ_TYPE_TYPE uint32_t
#define OBJ_TYPE_TYPE uint16_t


#define CH(c) ((CHAR_TYPE)c)


#define O_DICT				(1)
#define O_ARRAY				(1<<1)
#define O_STRING			(1<<2)
#define O_NUMBER			(1<<3)
#define O_IS_DICT_KEY 		(1<<4)
#define O_IS_DICT_VALUE 	(1<<5)
#define O_IS_ARRAY_ELEM 	(1<<6)


#define ERR_INIT_DICT		(1)
#define ERR_STORE_DICT		(2)
#define ERR_INIT_ARRAY		(3)
#define ERR_STORE_ARRAY		(4)
#define ERR_INIT_STRING		(5)
#define ERR_STORE_STRING	(6)
#define ERR_INIT_NUMBER		(7)
#define ERR_WRONG_LOGIC		(8)

class REJSONParserPrivate
{	
public:
	typedef struct _struct
	{
		uint8_t * data;
		uint8_t * end;
		REObject ** objects;
		OBJ_TYPE_TYPE * types;
		uint32_t capacity;
		int32_t index;
	} Struct;
	
	static void FreeParserDataStruct(REJSONParserPrivate::Struct * p);
	static void CleanAll(REJSONParserPrivate::Struct * p);
	static void * NewMem(const size_t size);
	static uint32_t IncCapacity(REJSONParserPrivate::Struct * p);
	static const char * ErrorCodeDescription(const int32_t errorCode);
	static void Error(REJSONParserPrivate::Struct * p, const int32_t errorCode);
	static void BeforeOutWithError(REJSONParserPrivate::Struct * p, const int32_t errorCode);
	static REObject * TryNumber(REJSONParserPrivate::Struct * p);
	static uint32_t UniCharToUTF8(const uint32_t uniChar, uint8_t * cursor);
	static void ParseReplacementString(const uint8_t * data, uint32_t len, REStringObject ** resString);
	static void ParseString(REJSONParserPrivate::Struct * p, REStringObject ** resString);
	static uint32_t AddObject(REJSONParserPrivate::Struct * p, REObject * obj, const OBJ_TYPE_TYPE type);
	static void EndContainer(REJSONParserPrivate::Struct * p);
	static REObject * Parse(const uint8_t * inData, const uint32_t inDataLength, void ** error);
};

void REJSONParserPrivate::FreeParserDataStruct(REJSONParserPrivate::Struct * p)
{
	if (p->objects) free(p->objects);
	p->objects = 0;
	
	if (p->types) free(p->types);
	p->types = 0;
}

void REJSONParserPrivate::CleanAll(REJSONParserPrivate::Struct * p)
{
	if (p->index >= 0)
	{
		REObject * rootObject = p->objects[0];
		if (rootObject) rootObject->Release();
	}
	REJSONParserPrivate::FreeParserDataStruct(p);
}

void * REJSONParserPrivate::NewMem(const size_t size)
{
	void * m = 0;
	return posix_memalign((void**)&m, 4, size) == 0 ? m : 0;
}

uint32_t REJSONParserPrivate::IncCapacity(REJSONParserPrivate::Struct * p)
{
	const size_t newCapacity = p->capacity + 16;
	
	REObject ** o = (REObject **)REJSONParserPrivate::NewMem(newCapacity * sizeof(REObject*));
	OBJ_TYPE_TYPE * t = (OBJ_TYPE_TYPE *)REJSONParserPrivate::NewMem(newCapacity * sizeof(OBJ_TYPE_TYPE));
	
	if (o && t)
	{
		if (p->capacity)
		{
			memcpy(o, p->objects, sizeof(REObject*) * p->capacity);
			memcpy(t, p->types, sizeof(OBJ_TYPE_TYPE) * p->capacity);
		}
		REJSONParserPrivate::FreeParserDataStruct(p);
		p->objects = o;
		p->types = t;
		p->capacity = newCapacity;
		return 1;
	}
	else 
	{
		if (o) free(o);
		if (t) free(t);
	}
	return 0;
}

const char * REJSONParserPrivate::ErrorCodeDescription(const int32_t errorCode)
{
	switch (errorCode) 
	{
		case ERR_INIT_DICT: return "Can't create dictionary object."; break;
		case ERR_STORE_DICT: return "Can't store dictionary object."; break;
		case ERR_INIT_ARRAY: return "Can't create array object."; break;
		case ERR_STORE_ARRAY: return "Can't store array object."; break;
		case ERR_INIT_STRING: return "Can't create string object."; break;
		case ERR_STORE_STRING: return "Can't store string object."; break;
		case ERR_INIT_NUMBER: return "Can't create number object."; break;
		case ERR_WRONG_LOGIC: return "Internal logic error."; break;
		default: break;
	}
	return 0;
}

void REJSONParserPrivate::Error(REJSONParserPrivate::Struct * p, const int32_t errorCode)
{

}

void REJSONParserPrivate::BeforeOutWithError(REJSONParserPrivate::Struct * p, const int32_t errorCode)
{
	REJSONParserPrivate::CleanAll(p);
	REJSONParserPrivate::Error(p, errorCode);
}

#define IS_CHAR_START_OF_DIGIT(c) (c>=CH('0')&&c<=CH('9'))||c==CH('-')||c==CH('+')
#define IS_GIGIT_CHAR(c) (c>=CH('0')&&c<=CH('9'))||c==CH('-')||c==CH('+')||c==CH('.')||c==CH('e')||c==CH('E')

REObject * REJSONParserPrivate::TryNumber(REJSONParserPrivate::Struct * p)
{
	switch (*p->data) 
	{
		case CH('t'): /// true
			if (strncmp((const char *)p->data, "true", 4) == 0)
			{
				p->data += 3; /// don't set currect offset to last char
				return RENumberObject::CreateWithBool(true);
			} break;
		case CH('f'): /// false
			if (strncmp((const char *)p->data, "false", 5) == 0)
			{
				p->data += 4; /// don't set currect offset to last char
				return RENumberObject::CreateWithBool(false);
			} break;
		case CH('n'): /// null
			if (strncmp((const char *)p->data, "null", 4) == 0)
			{
				p->data += 3; /// don't set currect offset to last char
				return RENULLObject::NULLObject();
			} break;
		default: break; 
	}
	
	
	const char * start = (char *)p->data;
	const uint8_t * end = p->end;
	int isReal = 0, isDigitsPresent = 0;
	do 
	{
		const CHAR_TYPE c = *p->data;
		if ( IS_GIGIT_CHAR(c) )
		{
			if (c == CH('.')) isReal = 1;
			else if (c >= CH('0') && c <= CH('9')) isDigitsPresent = 1;
		}
		else
		{
			if (isDigitsPresent) break;
			else return 0;
		}
	} while (++p->data <= end);
	
	if (isReal)
	{
		char * endConvertion = 0;
		const double v = strtod(start, &endConvertion);
		if (endConvertion) p->data = (uint8_t *)--endConvertion;
		return RENumberObject::CreateWithFloat64(v);
	}
	else
	{
		char * endConvertion = 0;
		const long long v = strtoll(start, &endConvertion, 10);
		if (endConvertion) p->data = (uint8_t *)--endConvertion;
		return RENumberObject::CreateWithInt64(v);
	}
	return 0;
}

uint32_t REJSONParserPrivate::UniCharToUTF8(const uint32_t uniChar, uint8_t * cursor)
{
	int count = 0;
    wchar_t u = uniChar;
	if (u < (uint8_t)0x80) 
	{
		*cursor++ = (uint8_t)u;
		count++;
	} 
	else 
	{
		if (u < 0x0800) 
		{
			*cursor++ = (uint8_t)0xc0 | ((uint8_t) (u >> 6));
			count++;
		} 
		else 
		{
			if (u > 0xffff) 
			{
				/// if people are working in utf8, but strings are encoded in eg. latin1, the resulting
				/// name might be invalid utf8. This and the corresponding code in fromUtf8 takes care
				/// we can handle this without loosing information. This can happen with latin filenames
				/// and a utf8 locale under Unix.
				if ( (u > 0x10fe00) && (u < 0x10ff00) )
				{
					*cursor++ = (u - 0x10fe00);
					count++;
				} 
				else 
				{
					*cursor++ = (uint8_t)0xf0 | ((uint8_t) (u >> 18));
					*cursor++ = (uint8_t)0x80 | (((uint8_t) (u >> 12)) & (uint8_t)0x3f);
					count += 2;
				}
			} 
			else 
			{
				*cursor++ = (uint8_t)0xe0 | ((uint8_t) (u >> 12));
				count++;
			}
			*cursor++ = (uint8_t)0x80 | (((uint8_t) (u >> 6)) & (uint8_t)0x3f);
			count++;
		}
		*cursor++ = (uint8_t)0x80 | ((uint8_t) (u & (uint8_t)0x3f));
		count++;
	}
	return count;
}

void REJSONParserPrivate::ParseReplacementString(const uint8_t * data, uint32_t len, REStringObject ** resString)
{
	uint8_t * newBuffer = (uint8_t *)malloc(len + 1);
	if (newBuffer) 
	{
		const char * startNewBuff = (const char *)newBuffer;
		CHAR_TYPE prev = 0;
		CHAR_TYPE curr = *data;
		while (len--) 
		{
			switch (curr) 
			{
					//TODO: ugly code bellow ...
				case CH('\"'): if (prev == CH('\\')) { *--newBuffer = '\"'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('\\'): if (prev == CH('\\')) { *--newBuffer = '\\'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('/'): if (prev == CH('\\')) { *--newBuffer = '/'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('b'): if (prev == CH('\\')) { *--newBuffer = '\b'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('f'): if (prev == CH('\\')) { *--newBuffer = '\f'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('n'): if (prev == CH('\\')) { *--newBuffer = '\n'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('r'): if (prev == CH('\\')) { *--newBuffer = '\r'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('t'): if (prev == CH('\\')) { *--newBuffer = '\t'; newBuffer++; } else *newBuffer++ = *data; break;
				case CH('u'): if (prev == CH('\\')) 
				{
					const char * scanData = (char *)data;
					uint32_t uniChar = 0;
					if (sscanf(++scanData, "%04x", &uniChar) == 1)
					{
						const uint32_t count = REJSONParserPrivate::UniCharToUTF8(uniChar, --newBuffer);
						newBuffer += count;
						len -= 4;
						data += 4;
						curr = 0;
					}
				}
				else *newBuffer++ = *data;
					break;
				default: *newBuffer++ = *data; break;
			}
			prev = curr;
			curr = *++data;
		}
		
		REStringObject * newString = REStringObject::CreateWithCharsAndLen(startNewBuff, (REUInt32)((const uint8_t *)newBuffer - (const uint8_t *)startNewBuff));
		free(newBuffer);
		if (newString) *resString = newString;
	}
}

void REJSONParserPrivate::ParseString(REJSONParserPrivate::Struct * p, REStringObject ** resString)
{
	int isHasReplacement = 0;
	const uint8_t * start = p->data;
	const uint8_t * data = start;
	const uint8_t * end = p->end;
	CHAR_TYPE prev = 0;
	do 
	{
		const CHAR_TYPE curr = *data;
		if (prev == CH('\\')) 
		{
			switch (curr) 
			{
				case CH('\"'):
				case CH('\\'):
				case CH('/'): 
				case CH('b'): 
				case CH('f'): 
				case CH('n'): 
				case CH('r'): 
				case CH('t'): 
				case CH('u'): isHasReplacement = 1; break;
				default: break;
			}
		}
		if (curr == CH('\"') && prev != CH('\\')) { p->data = (uint8_t *)data; break; }
		prev = curr;
	} while (++data <= end);
	
	if (isHasReplacement) REJSONParserPrivate::ParseReplacementString(start, (data - start), resString);
	else *resString = REStringObject::CreateWithCharsAndLen((const char*)start, (REUInt32)(data - start));
}

#define IS_CONTAINER(o) ((o&O_DICT)||(o&O_ARRAY)) 

uint32_t REJSONParserPrivate::AddObject(REJSONParserPrivate::Struct * p, REObject * obj, const OBJ_TYPE_TYPE type)
{	
	const int addIndex = p->index + 1;
	
	if (addIndex >= p->capacity) if (!REJSONParserPrivate::IncCapacity(p)) return 0;
	
	if (addIndex == 0)
	{
		p->index = addIndex; p->objects[addIndex] = obj; p->types[addIndex] = type;
		return 1;
	}
	else 
	{
		const int currIndex = p->index;
		if ( p->types[currIndex] & O_ARRAY )
		{
			REObjectsArray * arr = (REObjectsArray *)p->objects[currIndex];
			arr->Add(obj);
			obj->Release();
			if ( IS_CONTAINER(type) )
			{
				p->index = addIndex; p->objects[addIndex] = obj; p->types[addIndex] = (type | O_IS_ARRAY_ELEM);
			}
			return 1;
		}
		else if ( p->types[currIndex] & O_DICT )
		{
			if ( IS_CONTAINER(type) ) return 0;
			else 
			{
				p->index = addIndex; p->objects[addIndex] = obj; p->types[addIndex] = (type | O_IS_DICT_KEY);
			}
			return 1;
		}
		
		const int prevIndex = currIndex - 1;
		if (prevIndex >= 0)
			if ( p->types[prevIndex] & O_DICT )
			{
				REObjectsDictionary * dict = (REObjectsDictionary *)p->objects[prevIndex];
				dict->SetObject(obj, p->objects[currIndex]);
				obj->Release();
				(p->objects[currIndex])->Release();
				if ( IS_CONTAINER(type) )
				{
					p->index = addIndex; p->objects[addIndex] = obj; p->types[addIndex] = (type | O_IS_DICT_VALUE);
				}
				else 
				{
					p->index = prevIndex;
				}
				return 1;
			}
	}
	return 0;
}

void REJSONParserPrivate::EndContainer(REJSONParserPrivate::Struct * p)
{
	const int currIndex = p->index;
	if (currIndex > 0)
	{
		const OBJ_TYPE_TYPE currType = p->types[currIndex];
		if (currType & O_IS_ARRAY_ELEM)	p->index = (currIndex - 1);
		else if (currType & O_IS_DICT_VALUE) p->index = (currIndex - 2);
	}
}

REObject * REJSONParserPrivate::Parse(const uint8_t * inData, const uint32_t inDataLength, void ** error)
{
	REJSONParserPrivate::Struct p = { 0 };
	p.index = -1;
	//p.error = (CFErrorRef *)error;
	p.data = (uint8_t *)inData;
	const uint8_t * end = p.end = p.data + inDataLength;
	
	do 
	{
		const CHAR_TYPE c = *p.data;
		
		switch (c) 
		{
			case CH('{'):
			{
				REObjectsDictionary * newDict = REObjectsDictionary::Create();
				if (newDict)
				{
					if (!REJSONParserPrivate::AddObject(&p, newDict, O_DICT)) 
					{
						REJSONParserPrivate::BeforeOutWithError(&p, ERR_STORE_DICT); 
						return 0;
					}
				}
				else 
				{
					REJSONParserPrivate::BeforeOutWithError(&p, ERR_INIT_DICT); 
					return 0;
				}
			} break;
				
			case CH('}'): REJSONParserPrivate::EndContainer(&p); break;
			case CH(']'): REJSONParserPrivate::EndContainer(&p); break;
				
			case CH('['):
			{
				REObjectsArray * newArray = REObjectsArray::CreateWithCapacity(2);
				if (newArray) 
				{
					if (!REJSONParserPrivate::AddObject(&p, newArray , O_ARRAY)) 
					{
						REJSONParserPrivate::BeforeOutWithError(&p, ERR_STORE_ARRAY);
						return 0;
					}
				}
				else 
				{
					REJSONParserPrivate::BeforeOutWithError(&p, ERR_INIT_ARRAY);
					return 0; 
				}
			} break;
				
			case CH('\"'):	
			{
				p.data++;
				REStringObject * newString = 0;
				REJSONParserPrivate::ParseString(&p, &newString);
				if (newString) 
				{
					if (!REJSONParserPrivate::AddObject(&p, newString, O_STRING)) 
					{
						REJSONParserPrivate::BeforeOutWithError(&p, ERR_STORE_STRING);
						return 0; 
					}
				}
				else 
				{
					REJSONParserPrivate::BeforeOutWithError(&p, ERR_INIT_STRING); 
					return 0;
				}
			} break;
				
			default:
			{
				if (IS_CHAR_START_OF_DIGIT(c) || c == CH('t') || c == CH('f') || c == CH('n'))
				{
					REObject * newNumber = REJSONParserPrivate::TryNumber(&p);
					if (newNumber) 
					{
						if (!REJSONParserPrivate::AddObject(&p, newNumber, O_NUMBER)) 
						{
							REJSONParserPrivate::BeforeOutWithError(&p, ERR_INIT_NUMBER);
							return 0;
						}
					}
				}
			} break;
		}
	} while (++p.data <= end);
	
	if ( p.index == 0 && IS_CONTAINER(p.types[0]) )
	{
		REObject * r = p.objects[0];
		REJSONParserPrivate::FreeParserDataStruct(&p);
		return r;
	}
	
	REJSONParserPrivate::BeforeOutWithError(&p, ERR_WRONG_LOGIC);
	
	return 0;
}


REObject * REJSONParser::GetRootObject() const
{
	return _rootObject;
}

REBOOL REJSONParser::Parse()
{
	if (_buffer)
	{
		_rootObject = REJSONParserPrivate::Parse((const uint8_t *)_buffer->GetBuffer(),
												 (const uint32_t)_buffer->GetSize(), 
												 NULL);
		return true;
	}
	return false;
}

REJSONParser::REJSONParser(const REData & data) :
	_rootObject(NULL),
	_buffer(NULL)
{
	if (!data.IsEmpty())
	{
		_buffer = REBufferObject::CreateWithMemory((const void *)data.GetBytes(), data.GetSize());
	}
}

REJSONParser::REJSONParser(REBufferObject * buffer) :
	_rootObject(NULL),
	_buffer(NULL)
{
	if (buffer)
	{
		_buffer = buffer;
		_buffer->Retain();
	}
}

REJSONParser::REJSONParser(REObjectsDictionary * dictionary, REBufferObject * buffer) :
	_rootObject(NULL),
	_buffer(NULL)
{
	if (buffer)
	{
		_buffer = buffer;
		_buffer->Retain();
	}
}

REJSONParser::~REJSONParser()
{
	if (_rootObject)
	{
		_rootObject->Release();
	}
	
	if (_buffer)
	{
		_buffer->Release();
	}
}


