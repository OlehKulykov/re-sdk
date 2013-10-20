/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
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

/* $ Revision 1.5.14 $ */

//#include "StdAfx.H"
#include "Base.h"
#include "Log.h"
#include "FileManager.h"

#ifndef ENGINE_FLASH

Engine::CArchive &Engine::CArchive::operator<<(const CString &String)
{
        int Length = String.GetLength();

        if (Length < 0xff)
                operator<<((BYTE) Length);
        else
        if (Length < 0xfffe)
        {
                operator<<((BYTE) 0xff);
                operator<<((REUInt16) Length);
        }
        else
        {
                operator<<((BYTE) 0xff);
                operator<<((REUInt16) 0xffff);
                operator<<((REUInt32) Length);
        }

        SafeWrite(String.GetBuffer(), Length);

        return *this;
}

Engine::CArchive &Engine::CArchive::operator>>(CString &String)
{
        int Length;

        BYTE bLength;
        operator>>(bLength);

        if (bLength < 0xff)
        {
                Length = (int) bLength;
        }
        else
        {
                REUInt16 wLength;
                operator>>(wLength);

                if (wLength < 0xfffe)
                {
                        Length = (int) wLength;
                }
                else
                {
                        REUInt32 dwLength;
                        operator>>(dwLength);

                        Length = (int) dwLength;
                }
        }

        if (Length)
        {
                String.AllocBeforeWrite(Length);
                char *pBuffer = String.GetBuffer();
                SafeRead(pBuffer, Length);
                pBuffer[Length] = '\0';
                String.SetLength(Length);
        }
        else
        {
                String = "";
        }

        return *this;
}

void Engine::CArchive::SafeWrite( void *pSrc, long nBytes ) /* Not Safe Yet */
{
        long nBytesWrited = m_pFile->Write(pSrc, nBytes);
#ifndef __RE_OS_ANDROID__
        if (nBytesWrited != nBytes)
                throw CException(""/*"Archive serialization error: Cannot write %d bytes, %d writed", nBytes, nBytesWrited*/);
#endif
}

void Engine::CArchive::SafeRead( void *pDest, long nBytes ) /* Not Safe Yet */
{
        long nBytesReaded = m_pFile->Read(pDest, nBytes);
#ifndef __RE_OS_ANDROID__
        if (nBytesReaded != nBytes)
                throw CException(""/*"Archive serialization error: Cannot read %d bytes, %d readed", nBytes, nBytesReaded*/);
#endif
}

Engine::CFileManager::CFileManager()
{
        m_bLocalMode = true;
        m_LocalPath[0] = '\0';
}

Engine::CFileManager::~CFileManager()
{
}

bool Engine::CFileManager::OpenFile(CFile &File, const char *pFileName)
{
        assert(pFileName);

        if (!m_bLocalMode)
                return false;

        FILE *pFile;
        int i;

        char ValidFileName[MAX_PATH];
        strcpy(ValidFileName, pFileName);

        for (i = 0; i < (int) strlen(pFileName); i ++)
                if (ValidFileName[i] == '\\')
                        ValidFileName[i] = '/';

        if (m_LocalPath[0] == '\0')
                 pFile = fopen(ValidFileName, "rb");
        else
        {
                CString FullName = CString(m_LocalPath) + '/' + CString(ValidFileName);

                pFile = fopen(FullName.GetBuffer(), "rb");
        }

        if (!pFile)
                return false;

    File.SetFile(pFile);

        return true;
}

void Engine::CFileManager::SetLocalPath(const char *pPath)
{
        assert(pPath);
        strcpy(m_LocalPath, pPath);
}

bool Engine::CFile::Open(const char *pFileName)
{
        if (!CFileManager::GetSingleton()->OpenFile(*this, pFileName))
        {
				//CLog::GetSingleton()->PrintLn(""/*"Unable to open file '%s'", pFileName*/);
                return false;
        }

        m_FileName = pFileName;

        return true;
}

bool Engine::CFile::Open( const CString &FileName )
{
        return Open(FileName.GetBuffer());
}

long Engine::CFile::GetSize()
{
        assert(m_pInFile != NULL);

        return m_pInFile->GetSize();
}

void Engine::CFile::Close()
{
        m_FileName.Release();

        SAFE_DELETE(m_pInFile);

        if (m_pStdioFile)
        {
                FILE *pFile = m_pStdioFile->GetFile();
                assert(pFile != NULL);
                fclose(pFile);
                SAFE_DELETE(m_pStdioFile);
        }
}

