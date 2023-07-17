#include "MSFileSources.h"

#include "cdc/runtime/cdcSys/Assert.h"

int cdc::MSFileSourceDisk::Open(const char* fileName)
{
	char str[800] = {};

	if (fileName[1] == ':')
	{
		strcpy(str, m_basePath);
	}

	if (fileName[0] != '\\')
	{
		auto len = strlen(str);

		if (len > 1 && str[1] == ':' && str[2] == '\\')
		{
			str[len] = '\\';
		}
	}

	strcat(str, fileName);

	HANDLE handle = CreateFileA(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	return (int)handle;
}

void cdc::MSFileSourceDisk::Close(int fileHandle)
{
	CloseHandle((HANDLE)fileHandle);
}

bool cdc::MSFileSourceDisk::Read(int fileHandle, unsigned int offset, unsigned int numReadBytes, void* target)
{
	this->m_Overlapped = {};
	this->m_Overlapped.Offset = offset;

	if (ReadFile((HANDLE)fileHandle, target, numReadBytes, NULL, &this->m_Overlapped) || GetLastError() == ERROR_IO_PENDING)
	{
		return true;
	}

	cdc::FatalError("Disc error");
	return false;
}

bool cdc::MSFileSourceDisk::GetReadResult(int fileHandle, unsigned int* numBytesRead)
{
	if (!GetOverlappedResult((HANDLE)fileHandle, &this->m_Overlapped, (LPDWORD)numBytesRead, FALSE))
	{
		auto lastError = GetLastError();

		if (lastError != ERROR_IO_PENDING && lastError != ERROR_IO_INCOMPLETE)
		{
			cdc::FatalError("Disc error");
			return false;
		}
	}

	return true;
}

unsigned int cdc::MSFileSourceDisk::GetSize(const char* fileName)
{
	auto handle = Open(fileName);

	if (handle)
	{
		auto size = GetSize(handle);
		Close(handle);

		return size;
	}

	return 0;
}

unsigned int cdc::MSFileSourceDisk::GetSize(int fileHandle)
{
	auto size = GetFileSize((HANDLE)fileHandle, 0);

	if (size == INVALID_FILE_SIZE)
	{
		cdc::FatalError("Disc error");
	}

	return size;
}

bool cdc::MSFileSourceDisk::Exists(const char* fileName)
{
	return GetSize(fileName) != 0;
}