//
//  MyGameRootViewController.cpp
//  recoreTestApp
//
//  Created by Resident Evil on 16.11.12.
//  Copyright (c) 2012 n. All rights reserved.
//

#include "MyGameRootViewController.h"
#include "../../../../include/recore/REInt24.h"

class Thread1 : public REThread 
{
protected:	
	virtual void threadBody()
	{
		int y = 0;
		y += 6;
		for (int i = 0; i < 1000; i++) 
		{
			int y = 0;
			y += 6;
		}
	}
public:
	Thread1() : REThread()
	{
		int y = 0;
		y += 6;
	}
	virtual ~Thread1()
	{
		int y = 0;
		y += 6;
	}
};

void TestThreads()
{
//	Thread1 * th1 = new Thread1();
//	th1->setAutoReleaseWhenDone(true);
//	th1->start();
}

void testAddPathComp(const char * p1, const char * p2, const char * needRes)
{
	if (needRes)
	{
		if (strlen(needRes) == 0) needRes = NULL;
	}
	
	REMutableString ms3(p1);
	ms3.appendPathComponent(p2);
	const char * res = ms3.UTF8String();
	if (res && needRes)
	{
		if (strcmp(res, needRes) != 0) 
		{
			printf("ERROR testAddPathComp");
			int ok = 1;
			ok -= 1;
		}
	}
	else if (res == NULL && needRes == NULL)
	{
		int ok = 0;
		ok += 1;
	}
	else
	{
		printf("ERROR testAddPathComp");
		int ok = 1;
		ok -= 1;
	}
}


void testDelExt(const char * p1, const char * needRes)
{
	if (needRes)
	{
		if (strlen(needRes) == 0) needRes = NULL;
	}
	
	REMutableString ms3(p1);
	ms3.removePathExtension();
	const char * res = ms3.UTF8String();
	if (res && needRes)
	{
		if (strcmp(res, needRes) != 0) 
		{
			printf("ERROR testDelExt");
			int ok = 1;
			ok -= 1;
		}
	}
	else if (res == NULL && needRes == NULL)
	{
		int ok = 0;
		ok += 1;
	}
	else
	{
		printf("ERROR testDelExt");
		int ok = 1;
		ok -= 1;
	}
}


void testDelLastComp(const char * p1, const char * needRes)
{
	if (needRes)
	{
		if (strlen(needRes) == 0) needRes = NULL;
	}
	
	REMutableString ms3(p1);
	ms3.removeLastPathComponent();
	const char * res = ms3.UTF8String();
	if (res && needRes)
	{
		if (strcmp(res, needRes) != 0) 
		{
			printf("ERROR testDelLastComp");
			int ok = 1;
			ok -= 1;
		}
	}
	else if (res == NULL && needRes == NULL)
	{
		int ok = 0;
		ok += 1;
	}
	else
	{
		printf("ERROR testDelLastComp");
		int ok = 1;
		ok -= 1;
	}
}

void testPathExt(const char * p1, const char * needRes)
{
	if (needRes)
	{
		if (strlen(needRes) == 0) needRes = NULL;
	}
	
	REMutableString ms3(p1);
	REString ext = ms3.pathExtension();
	const char * res = ext.UTF8String();
	if (res && needRes)
	{
		if (strcmp(res, needRes) != 0)  
		{
			printf("ERROR testPathExt: in:%s res:%s expected:%s", 
				   p1 ? p1 : "NULL",
				   res ? res : "NULL",
				   needRes ? needRes : "NULL"
				   );
			int ok = 1;
			ok -= 1;
		}
	}
	else if (res == NULL && needRes == NULL)
	{
		int ok = 0;
		ok += 1;
	}
	else
	{
		printf("ERROR testPathExt: in:%s res:%s expected:%s", 
			   p1 ? p1 : "NULL",
			   res ? res : "NULL",
			   needRes ? needRes : "NULL"
			   );
		int ok = 1;
		ok -= 1;
	}
}


