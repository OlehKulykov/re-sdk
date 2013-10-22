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


#ifndef __REURLCONNECTIONCURLPRIVATE_H__
#define __REURLCONNECTIONCURLPRIVATE_H__


#include "../REURLConnection.h"
#include "../REThread.h"
#include "REMutableURLResponse.h"

#if defined(__RE_USING_ADITIONAL_CURL_LIBRARY__)
#include "../../../src/addlibs/curl.h"
#elif defined(__RE_USING_SYSTEM_CURL_LIBRARY__)
#include <curl/curl.h>
#endif


class __RE_PUBLIC_CLASS_API__ REURLConnectionCURLPrivate : public REURLConnection, public REThread
{
private:
	CURL * _curl;
	REMutableURLResponse * _mutableResponse;
	
	static size_t writeFunc(char * ptr, size_t size, size_t nmemb, void * userp)
	{
		REURLConnectionCURLPrivate * connection = (REURLConnectionCURLPrivate *)userp;
		const size_t needWriteSize = size * nmemb;
		if (connection->_receivedBuffer->append(ptr, (REUInt32)needWriteSize))
		{
			if (connection->_delegate) connection->_delegate->URLConnectionReceivedData(connection, ptr, (REUInt32)needWriteSize);
			return needWriteSize;
		}
		return 0;
	}
	
	static size_t writeHeaders(void * buffer, size_t size, size_t nmemb, void * userp)
	{
		REURLConnectionCURLPrivate * connection = (REURLConnectionCURLPrivate *)userp;
		const char * s = (const char *)buffer;
		switch (*s) 
		{
			case 'H':
			{
				float httpVersion = -1.0f;
				int statusCode = -1;
				if (sscanf(s, "HTTP/%f %i", &httpVersion, &statusCode) == 2) 
				{
					connection->_mutableResponse->setStatusCode(statusCode);
					connection->_mutableResponse->setHttpVersion(httpVersion);
				}
			}
				break;
			case 'C':
			{
				char contType[128] = { 0 };
				unsigned long long contLen = 0;
				if (sscanf(s, "Content-Type: %s", contType) == 1) 
				{
					connection->_mutableResponse->setMIMEType(contType);
				}
				else if (sscanf(s, "Content-Length: %llu", &contLen) == 1)
				{
					connection->_mutableResponse->setExpectedContentSize(contLen);
					connection->_mutableResponse->setIsExpectedContentSizeDetermined(true);
				}
			}
				break;
			case '\r':
			{
				if (strlen(s) == 2)
				{
					if (strncmp(s, "\r\n", 2) == 0)
					{
						connection->swapResponcesIfNeeded();
						if (connection->_delegate)
						{
							connection->_delegate->URLConnectionReceivedResponse(connection, connection->_mutableResponse);
						}
					}
				}
			}
				break;
			default:
			{
				
			}
				break;
		}
		const size_t needWriteSize = size * nmemb;
		return needWriteSize;
	}
	
protected:
	void swapResponcesIfNeeded()
	{
		if (_responce.isEmpty() && _mutableResponse)
		{
			_responce = REPtr<REURLResponse>(_mutableResponse);
			_mutableResponse = NULL;
		}
	}
 	
	REBOOL startCURL()
	{
		if (_curl) 
		{
			const CURLcode res = curl_easy_perform(_curl);
			curl_easy_cleanup(_curl);
			_curl = NULL;
			this->swapResponcesIfNeeded();
			if (_delegate) _delegate->URLConnectionFinishedLoading(this);
			if (res == CURLE_OK) 
			{
				return true;
			}
		}
		return false;
	}
	
	virtual void threadBody()
	{
		this->startCURL();
	}
	
public:	
	REBOOL initialize()
	{
		if (_curl) return true;
		if (_request.isEmpty()) return false;
		const char * urlString = _request->url().UTF8String();
		if (!urlString) return false;
		
		CURL * curl = curl_easy_init();
		if (!curl) return false;
		curl_easy_setopt(curl, CURLOPT_URL, urlString);
		if (_request->connectingTimeoutInterval() > 0.0) 
		{
			const long timeOut = (long)(1000.0 * _request->connectingTimeoutInterval());
			if (timeOut) 
			{
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeOut); 
			}
		}
		
		_receivedBuffer = REPtr<REBuffer>(new REBuffer());
		if (_receivedBuffer.isEmpty()) { curl_easy_cleanup(curl); return false; }
		
		REURLConnectionCURLPrivate * connection = this;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, REURLConnectionCURLPrivate::writeFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, connection);
		
		_responce.release();
		_mutableResponse = new REMutableURLResponse();
		if (!_mutableResponse) { curl_easy_cleanup(curl); return false; }
		
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, REURLConnectionCURLPrivate::writeHeaders);
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, connection);
		
		_mutableResponse->setURL(_request->url());
		
		switch (_request->HTTPMethod()) 
		{
			case REURLRequestGET: break;
			case REURLRequestPOST: curl_easy_setopt(curl, CURLOPT_POST, (long)1); break;	
			case REURLRequestPUT:
				//curl_easy_setopt(curl, CURLOPT_PUT, (long)1);
				/*CURLOPT_PUT
				 A parameter set to 1 tells the library to use HTTP PUT to transfer data. 
				 The data should be set with CURLOPT_READDATA and CURLOPT_INFILESIZE.
				 This option is deprecated and starting with version 7.12.1 you should instead use CURLOPT_UPLOAD. */
				break;
			default: break;
		}
		
		_curl = curl;
		return true;
	}
	
	virtual REBOOL startConnection()
	{
		if (this->initialize()) 
		{
			return this->startCURL();
		}
		return false;
	}
	
	REBOOL startCURLConnection(const REBOOL isThreaded)
	{
		if (this->initialize())
		{
			if (isThreaded) 
			{
				return this->start();
			}
			return this->startCURL();
		}
		return false;
	}
	
	REURLConnectionCURLPrivate() : REURLConnection(), REThread(),
		_curl(NULL),
		_mutableResponse(NULL)
	{
		
	}
	
	virtual ~REURLConnectionCURLPrivate()
	{
		if (_curl)
		{
			curl_easy_cleanup(_curl);
			_curl = NULL;
		}
		RE_SAFE_DELETE(_mutableResponse);
	}
};


#endif /* __REURLCONNECTIONCURLPRIVATE_H__ */

