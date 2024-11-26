#pragma once

#include <Windows.h>

namespace cdc
{
	class MSFileSource
	{
	public:
		virtual int Open(const char* fileName) = 0;
		virtual void Close(int fileHandle) = 0;
		virtual bool Read(int fileHandle, unsigned int offset, unsigned int numReadBytes, void* target) = 0;
		virtual bool GetReadResult(int fileHandle, unsigned int* numReadBytes) = 0;
		virtual unsigned int GetSize(const char* fileName);
		virtual unsigned int GetSize(int fileHandle) = 0;
		virtual bool Exists(const char* fileName);
	};

	class MSFileSourceDisk : public MSFileSource
	{
	private:
		OVERLAPPED m_Overlapped;
		char m_basePath[MAX_PATH];

	public:
		MSFileSourceDisk(const char* basePath);

		int Open(const char* fileName);
		void Close(int fileHandle);
		bool Read(int fileHandle, unsigned int offset, unsigned int numReadBytes, void* target);
		bool GetReadResult(int fileHandle, unsigned int* numBytesRead);
		unsigned int GetSize(int fileHandle);
	};
}