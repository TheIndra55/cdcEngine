#pragma once

#include <Windows.h>

#include "MSFileSystem.h"

namespace cdc
{
	class MSFileSourceDisk : public cdc::MSFileSource
	{
	private:
		OVERLAPPED m_Overlapped;

	public:
		char m_basePath[260];

		int Open(const char* fileName);
		void Close(int fileHandle);
		bool Read(int fileHandle, unsigned int offset, unsigned int numReadBytes, void* target);
		bool GetReadResult(int fileHandle, unsigned int* numReadBytes);
		unsigned int GetSize(const char* fileName);
		unsigned int GetSize(int fileHandle);
		bool Exists(const char* fileName);
	};
}