long Engine::CFile::Seek(long Position, int Where)
{
        assert(m_pInFile != NULL);

        return m_pInFile->Seek(Position, Where);
}

long Engine::CFile::Tell()
{
        assert(m_pInFile != NULL);

        return m_pInFile->Tell();
}

long Engine::CFile::Read(void *pBuffer, long nBytes)
{
        long nReadedBytes = m_pInFile->Read(pBuffer, nBytes);

        return nReadedBytes;
}

void Engine::CFile::SetFile( FILE *pFile )
{
        assert(pFile != NULL);
        assert(m_pStdioFile == NULL);
        assert(m_pInFile == NULL);

        m_pStdioFile = new CStdioFile(pFile);
        m_pInFile = new CInFile(*m_pStdioFile, 0);
}

void Engine::CFile::SetPakFile( FILE *pPakFile, int Position, int Size, bool bEncoded )
{
        assert(pPakFile != NULL);
        assert(m_pStdioFile == NULL);
        assert(m_pInFile == NULL);

        m_pStdioFile = new CStdioFile(pPakFile);
        m_pInFile = new CInFile(*m_pStdioFile, Position, Size);

        m_bEncoded = bEncoded;
}

void Engine::CFile::SetPakFile( CAbstractFile *pPakFile, int Position, int Size, bool bEncoded )
{
        assert(pPakFile != NULL);
        assert(m_pStdioFile == NULL);
        assert(m_pInFile == NULL);

        m_pInFile = new CInFile(*pPakFile, Position, Size);

        m_bEncoded = bEncoded;
}

Engine::CFile::CFile()
{
        m_pInFile = NULL;
        m_pStdioFile = NULL;
        m_bEncoded = false;
}

const char * Engine::CFile::GetFileName() const
{
        return m_FileName.GetBuffer();
}

void Engine::CFile::OpenSafe( const char *pFileName )
{
		bool isOpened = Open(pFileName);
        if (!isOpened)
		{
#ifndef __RE_OS_ANDROID__
                throw CException(""/*"Can't open file '%s'", pFileName*/);
#endif
		}
}

long Engine::CInFile::Write( void *pDest, long nBytes )
{
        long MaxBytes = m_nBytes - (m_Position - m_StartPosition);
        if (nBytes > MaxBytes)
                nBytes = MaxBytes;

        long nGotBytes = m_File.Write(pDest, nBytes);
        m_Position += nGotBytes;

        return nGotBytes;
}

long Engine::CInFile::Seek( long Position, int Where /*= SEEK_SET*/ )
{
        if (Where == SEEK_SET)
                m_Position = m_StartPosition + Position;
        else
                if (Where == SEEK_CUR)
                        m_Position += Position;
                else
                        if (Where == SEEK_END)
                                m_Position = m_StartPosition + m_nBytes;

        if (m_Position < m_StartPosition)
                m_Position = m_StartPosition;

        if (m_Position > m_StartPosition + m_nBytes)
                m_Position = m_StartPosition + m_nBytes;

        return m_File.Seek(m_Position, SEEK_SET) - m_StartPosition;
}

long Engine::CInFile::Read( void *pDest, long nBytes )
{
        long MaxBytes = m_nBytes - (m_Position - m_StartPosition);
        if (nBytes > MaxBytes)
                nBytes = MaxBytes;

        long nGotBytes = m_File.Read(pDest, nBytes);
        m_Position += nGotBytes;

        return nBytes;
}

Engine::CInFile::CInFile( CAbstractFile &File, long StartPosition, long nBytes /*= -1*/ ) :
        m_File(File)
{
        long nFileBytes = m_File.GetSize() - StartPosition;
		bool errrrrr = ((nBytes >= 0 && nFileBytes < 0) || nFileBytes < nBytes);
        if (!errrrrr)
		{
        if (nBytes < 0)
                nBytes = nFileBytes;
        m_nBytes = nBytes;
        m_File.Seek(StartPosition);
        m_Position = StartPosition;
        m_StartPosition = StartPosition;
		}
}

Engine::CMemoryFile::CMemoryFile( long nBytes )
{
        long nMaxBytes = 16;
        while (nMaxBytes < nBytes)
                nMaxBytes *= 2;

        m_pData = (BYTE *) malloc(sizeof(BYTE) * nMaxBytes);
        m_nBytes = 0;
        m_nMaxBytes = nMaxBytes;
        m_Position = 0;
        m_bWrite = true;

        m_bFreeBuffer = true;
}

