#pragma once

#include "cdc/runtime/cdcFile/DebugFileSystem.h"

namespace cdc
{
	class MSDebugFile : public DebugFile
	{
	private:
		void* m_Handle;
		unsigned int m_NumBytesRead;
		cdc::eReadWrite m_ReadWrite;

	public:
		bool Open(const char* fileName, eReadWrite readWrite);
		void Close();
		unsigned int Read(void* buffer, unsigned int size);
		unsigned int Write(void* buffer, unsigned int size);
		void Flush();
		bool Exists(const char* fileName);
		bool EndOfFile();
		unsigned int GetSize();
		bool IsOpen();
	};
}