void Test1()
{
	testAddPathComp(NULL, NULL, NULL);
	testAddPathComp("", "", "");
	
	testAddPathComp("", "/Volumes/Data", "/Volumes/Data");
	testAddPathComp("", "/Volumes/Data/", "/Volumes/Data");
	testAddPathComp("", "\\Volumes\\Data", "/Volumes/Data");
	testAddPathComp("", "\\Volumes\\Data\\", "/Volumes/Data");
	
	testAddPathComp("/Volumes/Data", "", "/Volumes/Data");
	testAddPathComp("/Volumes/Data/", "", "/Volumes/Data");
	testAddPathComp("\\Volumes\\Data", "", "/Volumes/Data");
	testAddPathComp("\\Volumes\\Data\\", "", "/Volumes/Data");
	
	testAddPathComp("/Volumes/Data", "dir", "/Volumes/Data/dir");
	testAddPathComp("/Volumes/Data", "dir/", "/Volumes/Data/dir");
	testAddPathComp("/Volumes/Data", "/dir", "/Volumes/Data/dir");	
	testAddPathComp("/Volumes/Data", "/dir/", "/Volumes/Data/dir");	
	testAddPathComp("/Volumes/Data/", "dir", "/Volumes/Data/dir");
	testAddPathComp("/Volumes/Data/", "dir/", "/Volumes/Data/dir");
	testAddPathComp("/Volumes/Data/", "/dir", "/Volumes/Data/dir");	
	testAddPathComp("/Volumes/Data/", "/dir/", "/Volumes/Data/dir");	

	testAddPathComp("\\Volumes\\Data", "dir", "/Volumes/Data/dir");
	testAddPathComp("\\Volumes\\Data", "dir\\", "/Volumes/Data/dir");
	testAddPathComp("\\Volumes\\Data", "\\dir", "/Volumes/Data/dir");	
	testAddPathComp("\\Volumes\\Data", "\\dir\\", "/Volumes/Data/dir");	
	testAddPathComp("\\Volumes\\Data\\", "dir", "/Volumes/Data/dir");
	testAddPathComp("\\Volumes\\Data\\", "dir\\", "/Volumes/Data/dir");
	testAddPathComp("\\Volumes\\Data\\", "\\dir", "/Volumes/Data/dir");	
	testAddPathComp("\\Volumes\\Data\\", "\\dir\\", "/Volumes/Data/dir");	
	
	testAddPathComp("/Volumes/Data", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data/", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data", "/file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data/", "/file.txt", "/Volumes/Data/file.txt");
	
	testAddPathComp("\\Volumes\\Data", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data\\", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data", "\\file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data\\", "\\file.txt", "/Volumes/Data/file.txt");
	
	testAddPathComp("\\Volumes\\Data", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data\\", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data", "/file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("\\Volumes\\Data\\", "/file.txt", "/Volumes/Data/file.txt");
	
	testAddPathComp("/Volumes/Data", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data/", "file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data", "\\file.txt", "/Volumes/Data/file.txt");
	testAddPathComp("/Volumes/Data/", "\\file.txt", "/Volumes/Data/file.txt");
	
	testDelExt("", "");
	testDelExt(NULL, "");
	testDelExt("", NULL);
	testDelExt(NULL, NULL);
	testDelExt("/", "/");
	testDelExt("\\", "/");
	
	testDelExt("file.txt", "file");
	testDelExt("file.txt/", "file");
	testDelExt("file.txt\\", "file");
	testDelExt("file.txt/asd/", "file.txt/asd");
	testDelExt("file.txt\\asd\\", "file.txt/asd");
	
	testDelExt("qwert/file.txt", "qwert/file");
	testDelExt("qwert/file.txt/", "qwert/file");
	testDelExt("qwert\\file.txt\\", "qwert/file");
	testDelExt("qwert/file.txt/asd/", "qwert/file.txt/asd");
	testDelExt("qwert\\file.txt\\asd\\", "qwert/file.txt/asd");
	

	testDelLastComp("", "");
	testDelLastComp(NULL, NULL);
	testDelLastComp(NULL, "");
	testDelLastComp("", NULL);
	
	testDelLastComp("/", "/");
	testDelLastComp("\\", "/");
	
	testDelLastComp("file.txt", NULL);
	testDelLastComp("file.txt/", NULL);
	testDelLastComp("file.txt\\", NULL);
	testDelLastComp("file.txt/asd/", "file.txt");
	testDelLastComp("file.txt\\asd\\", "file.txt");
	
	testDelLastComp("qwert/file.txt", "qwert");
	testDelLastComp("qwert/file.txt/", "qwert");
	testDelLastComp("qwert\\file.txt\\", "qwert");
	testDelLastComp("qwert/file.txt/asd/", "qwert/file.txt");
	testDelLastComp("qwert\\file.txt\\asd\\", "qwert/file.txt");
	
	 
	testPathExt(NULL, NULL);
	testPathExt("", NULL);
	testPathExt("file", NULL);
	testPathExt("file.txt", "txt");
	testPathExt("file.txt\\", "txt");
	testPathExt("file.txt/", "txt");
	testPathExt("file.txt", "txt");
	testPathExt("file.txt\\", "txt");
	testPathExt("file.txt/", "txt");
	
	testPathExt("file.t", "t");
	testPathExt("file.t\\", "t");
	testPathExt("file.t/", "t");
	testPathExt("file.t", "t");
	testPathExt("file.t\\", "t");
	testPathExt("file.t/", "t");
	
	testPathExt("file.", NULL);
	testPathExt("file.\\", NULL);
	testPathExt("file./", NULL);
	testPathExt("file.", NULL);
	testPathExt("file.\\", NULL);
	testPathExt("file./", NULL);
	
	testPathExt("file.txt/fdg", NULL);
	testPathExt("file.txt\\asd", NULL);
	testPathExt("file.txt/asd", NULL);
	
	testPathExt("sad/file.txt/fdg", NULL);
	testPathExt("sdf\\file.txt\\asd", NULL);
	testPathExt("asd/file.txt/asd", NULL);
	
	testPathExt(".git", "git");
	testPathExt(".", NULL);
	testPathExt("..", NULL);
	
	REMutableString ms2;
	ms2.replace("Hello", "Buy");
	
	ms2 = "Hello Hello World !!!";
	ms2.replace("Hello ");
	ms2.replace("");
	
	ms2 = L"Привет Привет World";
	ms2.replace(L"Привет", L"Пока");
	
	REString strn1;
	REMutableString ms1 = strn1;
	REWideString sw1 = strn1;
	
	
	strn1 = ms1;
	strn1 = sw1;
	
	ms1 = sw1;
	sw1 = strn1;
	sw1 = ms1;

	strn1.setFromWideString(L"QыQ", 3, REStringTypeUTF8);
	sw1.setFromUTF8String("qwe", RENotFound, REStringTypeWide);
	
	REDate date1;
	REDate date2;
	
	date2 = date1;
	
	date1.setYear(2015);
	
	RETypedArray arr1;
	arr1.add(RETypedPtr(new RENumber((REUInt64)-1), REPtrTypeNumber));
	arr1.add(RETypedPtr(new RENumber((REUInt64)-2), REPtrTypeNumber));
	
	REDictionary dict;
	dict.setValue(RETypedPtr(new RENumber((REInt64)2), REPtrTypeNumber), "count");
	dict.setValue(RETypedPtr(new RENumber((REInt64)4), REPtrTypeNumber), "capacity");
	dict.setValue(RETypedPtr(new REString("some title"), REPtrTypeNumber), "title");
	
	RETypedArray keys1;
	keys1 = dict.allKeys();
	keys1 = dict.allValues();
	keys1.isEmpty();
}

void TestInt24()
{
	int intValue = 0;
	REInt24 v;
	v += 1;
	intValue = v;
	v = 0;
	intValue = v;
	
	v += 1;
	intValue = v;
	
	v -= 2;
	intValue = v;
	
	intValue = 0;
}

void TestREObject(REView * inObj)
{
	
}

void TestSRC32vcMD5()
{
	const char * str = "REDictionaryObject";
	RETimeInterval t1 = RETime::time();
	REUInt32 md5 = 0;
	for (int i = 0; i < 1000; i++) 
	{
		md5 = REMD5Generator::generateFromString(str);
	}
	t1 = RETime::time() - t1;
	
	
	RETimeInterval t2 = RETime::time();
	REUInt32 crc32 = 0;
	for (int i = 0; i < 1000; i++) 
	{
		 crc32 = RECRC32Generator::generateFromString(str);
	}
	t2 = RETime::time() - t2;
	
	RELog::log("REMD5Generator:   %f  %u", t1, md5);
	RELog::log("RECRC32Generator: %f  %u", t2, crc32);
}

void setFrame1(const char * type, ...)
{
	
}

void TestARGS()
{
	//RERect f;
	//setFrame1("frame", f);
}

void TestDate()
{
	REDate d1;
	REThread::uSleepInSeconds(2);
	REDate d2;
	
	RETimeInterval diff1 = d1.difference(d2);
	RETimeInterval diff2 = d1.minimumDifference(d2);
	
	diff1 = d2.difference(d1);
	diff2 = d2.minimumDifference(d1);
	
	if (diff1 == diff2) 
	{
		
	}
}

void TestURLConnection()
{
	REPtr<REBuffer> buff;
	REPtr<REURLResponse> responce;
	REBOOL isOK =  REURLConnection::sendRequest(REURLRequest::createWithURL(REURL("http://pagead2.googlesyndication.com/simgad/11459584590707166672")),
								 &buff, 
								 &responce);
	
	isOK = false;
}

void Test()
{
	RELocale * loc = RELocale::defaultLocale();
	//TestThreads();
	//return;
	
//	TestURLConnection();
	return;
	TestDate();
	TestARGS();
	TestSRC32vcMD5();
	TestInt24();
	
	REViewController * vc1 = REViewController::create();
	TestREObject(vc1);
	
	const int isCorrectTypes = RECore::isCorrectTypes();
	//Test1();
	//return;
	
	
	REData data;
	data.initFromPath("/Volumes/Data/Work/OKJSONParser/TestJSONFiles/test12.json");
	//data.initFromPath("/Volumes/WORK/Documents/OKJSONParser/TestJSONFiles/test1.json");
	REDictionary ddd1;
	ddd1.initializeFromJSONData(data.bytes(), data.size());
	
	REDictionaryObject * ddd2 = REDictionaryObject::create();
	ddd2->initializeFromJSONData(data.bytes(), data.size());
	ddd2->release();
	
	REMutableString json = ddd1.JSONString();
	const char * jsonStr = json.UTF8String();
	
	RETypedPtr p1(new REString("dasdsd"), REPtrTypeString);
	RETypedPtr p2 = p1;
	REString * str1 = p2.getString();
	str1->isEmpty();
	
	REBuffer buff;
	buff.set("0123456789", 10);
	
	RETypedPtr compressed = REZLIBCompression::compress(buff, 1.0f);
	compressed = REZLIBCompression::compress(buff, 0.0f);
	
	compressed.getType();
	compressed.getBuffer();
	
	REStringObject * strOjb1 = REStringObject::createWithChars("Hello");
	RETypedPtr p3(strOjb1, REPtrTypeVoidPointer);
	
	REStringObject * strOjb2 = (REStringObject *)p3.getVoidPointer();
	
	strOjb2 = 0;
	
	REDictionary dict;
	
	dict.setValue(RETypedPtr(new RENumber((REInt64)2), REPtrTypeNumber), "count");
	RETypedPtr val1 = dict.valueForKey("count");
	
	RENumber * n4 = val1.getNumber();
	dict.setValue(RETypedPtr(new RENumber((REInt64)-623463), REPtrTypeNumber), "count");
	val1 = dict.valueForKey("count");
	n4 = val1.getNumber();
	
	dict.setValue(RETypedPtr(), "count");
	
	n4 = NULL;
}

void MyGameRootViewController::render()
{
	REViewController::render();
}

void MyGameRootViewController::renderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
	REViewController::renderWithOffset(offsetX, offsetY);
}

MyGameRootViewController::MyGameRootViewController() : REViewController()
{
	Test();
}

MyGameRootViewController::~MyGameRootViewController()
{
	
}