Engine::CMemoryFile::CMemoryFile( CAbstractFile *pFile, long nBytes )
{
        if (nBytes)
        {
                long nMaxBytes = 16;
                while (nMaxBytes < nBytes)
                        nMaxBytes *= 2;

                m_pData = (BYTE *) malloc(sizeof(BYTE) * nMaxBytes);
                pFile->Read(m_pData, nBytes);
                m_nBytes = nBytes;
                m_nMaxBytes = nMaxBytes;
                m_Position = 0;
                m_bWrite = false;
        }
        else
        {
                m_pData = NULL;
                m_nBytes = 0;
                m_nMaxBytes = 0;
                m_Position = 0;
                m_bWrite = false;
        }

        m_bFreeBuffer = true;
}

Engine::CMemoryFile::CMemoryFile()
{
        m_nBytes = 0;
        m_nMaxBytes = 0;
        m_pData = NULL;
        m_Position = 0;
        m_bWrite = true;
        m_bFreeBuffer = true;
}

Engine::CMemoryFile::CMemoryFile( void *pData, long nBytes )
{
        m_pData = (BYTE *) pData;
        m_nBytes = nBytes;
        m_nMaxBytes = 0;
        m_Position = 0;
        m_bWrite = false;
        m_bFreeBuffer = false;
}

void Engine::CMemoryFile::Free()
{
        if (m_bFreeBuffer)
                free(m_pData);

        m_nBytes = 0;
        m_nMaxBytes = 0;
        m_pData = NULL;
        m_Position = 0;
        m_bWrite = true;
        m_bFreeBuffer = true;
}

long Engine::CMemoryFile::Read( void *pDest, long nBytes )
{
        long MaxBytes = m_nBytes - m_Position;
        if (nBytes > MaxBytes)
                nBytes = MaxBytes;

        if (nBytes > 0)
        {
                memcpy(pDest, m_pData + m_Position, nBytes);
                m_Position += nBytes;
        }
        else
                if (nBytes < 0)
                        nBytes = 0;

        return nBytes;
}

void Engine::CMemoryFile::ExpandBuffer(long nBytes)
{
        if (m_nMaxBytes > nBytes)
                return;

        if (m_nMaxBytes == 0)
                m_nMaxBytes = 128;
        while (nBytes > m_nMaxBytes)
                m_nMaxBytes *= 2;

        m_pData = (BYTE *) realloc(m_pData, sizeof(BYTE) * m_nMaxBytes);
}

long Engine::CMemoryFile::Write( void *pSrc, long nBytes )
{
        long LastByte = m_Position + nBytes;

        ExpandBuffer(LastByte);

        if (m_nBytes < LastByte)
                m_nBytes = LastByte;

        memcpy(m_pData + m_Position, pSrc, sizeof(BYTE) * nBytes);
        m_Position += nBytes;

        return nBytes;
}

void Engine::CMemoryFile::Compact()
{
        if (!m_pData || m_nBytes == m_nMaxBytes)
                return;

        m_pData = (BYTE *) realloc(m_pData, sizeof(BYTE) * m_nBytes);
}

long Engine::CMemoryFile::Seek( long Position, int Where /*= SEEK_SET*/ )
{
        if (Where == SEEK_SET)
        {
                m_Position = Position;
        }
        else
        if (Where == SEEK_CUR)
        {
                m_Position += Position;
        }
        else
        if (Where == SEEK_END)
        {
                m_Position = m_nBytes;
        }

        return m_Position;
}

Engine::CFileManager *Engine::CFileManager::GetSingleton()
{
        CSingleton<CFileManager> Instance;
        return Instance.GetSingleton();
}

Engine::CMemoryFile::~CMemoryFile()
{
        if (m_bFreeBuffer)
                free(m_pData);
}

long Engine::CStdioFile::GetSize()
{
        long PrevPosition = (long) ftell(m_pFile);
        fseek(m_pFile, 0L, SEEK_END);
        long Size = (long) ftell(m_pFile);
        fseek(m_pFile, PrevPosition, SEEK_SET);

        return Size;
}

long Engine::CStdioFile::Seek( long Position, int Where /*= SEEK_SET*/ )
{
        return fseek(m_pFile, Position, Where);
}

long Engine::CStdioFile::Tell()
{
        return ftell(m_pFile);
}

long Engine::CStdioFile::Read( void *pDest, long nBytes )
{
        return (long) fread(pDest, 1, nBytes, m_pFile);
}

long Engine::CStdioFile::Write( void *pSrc, long nBytes )
{
        return (long) fwrite(pSrc, 1, nBytes, m_pFile);
}

#endif /* ENGINE_FLASH */
