#include "MSFileSources.h"

#include "cdc/runtime/cdcSys/Assert.h"

unsigned int cdc::MSFileSource::GetSize(const char* fileName)
{
	auto handle = Open(fileName);
	auto size = 0;

	if (handle)
	{
		size = GetSize(handle);
		Close(handle);
	}

	return size;
}

bool cdc::MSFileSource::Exists(const char* fileName)
{
	return GetSize(fileName) != 0;
}

cdc::MSFileSourceDisk::MSFileSourceDisk(const char* basePath) : m_Overlapped()
{
	strcpy_s(m_basePath, basePath);
}

int cdc::MSFileSourceDisk::Open(const char* fileName)
{
	char str[800] = "";

	if (fileName[1] != ':')
	{
		strcpy_s(str, m_basePath);
	}

	if (fileName[0] != '\\' && strlen(str) > 1 && str[1] == ':' && str[2] != '\\')
	{
		strcat(str, "\\");
	}

	strcat(str, fileName);

	// Open the file
	auto handle = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

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
	m_Overlapped = { };
	m_Overlapped.Offset = offset;

	if (ReadFile((HANDLE)fileHandle, target, numReadBytes, NULL, &m_Overlapped) || GetLastError() == ERROR_IO_PENDING)
	{
		return true;
	}

	cdc::FatalError("Disc error\n");

	return false;
}

bool cdc::MSFileSourceDisk::GetReadResult(int fileHandle, unsigned int* numBytesRead)
{
	if (!GetOverlappedResult((HANDLE)fileHandle, &m_Overlapped, (LPDWORD)numBytesRead, FALSE))
	{
		auto lastError = GetLastError();

		if (lastError != ERROR_IO_PENDING && lastError != ERROR_IO_INCOMPLETE)
		{
			cdc::FatalError("Disc error\n");
		}

		return false;
	}

	return true;
}

unsigned int cdc::MSFileSourceDisk::GetSize(int fileHandle)
{
	auto size = GetFileSize((HANDLE)fileHandle, NULL);

	if (size == INVALID_FILE_SIZE)
	{
		cdc::FatalError("Disc error\n");
	}

	return size;
}