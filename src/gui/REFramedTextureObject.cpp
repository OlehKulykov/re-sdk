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


#include "../../include/regui/REFramedTextureObject.h"
#include "../../include/regui/REAnimation.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REMutableString.h"

#if defined(__RE_USING_ADITIONAL_TINYXML_LIBRARY__)
#include "../addlibs/tinyxml.h"
using namespace tinyxml2;
#elif defined(__RE_USING_SYSTEM_TINYXML_LIBRARY__)
#include <tinyxml.h>
#else
#define __RE_NO_XML_PARSER_PRIVATE__
#endif


/// Returns 0 or 'RENotFound' if no frames avaiable.
const REUInt32 REFramedTextureObject::GetFirstFrameIndex() const
{
	if (_frames.count()) 
	{
		return 0;
	}
	return RENotFound;
}

/// Returns 0 or 'RENotFound' if no frames avaiable.
const REUInt32 REFramedTextureObject::GetLastFrameIndex() const
{
	if (_frames.count()) 
	{
		return (_frames.count() - 1);
	}
	return RENotFound;
}

void REFramedTextureObject::REAnimationRespondentAnimationDidStep(const REFloat32 progress)
{
	const REUInt32 index = (REUInt32)_frameIndexParam;
	_frame.set(_frames[index]);
}

/// Set current texture frame index.
/// Can be animated using REAnimation, call this method between REAnimation::Setup(...) and REAnimation::Execute().
REBOOL REFramedTextureObject::SetFrameIndex(const REUInt32 newIndex)
{
	if (newIndex < _frames.count())
	{
		const REUInt32 currentIndex = (REUInt32)_frameIndexParam;
		if (newIndex != currentIndex) 
		{
			_frameIndexParam = (REFloat32)newIndex;
			this->REAnimationRespondentAnimationDidStep(0.0f);
		}
		return true;
	}
	return false;
}

REBOOL REFramedTextureObject::SetFrameIndexAnimated(const REUInt32 newIndex)
{
	if (REAnimation::isSetuping()) 
	{
		if (newIndex < _frames.count())
		{
			const REUInt32 currentIndex = (REUInt32)_frameIndexParam;
			if (newIndex != currentIndex) 
			{
				REAnimation::addFloatParam(this, 
											   &_animationsCount, 
											   &_frameIndexParam, 
											   (REFloat32)currentIndex, 
											   (REFloat32)newIndex);
			}
			return true;
		}
	}
	else
	{
		return this->SetFrameIndex(newIndex);
	}
	return false;
}

/// Stopes all animation of view with stop type.
/// Value of parameter 'isStopWithSubviews' indicates that need stop subviews animation.
/// Value of parameter 'isNeedCallDelegate' indicates than need inform animation delegate about stoping animation.
void REFramedTextureObject::StopAnimation(const REAnimationStopType stopType, const REBOOL isNeedCallDelegate)
{
	if (this->IsAnimating())
	{
		REAnimation::stopAllAnimations(this, stopType, isNeedCallDelegate);
	}
}

/* REObject */
const REUInt32 REFramedTextureObject::getClassIdentifier() const
{
	return REFramedTextureObject::classIdentifier();
}

const REUInt32 REFramedTextureObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REFramedTextureObject");
	return clasIdentif;
}

REBOOL REFramedTextureObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REFramedTextureObject::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("IREAnimationRespondent") == classIdentifier) ||
			RETextureObject::isImplementsClass(classIdentifier));
}

