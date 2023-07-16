#include "MSFileSources.h"

int cdc::MSFileSourceDisk::Open(const char* fileName)
{
	return 0;
}

void cdc::MSFileSourceDisk::Close(int fileHandle)
{
}

bool cdc::MSFileSourceDisk::Read(int fileHandle, unsigned int offset, unsigned int numReadBytes, void* target)
{
	return false;
}

bool cdc::MSFileSourceDisk::GetReadResult(int fileHandle, unsigned int* numReadBytes)
{
	return false;
}

unsigned int cdc::MSFileSourceDisk::GetSize(const char* fileName)
{
	return 0;
}

unsigned int cdc::MSFileSourceDisk::GetSize(int fileHandle)
{
	return 0;
}

bool cdc::MSFileSourceDisk::Exists(const char* fileName)
{
	return false;
}