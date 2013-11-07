/*
 *   Copyright 2012 - 2013 Kulikov Oleg
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


#include "../../include/remedia/REAudioOpenAL.h"

#ifdef __RE_USING_OPENAL__

__RE_PUBLIC_CLASS_API__ ALCdevice * REAudioOpenAL::_alDevice = NULL;
__RE_PUBLIC_CLASS_API__ ALCcontext * REAudioOpenAL::_alContext = NULL;

ALCdevice * REAudioOpenAL::openAllDevices()
{
	ALCdevice * openedDevice = NULL;
	
	/* Checking for OpenAl ind extensions */
#if (defined(ALC_ALL_DEVICES_SPECIFIER) && defined(ALC_DEFAULT_ALL_DEVICES_SPECIFIER))
	if( alcIsExtensionPresent( 0, "ALC_ENUMERATE_ALL_EXT" ) )
	{
		const ALchar * pDeviceNames = alcGetString( 0, ALC_ALL_DEVICES_SPECIFIER );;
		const ALchar * pDefaultDevice = alcGetString( 0, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
		if( !strlen(pDeviceNames) )
		{
			return openedDevice;
		}
		else
		{
			if ( strlen(pDefaultDevice) > 0 )
			{
				openedDevice = alcOpenDevice( pDefaultDevice );
				if ( openedDevice != 0)
				{
					return openedDevice;
				}
			}
			
			while( pDeviceNames && *pDeviceNames )
			{
				openedDevice = alcOpenDevice( pDeviceNames );
				if ( openedDevice != 0)
				{
					return openedDevice;
				}
				pDeviceNames += strlen(pDeviceNames) + 1;
			}
		}
	}
#endif /* ALC_ALL_DEVICES_SPECIFIER && ALC_DEFAULT_ALL_DEVICES_SPECIFIER*/
	return openedDevice;
}

ALCdevice * REAudioOpenAL::openDefaultDevice()
{
	ALCdevice * openedDevice = NULL;
	
	/* Checking for OpenAl ind extensions */
#if (defined(ALC_DEVICE_SPECIFIER) && defined(ALC_DEFAULT_DEVICE_SPECIFIER))
	if( alcIsExtensionPresent( 0, "ALC_ENUMERATION_EXT" ) )
	{
		const ALchar * pDeviceNames = alcGetString( 0, ALC_DEVICE_SPECIFIER );
		const ALchar * pDefaultDevice = alcGetString( 0, ALC_DEFAULT_DEVICE_SPECIFIER );
		if( !strlen(pDeviceNames) )
		{
			return openedDevice;
		}
		else
		{
			if ( strlen(pDefaultDevice) > 0 )
			{
				openedDevice = alcOpenDevice( pDefaultDevice );
				if ( openedDevice != 0)
				{
					return openedDevice;
				}
			}
			while( pDeviceNames && *pDeviceNames )
			{
				openedDevice = alcOpenDevice( pDeviceNames );
				if ( openedDevice != 0)
				{
					return openedDevice;
				}
				pDeviceNames += strlen(pDeviceNames) + 1;
			}
		}
	}
#endif /* defined(ALC_DEVICE_SPECIFIER) && defined(ALC_DEFAULT_DEVICE_SPECIFIER) */
	return openedDevice;
}

ALCdevice * REAudioOpenAL::openDevice()
{
	ALCdevice * openedDevice = REAudioOpenAL::openAllDevices();
	if (openedDevice != NULL)
	{
		return openedDevice;
	}
	return REAudioOpenAL::openDefaultDevice();
}

REBOOL REAudioOpenAL::initDevice()
{
	// if no context exists
	if (alcGetCurrentContext() == 0)
	{
#if (defined(__RE_OS_WINDOWS__) || defined(__RE_OS_MACOSX__) || defined(__RE_OS_LINUX__))
		_alDevice = REAudioOpenAL::OpenDevice();
#endif
		
#ifdef __RE_OS_IPHONE__
		// Pass NULL to specify the system’s default output device
		_alDevice = alcOpenDevice(NULL);
#endif
		
		if (alcGetError(_alDevice) != ALC_NO_ERROR)
		{
			// Error open device
			alcCloseDevice(_alDevice);
			_alDevice = NULL;
			return false;
		}
		else
		{
			// Device open
			_alContext = alcCreateContext(_alDevice, 0);
			if (_alContext == NULL)
			{
				// Error create context
				alcCloseDevice(_alDevice);
				_alDevice = NULL;
				return false;
			}
			else if (alcGetError(_alDevice) != ALC_NO_ERROR)
			{
				alcDestroyContext(_alContext);
				alcCloseDevice(_alDevice);
				_alContext = NULL;
				_alDevice = NULL;
				return false;
			} 
			else if (alcMakeContextCurrent(_alContext) != ALC_TRUE) 
			{
				alcDestroyContext(_alContext);
				alcCloseDevice(_alDevice);
				_alContext = NULL;
				_alDevice = NULL;
				return false;
			}
			else if (alcGetError(_alDevice) != ALC_NO_ERROR) 
			{
				alcMakeContextCurrent(0);
				alcDestroyContext(_alContext);
				alcCloseDevice(_alDevice);
				_alContext = NULL;
				_alDevice = NULL;
				return false;
			}
			else
			{
				ALfloat listenerPos[] = { 0.0f, 0.0f, 0.0f };
				ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
				ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f };
				alListenerfv(AL_POSITION, listenerPos);
				alListenerfv(AL_VELOCITY, listenerVel);
				alListenerfv(AL_ORIENTATION, listenerOri);
				alListenerf(AL_GAIN, 1.0f /* _volume */ );
			}
		}

		return true;
	}	
	
	return false;
}

REAudioOpenAL::REAudioOpenAL()
{
	if ( (_alDevice == NULL) && (_alContext == NULL) ) 
	{
		REAudioOpenAL::initDevice();
	}
}

REAudioOpenAL::~REAudioOpenAL()
{
	
}







#endif /* __RE_USING_OPENAL__ */


