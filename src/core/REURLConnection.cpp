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


#include "../../include/recore/REURLConnection.h"
#include "../../include/recore/REMD5Generator.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif



#ifndef __RE_RECORE_NO_LOAD_FROM_URL__

#if defined(HAVE_CURL_CURL_H)
/* HAVE_CURL_CURL_H */
#include <curl/curl.h>

#ifndef __RE_USING_CURL_LIBRARY__
#define __RE_USING_CURL_LIBRARY__ 1
#endif

#else 

#include "../addlibs/curl.h"
#ifndef __RE_USING_CURL_LIBRARY__
#define __RE_USING_CURL_LIBRARY__ 1
#endif

#endif 


#endif /* __RE_RECORE_NO_LOAD_FROM_URL__ */



#ifdef __RE_USING_CURL_LIBRARY__
#include "../../include/recore/private/REURLConnectionCURLPrivate.h"
#endif



IREURLConnectionDelegate * REURLConnection::delegate() const
{
	return _delegate;
}

void REURLConnection::setDelegate(IREURLConnectionDelegate * newDelegate)
{
	_delegate = newDelegate;
}

REBOOL REURLConnection::startConnection()
{
	return false;
}

const REPtr<REURLRequest> & REURLConnection::request() const
{
	return _request;
}

const REPtr<REURLResponse> & REURLConnection::responce() const
{
	return _responce;
}

const REPtr<REBuffer> & REURLConnection::receivedBuffer() const
{
	return _receivedBuffer;
}

REURLConnection::REURLConnection() :
	_delegate(NULL)
{
	
}
REURLConnection::~REURLConnection()
{
	
}

REPtr<REURLConnection> REURLConnection::createWithRequestAndDelegate(REPtr<REURLRequest> request,
																	 IREURLConnectionDelegate * delegate)
{
#ifdef __RE_USING_CURL_LIBRARY__	
	REURLConnectionCURLPrivate * con = new REURLConnectionCURLPrivate();
	if (con) 
	{
		con->_request = request;
		con->_delegate = delegate;
		return REPtr<REURLConnection>(con);
	}
#endif	
	return REPtr<REURLConnection>();
}

REBOOL REURLConnection::sendRequest(REPtr<REURLRequest> request, 
									REPtr<REBuffer> * receivedBuffer, 
									REPtr<REURLResponse> * responce)
{
#ifdef __RE_USING_CURL_LIBRARY__	
	REPtr<REURLConnectionCURLPrivate> con(new REURLConnectionCURLPrivate());
	if (con.isNotEmpty()) 
	{
		con->_request = request;
		if (con->initialize()) 
		{
			if (con->startCURLConnection(false)) 
			{
				if (receivedBuffer) 
				{
					(*receivedBuffer) = con->receivedBuffer();
				}
				if (responce)
				{
					(*responce) = con->responce();
				}
				return true;
			}
			return true;
		}
	}
#endif	
	return false;
}



