/*
 * Copyright (c) 2002-2006 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * REPRODUCTION IN WHOLE OR IN PART IS PROHIBITED WITHOUT THE WRITTEN
 * CONSENT OF THE COPYRIGHT OWNER.
 *
 */

/* Revision 1.2.12 */

//#include "StdAfx.H"
#include "Log.h"

#ifndef ENGINE_FLASH

//Engine::CLog::CLog()
//{
//        m_pLogData = NULL;
//    m_pLogFile = NULL;
//    m_bLock = false;
//}
//
//Engine::CLog::~CLog()
//{
//#ifndef __RE_OS_ANDROID__
//        m_Sections.clear();
//#endif
//}
//
//bool Engine::CLog::Init(const char *pLogFileName, bool bReOpen)
//{
//    m_Ident = 0;
//
//    m_pLogFileName = pLogFileName;
//    m_bReOpen = bReOpen;
//
//    if (!pLogFileName)
//    {
//        m_bLock = true;
//                return true;
//    }
//
//    m_pLogFile = fopen(m_pLogFileName, "wt");
//
//    if (!m_pLogFile)
//        return false;
//
//    if (m_bReOpen)
//    {
//        fclose(m_pLogFile);
//        m_pLogFile = NULL;
//    }
//
//    m_LogDataSize = LOG_DEFAULT_DATA_SIZE;
//    m_pLogData = (char *) malloc(sizeof(char) * m_LogDataSize);
//
//    return true;
//}
//
//void Engine::CLog::Done()
//{
//#ifndef __RE_OS_ANDROID__
//        m_Sections.clear();
//#endif
//	
//    if (m_pLogData)
//    {
//        free(m_pLogData);
//        m_pLogData = NULL;
//    }
//
//    if (m_pLogFile)
//    {
//        fclose(m_pLogFile);
//        m_pLogFile = NULL;
//    }
//}
//
//void Engine::CLog::Ident()
//{
//    if (!m_pLogFile)
//        return;
//
//#ifdef _WIN32
//
//        SYSTEMTIME SystemTime;
//        GetLocalTime(&SystemTime);
//
//        fprintf(m_pLogFile, "[%02d:%02d:%02d] ",
//                        (int) SystemTime.wHour, (int) SystemTime.wMinute, (int) SystemTime.wSecond);
//
//#else
//        time_t Time = time(NULL);
//        tm *pLocalTime = localtime(&Time);
//
//        fprintf(m_pLogFile, "[%02d:%02d:%02d] ",
//                        pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
//#endif /* _WIN32 */
//
//    for (int i = 0; i < m_Ident; i ++)
//        fputs("    ", m_pLogFile);
//}
//
//void Engine::CLog::LogDataPrint(bool bIdent)
//{
//    assert(m_pLogData);
//
//    if (m_bReOpen && !m_pLogFile)
//        m_pLogFile = fopen(m_pLogFileName, "at");
//
//    if (m_pLogFile)
//    {
//        char *pTmpStr = m_pLogData;
//
//        char *pFoundEOL = strchr(pTmpStr, '\n');
//
//        while (pFoundEOL && pFoundEOL[1] != '\0')
//        {
//            *pFoundEOL = '\0';
//
//            if (bIdent)
//                Ident();
//
//            fprintf(m_pLogFile, pTmpStr);
//            fputc('\n', m_pLogFile);
//
//            pTmpStr = pFoundEOL + 1;
//
//            pFoundEOL = strchr(pTmpStr, '\n');
//        }
//
//        if (bIdent)
//            Ident();
//
//        fputs(pTmpStr, m_pLogFile);
//
//        if (m_bReOpen)
//        {
//            fclose(m_pLogFile);
//            m_pLogFile = NULL;
//        }
//    }
//}
//
//void Engine::CLog::LogDataResize()
//{
//    assert(m_pLogData);
//
//    m_LogDataSize *= 2;
//    m_pLogData = (char *) realloc(m_pLogData, sizeof(char) * m_LogDataSize);
//}
//
//void Engine::CLog::Print(const char *pFormatStr, ...)
//{
//        if (m_bLock)
//                return;
//
//    if (!m_pLogData)
//                return;
//
//    va_list ArgList;
//    va_start(ArgList, pFormatStr);
//
//    while (-1 == vsnprintf(m_pLogData, m_LogDataSize - 1, pFormatStr, ArgList))
//        LogDataResize();
//
//    LogDataPrint();
//
//    va_end(ArgList);
//}
//
//void Engine::CLog::PrintLn(const char *pFormatStr, ...)
//{
//        if (m_bLock)
//                return;
//
//    if (!m_pLogData)
//                return;
//
//    va_list ArgList;
//    va_start(ArgList, pFormatStr);
//
//    while (-1 == vsnprintf(m_pLogData, m_LogDataSize - 2, pFormatStr, ArgList))
//        LogDataResize();
//
//    char *pTmpStr = m_pLogData + strlen(m_pLogData);
//    *pTmpStr ++ = '\n';
//    *pTmpStr = '\0';
//
//    LogDataPrint();
//
//    va_end(ArgList);
//}
//
//void Engine::CLog::Append(const char *pFormatStr, ...)
//{
//        if (m_bLock)
//                return;
//
//    if (!m_pLogData)
//                return;
//
//    va_list ArgList;
//    va_start(ArgList, pFormatStr);
//
//    while (-1 == vsnprintf(m_pLogData, m_LogDataSize - 1, pFormatStr, ArgList))
//        LogDataResize();
//
//    LogDataPrint(false);
//
//    va_end(ArgList);
//}
//
//void Engine::CLog::AppendLn(const char *pFormatStr, ...)
//{
//        if (m_bLock)
//                return;
//
//    if (!m_pLogData)
//                return;
//
//    va_list ArgList;
//    va_start(ArgList, pFormatStr);
//
//    while (-1 == vsnprintf(m_pLogData, m_LogDataSize - 2, pFormatStr, ArgList))
//        LogDataResize();
//
//    char *pTmpStr = m_pLogData + strlen(m_pLogData);
//    *pTmpStr ++ = '\n';
//    *pTmpStr = '\0';
//
//    LogDataPrint(false);
//
//    va_end(ArgList);
//}
//
//void Engine::CLog::SetAppendMode()
//{
//    m_bReOpen = false;
//}
//
//void Engine::CLog::SetReOpenMode()
//{
//    m_bReOpen = true;
//}
//
//void Engine::CLog::BeginSection(const CString &SectionName)
//{
//        if (m_bLock)
//                return;
//#ifndef __RE_OS_ANDROID__
//    m_Sections.push_back(SectionName);
//    Print("[%s begin]\n", SectionName.GetBuffer());
//#endif	
//	
//    m_Ident ++;
//}
//
//void Engine::CLog::EndSection()
//{
//        if (m_bLock)
//                return;
//
//        assert(m_Ident > 0);
//
//    m_Ident --;
//#ifndef __RE_OS_ANDROID__
//    CString RemoveSection = m_Sections[m_Sections.size() - 1];
//    Print("[%s end]\n", RemoveSection.GetBuffer());	
//        m_Sections.pop_back();
//#endif
//}
//
//Engine::CLog *Engine::CLog::GetSingleton()
//{
//        CSingleton<CLog> Instance;
//        return Instance.GetSingleton();
//}

#endif /* ENGINE_FLASH */
