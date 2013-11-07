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


#include "../../include/recore/REResourcesStorage.h"
#include "../../include/recore/REZipReader.h"
#include "../../include/recore/REFileManager.h"
#include "../../include/recore/REFile.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REMutableString.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

class IREResourcesStorageContainer
{
public:
	virtual REBOOL readToBuffer(const REString & resourcePath, REBuffer * toBuffer) = 0;
	virtual REBOOL isExists(const REString & resourcePath) = 0;
	virtual ~IREResourcesStorageContainer() { }
};

class REZipArchiveResource : public REZipReader, public IREResourcesStorageContainer
{
public:
	virtual REBOOL readToBuffer(const REString & resourcePath, REBuffer * toBuffer)
	{
		REZipEntry * entry = this->getEntry(resourcePath);
		if (entry) 
		{
			return entry->read(toBuffer);
		}
		return false;
	}
	virtual REBOOL isExists(const REString & resourcePath)
	{
		REZipEntry * entry = this->getEntry(resourcePath);
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
	virtual REBOOL readToBuffer(const REString & resourcePath, REBuffer * toBuffer)
	{
		if (_rootPath)
		{
			REMutableString p(*_rootPath);
			p.appendPathComponent(resourcePath.UTF8String());
			REFile f(p, "rb");
			const REUInt32 fileSize = f.fileSize();
			if (fileSize) 
			{
				if (toBuffer->resize(fileSize, false)) 
				{
					if (f.fileRead(toBuffer->buffer(), fileSize))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	
	virtual REBOOL isExists(const REString & resourcePath)
	{
		if (_rootPath)
		{
			REMutableString p(*_rootPath);
			p.appendPathComponent(resourcePath.UTF8String());
			REFileManager m;
			REBOOL isDir = false;
			if (m.isFileExistsAtPath(p, &isDir))
			{
				return (!isDir);
			}
		}
		return false;
	}
	
	REBOOL isCanRead() const
	{
		return (_rootPath) ? true : false;
	}
	
	REDirectoryResource(const REString & path) : _rootPath(NULL)
	{
		REBOOL isDir = false;
		REFileManager m;
		if (m.isFileExistsAtPath(path, &isDir)) 
		{
			if (isDir && m.isReadableFileAtPath(path)) 
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
	typedef struct _callBacks
	{
		REBOOL (*ReadToBuf)(const char * filePath, REBuffer * toBuffer);
		REBOOL (*IsExists)(const char * filePath);
	} CallBacks;
	static REResourcesStoragePrivate::CallBacks staticCallBacks;
	static REBOOL addResourcesPath(const REString & resourcesPath);
	static REBOOL readToBuffer(const REString & resourcePath, REBuffer * toBuffer);
	static REBOOL isExists(const REString & resourcePath);
	static void clear();
};

REArray<IREResourcesStorageContainer *> REResourcesStoragePrivate::_resources;
REResourcesStoragePrivate::CallBacks REResourcesStoragePrivate::staticCallBacks = { 0 };


REBOOL REResourcesStoragePrivate::isExists(const REString & resourcePath)
{
	for (REUInt32 i = 0; i < _resources.count(); i++) 
	{
		IREResourcesStorageContainer * r = _resources[i];
		if (r->isExists(resourcePath))
		{
			return true;
		}
	}
	return false;
}

void REResourcesStoragePrivate::clear()
{
	for (REUInt32 i = 0; i < _resources.count(); i++) 
	{
		IREResourcesStorageContainer * r = _resources[i];
		delete r;
	}
	_resources.setCapacity(0);
}

REBOOL REResourcesStoragePrivate::addResourcesPath(const REString & resourcesPath)
{
	REBOOL isDir = false;
	REFileManager m;
	if (m.isFileExistsAtPath(resourcesPath, &isDir)) 
	{
		if (isDir) 
		{
			REDirectoryResource * dirRes = new REDirectoryResource(resourcesPath);
			if (dirRes) 
			{
				if (dirRes->isCanRead()) 
				{
					if (_resources.add(dirRes)) 
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
				if (zip->entriesCount() > 0) 
				{
					if (_resources.add(zip))
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

REBOOL REResourcesStoragePrivate::readToBuffer(const REString & resourcePath, REBuffer * toBuffer)
{
	for (REUInt32 i = 0; i < _resources.count(); i++) 
	{
		if (_resources[i]->readToBuffer(resourcePath, toBuffer))
		{
			return true;
		}
	}
	return false;
}



void REResourcesStorage::setReadToBufferCallBack(REBOOL (*ReadToBuf)(const char *, REBuffer *))
{
	REResourcesStoragePrivate::staticCallBacks.ReadToBuf = ReadToBuf;
}

void REResourcesStorage::setIsExistsCallBack(REBOOL (*IsExists)(const char *))
{
	REResourcesStoragePrivate::staticCallBacks.IsExists = IsExists;
}

void REResourcesStorage::clear()
{
	REResourcesStoragePrivate::clear();	
}

REResourcesStorage::REResourcesStorage()
{
	
}

REResourcesStorage::~REResourcesStorage()
{
	
}

REBOOL REResourcesStorage::readToBuffer(const REString & resourcePath, REBuffer * toBuffer)
{
	if (resourcePath.UTF8String() && toBuffer)
	{
		if (REResourcesStoragePrivate::staticCallBacks.ReadToBuf)
		{
			const char * p = resourcePath.UTF8String();
			if (REResourcesStoragePrivate::staticCallBacks.ReadToBuf(p, toBuffer))
			{
				return true;
			}
		}
		return REResourcesStoragePrivate::readToBuffer(resourcePath, toBuffer);			
	}
	return false;
}

REBOOL REResourcesStorage::isExists(const REString & resourcePath)
{
	if (resourcePath.length())
	{
		if (REResourcesStoragePrivate::staticCallBacks.IsExists)
		{
			const char * p = resourcePath.UTF8String();
			if (REResourcesStoragePrivate::staticCallBacks.IsExists(p))
			{
				return true;
			}
		}
		return REResourcesStoragePrivate::isExists(resourcePath);
	}
	return false;
}

REBOOL REResourcesStorage::addResourcesPath(const REString & resourcesPath)
{	
	return REResourcesStoragePrivate::addResourcesPath(resourcesPath);
}