//class REURLConnectionCURLPrivate 
//{
//public:
//#ifdef __RE_USING_CURL_LIBRARY__	
//	CURL * curl;
//	REURLResponseObjectPrivate * responce;
//#endif	
//	REURLRequestObject * request; 
//	IREURLConnectionDelegate * delegate;
//	REBufferObject * receiveBuffer;
//	REURLConnectionObject * connection;
//	REBOOL isHeadersWrited;
//	REBOOL isNeedDelegateHeaders;
//	REBOOL isCanDelegateWithConnection;
//	REBOOL isWorking;
//	
//	REBOOL Init();
//	REBOOL Run();
//	
//	REURLConnectionCURLPrivate();
//	~REURLConnectionCURLPrivate();
//	
//	static size_t WriteFunc(char * ptr, size_t size, size_t nmemb, void * userData);
//	static size_t WriteHeaders(void * buffer, size_t size, size_t nmemb, void * userData);
//};
//
//REBOOL REURLConnectionCURLPrivate::Init()
//{
//#ifdef __RE_USING_CURL_LIBRARY__	
//	REURLObject * url = request->GetURL();
//	if (url) 
//	{
//		const char * urlString = url->UTF8String();
//		curl = curl_easy_init();
//		if (curl && urlString) 
//		{
//			curl_easy_setopt(curl, CURLOPT_URL, urlString);
//			if (request->GetConnectingTimeoutInterval() > 0.0) 
//			{
//				const long timeOut = (long)(1000.0 * request->GetConnectingTimeoutInterval());
//				if (timeOut) { curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeOut); }
//			}
//			if (receiveBuffer) 
//			{
//				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, REURLConnectionCURLPrivate::WriteFunc);
//				curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
//			}
//			if (responce) 
//			{
//				curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, REURLConnectionCURLPrivate::WriteHeaders);
//				curl_easy_setopt(curl, CURLOPT_WRITEHEADER, this);
//				responce->SetURL(url);
//				if (responce->GetSuggestedFileName().IsEmpty()) 
//				{
//					REString fileName(url->GetFileNameWithExtension());
//					if (!fileName.IsEmpty()) 
//					{
//						responce->SetSuggestedFileName(fileName);
//					}
//				}
//			}
//			switch (request->GetHTTPMethod()) 
//			{
//				case REURLRequestGET: 
//					break;
//				case REURLRequestPOST: 
//					curl_easy_setopt(curl, CURLOPT_POST, (long)1); 
//					break;
//				case REURLRequestPUT:
//					/*CURLOPT_PUT
//					 A parameter set to 1 tells the library to use HTTP PUT to transfer data. The data should be set with CURLOPT_READDATA and CURLOPT_INFILESIZE.
//					 This option is deprecated and starting with version 7.12.1 you should instead use CURLOPT_UPLOAD. */
//					break;
//				default: break;
//			}
//			return true;
//		}
//		
//		curl_easy_cleanup(curl);
//		curl = NULL;
//	}
//#endif	
//	return false;
//}
//
//REBOOL REURLConnectionCURLPrivate::Run()
//{
//	isWorking = true;
//#ifdef __RE_USING_CURL_LIBRARY__	
//	if (curl) 
//	{
//		isCanDelegateWithConnection = ((delegate != NULL) && (connection != NULL));
//		CURLcode res = curl_easy_perform(curl);
//		curl_easy_cleanup(curl);
//		curl = NULL;
//		if (res == CURLE_OK) 
//		{
//			isWorking = false;
//			return true;
//		}
//	}
//#endif	
//	isWorking = false;
//	return false;
//}
//
//REURLConnectionCURLPrivate::REURLConnectionCURLPrivate() :
//#ifdef __RE_USING_CURL_LIBRARY__
//curl(NULL),
//responce(NULL),
//#endif
//request(NULL),
//delegate(NULL),
//receiveBuffer(NULL),
//connection(NULL),
//isHeadersWrited(false),
//isNeedDelegateHeaders(true),
//isCanDelegateWithConnection(false),
//isWorking(false)
//{
//	
//}
//
//REURLConnectionCURLPrivate::~REURLConnectionCURLPrivate()
//{
//#ifdef __RE_USING_CURL_LIBRARY__	
//	if (curl) 
//	{
//		curl_easy_cleanup(curl); 
//		curl = NULL;
//	}
//#endif	
//}
//
//size_t REURLConnectionCURLPrivate::WriteFunc(char * ptr, size_t size, size_t nmemb, void * userData)
//{
//#ifdef __RE_USING_CURL_LIBRARY__	
//	REURLConnectionCURLPrivate * con = (REURLConnectionCURLPrivate *)userData;
//	if (con->isNeedDelegateHeaders) 
//	{
//		con->isNeedDelegateHeaders = false;
//		if (con->isCanDelegateWithConnection) 
//		{
//			con->delegate->URLConnectionReceiveResponse(con->connection, con->responce);
//		}
//	}
//#endif
//	
//	const size_t needWriteSize = size * nmemb;
//	
//#ifdef __RE_USING_CURL_LIBRARY__	
//	con->receiveBuffer->Append(ptr, (REUInt32)needWriteSize);
//	if (con->isCanDelegateWithConnection) 
//	{
//		con->delegate->URLConnectionReceiveData(con->connection, ptr, (REUInt32)needWriteSize);
//	}
//#endif	
//	
//	return needWriteSize;
//}
//
//size_t REURLConnectionCURLPrivate::WriteHeaders(void * buffer, size_t size, size_t nmemb, void * userData)
//{
//	REURLConnectionCURLPrivate * con = (REURLConnectionCURLPrivate *)userData;
//	con->isHeadersWrited = true;
//	
//	const size_t needWriteSize = size * nmemb;
//	const char * s = (const char *)buffer;
//	switch (*s) 
//	{
//		case 'H':
//		{
//			float f = -1.0f;
//			int i = -1;
//			if (sscanf(s, "HTTP/%f %i", &f, &i) == 2) 
//			{
//				if (i >= 0) 
//				{
//#ifdef __RE_USING_CURL_LIBRARY__					
//					con->responce->SetStatusCode((REInt32)i); 
//#endif					
//				}
//			}
//		}
//			break;
//		case 'C':
//		{
//			char contType[128] = { 0 };
//			unsigned long long contLen = 0;
//			if (sscanf(s, "Content-Type: %s", contType) == 1) 
//			{
//#ifdef __RE_USING_CURL_LIBRARY__				
//				con->responce->SetMIMEType(contType); 
//#endif				
//			}
//			else if (sscanf(s, "Content-Length: %ull", &contLen) == 1)
//			{
//#ifdef __RE_USING_CURL_LIBRARY__				
//				con->responce->SetExpectedContentSize((REUInt32)contLen);
//				con->responce->SetIsExpectedContentSizeDetermined(true);
//#endif				
//			}
//		}
//			break;
//		default:
//			break;
//	}
//	//printf("\n%s", s);
//	
//	return needWriteSize;
//}
//
//REBufferObject * REURLConnectionObject::GetReceivedDataBuffer() const
//{
//	return _receivedDataBuffer;
//}
//
//REURLRequestObject * REURLConnectionObject::GetRequest() const
//{
//	return _request;
//}
//
//const REUInt32 REURLConnectionObject::GetClassIdentifier() const
//{
//	return REURLConnectionObject::ClassIdentifier();
//}
//
//const REUInt32 REURLConnectionObject::ClassIdentifier()
//{
//	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REURLConnectionObject");
//	return clasIdentif;
//}
//
//#if 0
//#ifndef __RE_OS_IPHONE__
//
//
////TODO:
////REURLConnectionObject::REURLConnectionObject(REURLRequestObject * request, IREURLConnectionDelegate * delegate);
////REURLConnectionObject::~REURLConnectionObject();
////REBOOL REURLConnectionObject::Start();
////REBOOL REURLConnectionObject::Stop();
//
//#endif /* __RE_OS_IPHONE__  */
//#endif /* 0 */
//
//#ifdef __RE_USING_CURL_LIBRARY__
//static CURL * CreateCURL(REURLRequestObject * request, 
//						 IREURLConnectionDelegate * delegate, 
//						 REBufferObject * downloadedBuffer, 
//						 REURLResponseObject ** responce)
//{
//	if (responce) { *responce = NULL; }
//	if (request) 
//	{
//		REURLObject * url = request->GetURL();
//		if (url) 
//		{
//			const char * urlString = url->UTF8String();
//			CURL * curl = curl_easy_init();
//			if (curl && urlString) 
//			{
//				curl_easy_setopt(curl, CURLOPT_URL, urlString);
//				if (request->GetConnectingTimeoutInterval() > 0.0) 
//				{
//					const long timeOut = (long)(1000.0 * request->GetConnectingTimeoutInterval());
//					if (timeOut) { curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeOut); }
//				}
//				if (downloadedBuffer) 
//				{
//					downloadedBuffer->Clear();
//					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, REURLCURLCallbacksPrivate::WriteFuncToREBuffer);
//					curl_easy_setopt(curl, CURLOPT_WRITEDATA, downloadedBuffer);
//				}
//				REURLResponseObjectPrivate * resp = NULL;
//				if (responce) 
//				{
//					resp = new REURLResponseObjectPrivate();
//					if (resp) 
//					{
//						curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, REURLCURLCallbacksPrivate::WriteHeaders);
//						curl_easy_setopt(curl, CURLOPT_WRITEHEADER, resp);
//						*responce = resp;
//						resp->SetURL(url);
//					}
//				}
//				
//				switch (request->GetHTTPMethod()) 
//				{
//					case REURLRequestGET: break;
//					case REURLRequestPOST: curl_easy_setopt(curl, CURLOPT_POST, (long)1); break;	
//					case REURLRequestPUT:
//						/*CURLOPT_PUT
//						 A parameter set to 1 tells the library to use HTTP PUT to transfer data. The data should be set with CURLOPT_READDATA and CURLOPT_INFILESIZE.
//						 This option is deprecated and starting with version 7.12.1 you should instead use CURLOPT_UPLOAD. */
//						break;
//					default: break;
//				}
//				return curl;
//			}
//			curl_easy_cleanup(curl);
//			curl = NULL;
//		}
//	}
//	
//	return NULL;
//}
//#endif
//
//void REURLConnectionObject::StartConnection()
//{
//	this->Clean();
//	
//	REBOOL isOk = false;
//	if (_request) 
//	{
//		REURLConnectionCURLPrivate * c = new REURLConnectionCURLPrivate();
//		if (c) 
//		{
//			_reserved = c;
//			c->connection = this;
//			c->request = _request;
//			c->delegate = _delegate;
//			REBufferObject * buff = REBufferObject::Create();
//			c->receiveBuffer = buff;
//			_receivedDataBuffer = buff;
//			if (_delegate)
//			{
//#ifdef __RE_USING_CURL_LIBRARY__				
//				REURLResponseObjectPrivate * resp = new REURLResponseObjectPrivate();
//				_responce = resp;
//				c->responce = resp;
//#endif				
//			}
//			if (c->Init()) 
//			{
//				isOk = c->Run();
//			}
//			
//			delete c;
//			_reserved = NULL;
//		}
//	}
//	
//	if (_delegate) 
//	{
//		if (isOk) { _delegate->URLConnectionFinishLoading(this); }
//		else { _delegate->URLConnectionFail(this); }
//	}
//	
//}
//
//void REURLConnectionObject::StartConnection(REBOOL isThreaded)
//{
//	if (isThreaded) 
//	{
//		this->Start();
//	}
//	else
//	{
//		this->StartConnection();
//	}
//}
//
//void REURLConnectionObject::ThreadBody()
//{
//	this->StartConnection();
//}
//
//REURLConnectionObject::REURLConnectionObject(REURLRequestObject * request, IREURLConnectionDelegate * delegate) : REThread(),
//	_request(request),
//	_delegate(delegate),
//	_receivedDataBuffer(NULL),
//	_reserved(NULL),
//	_responce(NULL)
//{
//	if (_request) 
//	{
//		_request->Retain();
//	}
//}
//
//void REURLConnectionObject::Clean()
//{
//	RE_SAFE_RELEASE(_receivedDataBuffer);
//	RE_SAFE_RELEASE(_responce);
//	
//	if (_reserved) 
//	{
//		REURLConnectionCURLPrivate * c = (REURLConnectionCURLPrivate *)_reserved;
//		delete c;
//	}
//}
//
//REURLConnectionObject::~REURLConnectionObject()
//{
//	this->Clean();
//	
//	RE_SAFE_RELEASE(_request);
//}
//
//REBOOL REURLConnectionObject::SendRequest(REURLRequestObject * request, REBuffer * downloadedBufferData, REURLResponseObject ** response)
//{
//	if (response) 
//	{
//		*response = NULL;
//	}
//#ifdef __RE_USING_CURL_LIBRARY__	
//	if (request) 
//	{
//		REURLObject * url = request->GetURL();
//		if (url) 
//		{
//			const char * urlString = url->UTF8String();
//			CURL * curl = curl_easy_init();
//			if (curl && urlString) 
//			{
//				curl_easy_setopt(curl, CURLOPT_URL, urlString);
//				if (request->GetConnectingTimeoutInterval() > 0.0) 
//				{
//					const long timeOut = (long)(1000.0 * request->GetConnectingTimeoutInterval());
//					if (timeOut) { curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeOut); }
//				}
//				
//				if (downloadedBufferData) 
//				{
//					downloadedBufferData->Clear();
//					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, REURLCURLCallbacksPrivate::WriteFuncToREBuffer);
//					curl_easy_setopt(curl, CURLOPT_WRITEDATA, downloadedBufferData);
//				}
//				
//				if (request->IsFollowRedirects()) 
//				{
//					curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)1);
//				}
//				else
//				{
//					curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)0);
//				}
//				
//				REURLResponseObjectPrivate * resp = NULL;
//				if (response) 
//				{
//					resp = new REURLResponseObjectPrivate();
//					if (resp) 
//					{
//						curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, REURLCURLCallbacksPrivate::WriteHeaders);
//						curl_easy_setopt(curl, CURLOPT_WRITEHEADER, resp);
//						*response = resp;
//						resp->SetURL(url);
//					}
//				}
//				
//				switch (request->GetHTTPMethod()) 
//				{
//					case REURLRequestGET:
//						break;
//					case REURLRequestPOST:
//						curl_easy_setopt(curl, CURLOPT_POST, (long)1);
//						/*
//						*/
//						break;	
//					case REURLRequestPUT:
//						curl_easy_setopt(curl, CURLOPT_PUT, (long)1);
//						/*
//						 CURLOPT_PUT
//						 A parameter set to 1 tells the library to use HTTP PUT to transfer data. The data should be set with CURLOPT_READDATA and CURLOPT_INFILESIZE.
//						 This option is deprecated and starting with version 7.12.1 you should instead use CURLOPT_UPLOAD. 
//						 */
//						break;
//					case REURLRequestDELETE:
//						curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); 
//						/*
//						*/
//						break;
//					default:
//						break;
//				}
//				
//				CURLcode res = curl_easy_perform(curl);
//				curl_easy_cleanup(curl);
//				curl = NULL;
//				if (res == CURLE_OK) 
//				{
//					if (resp) 
//					{
//						if (resp->GetSuggestedFileName().IsEmpty()) 
//						{
//							REString fileName(url->GetFileNameWithExtension());
//							if (!fileName.IsEmpty()) 
//							{
//								resp->SetSuggestedFileName(fileName);
//							}
//						}
//					}
//					return true;
//				}
//			}
//		}
//	}
//#endif	
//	return false;
//}
//
//REURLConnectionObject * REURLConnectionObject::CreateWithRequestAndDelegate(REURLRequestObject * request, IREURLConnectionDelegate * delegate)
//{
//	if (request) 
//	{
//		REURLConnectionObject * c = new REURLConnectionObject(request, delegate);
//		return c;
//	}
//	return NULL;
//}
//
//
//
//
//