REBOOL REFramedTextureObject::InitFramesForTexture(const char * data, REFramedTextureObject * texture)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	if (data)
	{
		XMLDocument doc;
		doc.Parse(data);
		
		if (doc.Error()) { return false; }
		
		XMLElement * root = doc.RootElement();
		if (root == NULL) { return false; }
		
		const char * rootVal = root->Value();
		if (rootVal)
		{
			if ((*rootVal) == 'c')
			{
				int readedCount = 0;
				RESize canvasSize(-1.0f, -1.0f);
				for (const XMLAttribute * attrib = root->FirstAttribute(); attrib != NULL; attrib = attrib->Next())
				{
					const char * name = attrib->Name();
					const char * value = attrib->Value();
					if (name && value)
					{
						if ((*name) == 'w')
						{
							float v = 0.0f;
							if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; canvasSize.width = v; } }
						}
						else if ((*name) == 'h')
						{
							float v = 0.0f;
							if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; canvasSize.height = v; } }
						}
					}
				}
				if (readedCount != 2)
				{
					return false;
				}
				const REFloat32 xWidthKoef = 1.0f / canvasSize.width;
				const REFloat32 yHeightKoef = 1.0f / canvasSize.height;
				for (XMLElement * childElem = root->FirstChildElement(); childElem != NULL; childElem = childElem->NextSiblingElement())
				{
					const char * nodeValue = childElem->Value();
					if (nodeValue)
					{
						if ((*nodeValue) == 'f')
						{
							RERectStruct rect;
							readedCount = 0;
							for (const XMLAttribute * attrib = childElem->FirstAttribute(); attrib != NULL; attrib = attrib->Next())
							{
								const char * name = attrib->Name();
								const char * value = attrib->Value();
								if (name && value)
								{
									if ((*name) == 'x')
									{
										float v = 0.0f;
										if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; rect.x = v; } }
									}
									else if ((*name) == 'y')
									{
										float v = 0.0f;
										if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; rect.y = v; } }
									}
									else if ((*name) == 'w')
									{
										float v = 0.0f;
										if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; rect.width = v; } }
									}
									else if ((*name) == 'h')
									{
										float v = 0.0f;
										if (sscanf(value, "%f", &v) == 1) { if (v > 0.0f) { readedCount++; rect.height = v; } }
									}
								}
							}
							if (readedCount == 4)
							{
								rect.x *= xWidthKoef;
								rect.width *= xWidthKoef;
								rect.y *= yHeightKoef;
								rect.height *= yHeightKoef;
								texture->_frames.add(rect);
							}
						}
					}
				}
				return true;
			}
		}
	}
#endif
	return false;
}

REBOOL REFramedTextureObject::InitFrames(const char * data)
{
	return REFramedTextureObject::InitFramesForTexture(data, this);
}

REBOOL REFramedTextureObject::InitFramedTextureFromPath(const REString & path)
{
	_frames.clear();
	_frameIndexParam = 0.0f;
	if (path.isEmpty()) 
	{
		return false;
	}

	REData xmlData;
	REMutableString xmlPath(path);
	xmlPath.removePathExtension();
	xmlPath.append(".xml");
	if (!xmlData.initFromPath(xmlPath)) 
	{
		return false;	
	}
	
	if (!this->InitFrames((const char *)xmlData.bytes()))
	{
		return false;
	}
	
	xmlData.clear();
	
	if (this->UpdateFromImageFilePath(path, this->GetFilterType())) 
	{
		if (_frames.count()) 
		{
			this->REAnimationRespondentAnimationDidStep(0.0f);
		}
		return true;
	}
	
	_frames.clear();
	return true;
}

/* IREXMLSerializable */
REBOOL REFramedTextureObject::acceptStringParameter(const char * key, const char * value)
{
	if (strcmp(key, "path") == 0) 
	{
		return this->InitFramedTextureFromPath(REString(value));
	}
	return RETextureObject::acceptStringParameter(key, value);
}

REFramedTextureObject::REFramedTextureObject() : RETextureObject(), 
	_frameIndexParam(0.0f),
	_animationsCount(0)
{
	
}

void REFramedTextureObject::onReleased()
{
	if (this->IsAnimating())
	{
		REAnimation::stopAllAnimations(this, REAnimationStopTypeImmediately, false);
	}
	
	RETextureObject::onReleased();
}

REFramedTextureObject::~REFramedTextureObject()
{
	
}

/// Creates and returns new empty 'REFramedTextureObject' or NULL on error.
REFramedTextureObject * REFramedTextureObject::Create()
{
	REFramedTextureObject * newTexture = new REFramedTextureObject();
	if (newTexture) 
	{
		return newTexture;
	}
	return NULL;
}

/// Creates and returns new 'REFramedTextureObject' initialized with image data from 'image' and
/// frames from xml file with same file name.
/// File path 'filePath' must be path to texture image and for xml frames file extension should differ only
/// Example: texture path = <folder/textureImage.png> and xml file must be: <folder/textureImage.xml>
REFramedTextureObject * REFramedTextureObject::CreateWithFilePath(const REString & filePath, 
																  const RETextureFilterType filterType)
{
	REFramedTextureObject * newTexture = new REFramedTextureObject();
	if (newTexture) 
	{
		if (newTexture->InitBlankTexture(REImagePixelFormatR8G8B8, 4, 4, filterType)) 
		{
			newTexture->SetFilterType(filterType);
			if (newTexture->InitFramedTextureFromPath(filePath))
			{
				return newTexture;
			}
		}
		delete newTexture;
	}
	return NULL;
}



