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


#include "../../include/regui/RETexture.h"

REUInt32 RETexture::openGLTexureIdentifier() const
{
#warning "REUInt32 RETexture::openGLTexureIdentifier() const"
	return 0;
}

void * RETexture::direct3DDevice9() const
{
#warning "void * RETexture::direct3DDevice9() const"
	return NULL;
}

void * RETexture::direct3DTexture9() const
{
#warning "void * RETexture::direct3DTexture9() const"
}

REBOOL RETexture::isNull() const
{
	return true;
}

REBOOL RETexture::isMipmaped() const
{
	return false;
}

REBOOL RETexture::update(const REUByte * pixelsData, 
					  const REImagePixelFormat pixelsFormat,
					  const REUInt32 width,
					  const REUInt32 height)
{
	return false;
}

const REBOOL RETexture::isBlended() const
{
	return false;
}

void RETexture::setFilterType(const RETextureFilterType filter)
{
	
}

const RETextureFilterType RETexture::filterType() const
{
	return RETextureFilterNone;
}

void RETexture::bind() const
{
	
}

RETexture::RETexture() : REObject()
{
	
}

RETexture::~RETexture()
{
	
}

RETexture * RETexture::create()
{
	RETexture * t = new RETexture();
	return t;
}


