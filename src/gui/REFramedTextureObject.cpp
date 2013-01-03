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

#if defined(__RE_USING_ADITIONAL_TINYXML_LIBRARY__)
#include "../addlibs/tinyxml.h"
#elif defined(__RE_USING_SYSTEM_TINYXML_LIBRARY__)
#include <tinyxml.h>
#else
#define __RE_NO_XML_PARSER_PRIVATE__
#endif


/// Returns 0 or 'RENotFound' if no frames avaiable.
const REUInt32 REFramedTextureObject::GetFirstFrameIndex() const
{
	if (_frames.Count()) 
	{
		return 0;
	}
	return RENotFound;
}

/// Returns 0 or 'RENotFound' if no frames avaiable.
const REUInt32 REFramedTextureObject::GetLastFrameIndex() const
{
	if (_frames.Count()) 
	{
		return (_frames.Count() - 1);
	}
	return RENotFound;
}

void REFramedTextureObject::REAnimationRespondentAnimationDidStep(const REFloat32 progress)
{
	const REUInt32 index = (REUInt32)_frameIndexParam;
	_frame.Set(_frames[index]);
}

/// Set current texture frame index.
/// Can be animated using REAnimation, call this method between REAnimation::Setup(...) and REAnimation::Execute().
REBOOL REFramedTextureObject::SetFrameIndex(const REUInt32 newIndex)
{
	if (newIndex < _frames.Count())
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
	if (REAnimation::IsSetuping()) 
	{
		if (newIndex < _frames.Count())
		{
			const REUInt32 currentIndex = (REUInt32)_frameIndexParam;
			if (newIndex != currentIndex) 
			{
				REAnimation::AddFloatParam(this, 
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
		REAnimation::StopAllAnimations(this, stopType, isNeedCallDelegate);
	}
}

/* REObject */
const REUInt32 REFramedTextureObject::GetClassIdentifier() const
{
	return REFramedTextureObject::ClassIdentifier();
}

const REUInt32 REFramedTextureObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REFramedTextureObject");
	return clasIdentif;
}

REBOOL REFramedTextureObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REFramedTextureObject::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("IREAnimationRespondent") == classIdentifier) ||
			RETextureObject::IsImplementsClass(classIdentifier));
}

REBOOL REFramedTextureObject::InitFramesForTexture(const char * data, REFramedTextureObject * texture)
{
#ifndef __RE_NO_XML_PARSER_PRIVATE__
	if (data)
	{
		TiXmlDocument doc;
		doc.Parse(data);
		
		if (doc.Error()) { return false; }
		
		TiXmlElement * root = doc.RootElement();
		if (root == NULL) { return false; }
		
		const char * rootVal = root->Value();
		if (rootVal)
		{
			if ((*rootVal) == 'c')
			{
				int readedCount = 0;
				RESize canvasSize(-1.0f, -1.0f);
				for (TiXmlAttribute * attrib = root->FirstAttribute(); attrib != NULL; attrib = attrib->Next())
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
				for (TiXmlElement * childElem = root->FirstChildElement(); childElem != NULL; childElem = childElem->NextSiblingElement())
				{
					const char * nodeValue = childElem->Value();
					if (nodeValue)
					{
						if ((*nodeValue) == 'f')
						{
							RERectStruct rect;
							readedCount = 0;
							for (TiXmlAttribute * attrib = childElem->FirstAttribute(); attrib != NULL; attrib = attrib->Next())
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
								texture->_frames.Add(rect);
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
	_frames.Clear();
	_frameIndexParam = 0.0f;
	if (path.IsEmpty()) 
	{
		return false;
	}

	REData xmlData;
	REString xmlPath(path);
	xmlPath.RemovePathExtension();
	xmlPath.Append(".xml");
	if (!xmlData.InitFromPath(xmlPath)) 
	{
		return false;	
	}
	
	if (!this->InitFrames((const char *)xmlData.GetBytes()))
	{
		return false;
	}
	
	xmlData.Clear();
	
	if (this->UpdateFromImageFilePath(path, this->GetFilterType())) 
	{
		if (_frames.Count()) 
		{
			this->REAnimationRespondentAnimationDidStep(0.0f);
		}
		return true;
	}
	
	_frames.Clear();
	return true;
}

/* IREXMLSerializable */
REBOOL REFramedTextureObject::AcceptStringParameter(const char * key, const char * value)
{
	if (strcmp(key, "path") == 0) 
	{
		return this->InitFramedTextureFromPath(REString(value));
	}
	return RETextureObject::AcceptStringParameter(key, value);
}

REFramedTextureObject::REFramedTextureObject() : RETextureObject(), 
	_frameIndexParam(0.0f),
	_animationsCount(0)
{
	
}

void REFramedTextureObject::OnReleased()
{
	if (this->IsAnimating())
	{
		REAnimation::StopAllAnimations(this, REAnimationStopTypeImmediately, false);
	}
	
	RETextureObject::OnReleased();
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



