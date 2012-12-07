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


#include "../../include/recore/REResourcesStorage.h"
#include "../../include/recore/REZipReader.h"
#include "../../include/recore/REFileManager.h"
#include "../../include/recore/REFile.h"

class IREResourcesStorageContainer
{
public:
	virtual REBOOL ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer) = 0;
	virtual REBOOL IsExists(const REString & resourcePath) = 0;
	virtual ~IREResourcesStorageContainer() { }
};

class REZipArchiveResource : public REZipReader, public IREResourcesStorageContainer
{
public:
	virtual REBOOL ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer)
	{
		REZipEntry * entry = this->GetEntry(resourcePath);
		if (entry) 
		{
			return entry->Read(toBuffer);
		}
		return false;
	}
	virtual REBOOL IsExists(const REString & resourcePath)
	{
		REZipEntry * entry = this->GetEntry(resourcePath);
		if (entry) 
		{
			return true;
		}
		return false;
	}
	REZipArchiveResource(const REString & path) : REZipReader(path) { }
	virtual ~REZipArchiveResource() { }
};

class REDirectoryResource : public IREResourcesStorageContainer
{
private:
	REString * _rootPath;
public:
	virtual REBOOL ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer)
	{
		if (_rootPath)
		{
			REString p(*_rootPath);
			p.AppendPathComponent(resourcePath.UTF8String());
			REFile f(p, "rb");
			const REUInt32 fileSize = f.GetFileSize();
			if (fileSize) 
			{
				if (toBuffer->Resize(fileSize, false)) 
				{
					if (f.FileRead(toBuffer->GetBuffer(), fileSize))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	
	virtual REBOOL IsExists(const REString & resourcePath)
	{
		if (_rootPath)
		{
			REString p(*_rootPath);
			p.AppendPathComponent(resourcePath.UTF8String());
			REFileManager m;
			REBOOL isDir = false;
			if (m.IsFileExistsAtPath(p, &isDir))
			{
				return (!isDir);
			}
		}
		return false;
	}
	
	REBOOL IsCanRead() const
	{
		return (_rootPath) ? true : false;
	}
	
	REDirectoryResource(const REString & path) : _rootPath(NULL)
	{
		REBOOL isDir = false;
		REFileManager m;
		if (m.IsFileExistsAtPath(path, &isDir)) 
		{
			if (isDir && m.IsReadableFileAtPath(path)) 
			{
				_rootPath = new REString(path);
			}
		}
	}
	
	virtual ~REDirectoryResource()
	{
		
	}
};

class REResourcesStoragePrivate
{
private:
	static REArray<IREResourcesStorageContainer *> _resources;
public:
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
	typedef struct _callBacks
	{
		REBOOL (*ReadToBuf)(const char * filePath, REBuffer * toBuffer);
		REBOOL (*IsExists)(const char * filePath);
	} CallBacks;
	static REResourcesStoragePrivate::CallBacks staticCallBacks;
#endif	
	static REBOOL AddResourcesPath(const REString & resourcesPath);
	static REBOOL ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer);
	static REBOOL IsExists(const REString & resourcePath);
	static void Clear();
};

REArray<IREResourcesStorageContainer *> REResourcesStoragePrivate::_resources;
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
REResourcesStoragePrivate::CallBacks REResourcesStoragePrivate::staticCallBacks = { 0 };
#endif	

REBOOL REResourcesStoragePrivate::IsExists(const REString & resourcePath)
{
	for (REUInt32 i = 0; i < _resources.Count(); i++) 
	{
		IREResourcesStorageContainer * r = _resources[i];
		if (r->IsExists(resourcePath))
		{
			return true;
		}
	}
	return false;
}

void REResourcesStoragePrivate::Clear()
{
	for (REUInt32 i = 0; i < _resources.Count(); i++) 
	{
		IREResourcesStorageContainer * r = _resources[i];
		delete r;
	}
	_resources.SetCapacity(0);
}

REBOOL REResourcesStoragePrivate::AddResourcesPath(const REString & resourcesPath)
{
	REBOOL isDir = false;
	REFileManager m;
	if (m.IsFileExistsAtPath(resourcesPath, &isDir)) 
	{
		if (isDir) 
		{
			REDirectoryResource * dirRes = new REDirectoryResource(resourcesPath);
			if (dirRes) 
			{
				if (dirRes->IsCanRead()) 
				{
					if (_resources.Add(dirRes)) 
					{
						return true; 
					}
				}
				delete dirRes;
			}
		}
		else 
		{
			REZipArchiveResource * zip = new REZipArchiveResource(resourcesPath);
			if (zip)
			{
				if (zip->GetEntriesCount() > 0) 
				{
					if (_resources.Add(zip))
					{
						return true;
					}
				}
				delete zip;
			}
		}
	}
	return false;
}

REBOOL REResourcesStoragePrivate::ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer)
{
	for (REUInt32 i = 0; i < _resources.Count(); i++) 
	{
		if (_resources[i]->ReadToBuffer(resourcePath, toBuffer))
		{
			return true;
		}
	}
	return false;
}



#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
void REResourcesStorage::SetReadToBufferCallBack(REBOOL (*ReadToBuf)(const char *, REBuffer *))
{
	REResourcesStoragePrivate::staticCallBacks.ReadToBuf = ReadToBuf;
}

void REResourcesStorage::SetIsExistsCallBack(REBOOL (*IsExists)(const char *))
{
	REResourcesStoragePrivate::staticCallBacks.IsExists = IsExists;
}
#endif	

void REResourcesStorage::Clear()
{
	REResourcesStoragePrivate::Clear();	
}

REResourcesStorage::REResourcesStorage()
{
	
}

REResourcesStorage::~REResourcesStorage()
{
	
}

REBOOL REResourcesStorage::ReadToBuffer(const REString & resourcePath, REBuffer * toBuffer)
{
	if (resourcePath.Length() && toBuffer)
	{
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
		if (REResourcesStoragePrivate::staticCallBacks.ReadToBuf)
		{
			const char * p = resourcePath.UTF8String();
			if (REResourcesStoragePrivate::staticCallBacks.ReadToBuf(p, toBuffer))
			{
				return true;
			}
		}
#endif
		return REResourcesStoragePrivate::ReadToBuffer(resourcePath, toBuffer);			
	}
	return false;
}

REBOOL REResourcesStorage::IsExists(const REString & resourcePath)
{
	if (resourcePath.Length())
	{
#ifdef __RE_USING_STATIC_CALLBACKS_FOR_RESOURCES_STORAGE__	
		if (REResourcesStoragePrivate::staticCallBacks.IsExists)
		{
			const char * p = resourcePath.UTF8String();
			if (REResourcesStoragePrivate::staticCallBacks.IsExists(p))
			{
				return true;
			}
		}
#endif		
		return REResourcesStoragePrivate::IsExists(resourcePath);
	}
	return false;
}

REBOOL REResourcesStorage::AddResourcesPath(const REString & resourcesPath)
{	
	return REResourcesStoragePrivate::AddResourcesPath(resourcesPath);
}


