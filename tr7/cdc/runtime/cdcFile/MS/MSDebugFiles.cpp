#include "MSDebugFiles.h"

#include <Windows.h>

bool cdc::MSDebugFile::Open(const char* fileName, eReadWrite readWrite)
{
	m_Handle = CreateFile(
		fileName,
		readWrite == READ ? GENERIC_READ : GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		readWrite == READ ? OPEN_EXISTING : CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_Handle == INVALID_HANDLE_VALUE)
	{
		m_Handle = nullptr;
		return false;
	}

	m_NumBytesRead = 0;
	m_ReadWrite = readWrite;

	strcpy_s(m_Filename, fileName);

	return true;
}

void cdc::MSDebugFile::Close()
{
	if (IsOpen())
	{
		CloseHandle(m_Handle);

		m_Handle = nullptr;
		m_Filename[0] = 0;
	}
}

unsigned int cdc::MSDebugFile::Read(void* buffer, unsigned int size)
{
	DWORD numBytesRead;

	if (!ReadFile(m_Handle, buffer, size, &numBytesRead, NULL))
	{
		return 0;
	}

	m_NumBytesRead += numBytesRead;

	return numBytesRead;
}

unsigned int cdc::MSDebugFile::Write(void* buffer, unsigned int size)
{
	DWORD numBytesWritten;

	if (!WriteFile(m_Handle, buffer, size, &numBytesWritten, NULL))
	{
		return 0;
	}

	return numBytesWritten;
}

void cdc::MSDebugFile::Flush()
{
	FlushFileBuffers(m_Handle);
}

bool cdc::MSDebugFile::Exists(const char* fileName)
{
	if (!Open(fileName, READ))
	{
		return false;
	}

	Close();

	return true;
}

bool cdc::MSDebugFile::EndOfFile()
{
	return m_NumBytesRead >= GetSize();
}

unsigned int cdc::MSDebugFile::GetSize()
{
	return GetFileSize(m_Handle, NULL);
}

bool cdc::MSDebugFile::IsOpen()
{
	return m_Handle != nullptr;
}