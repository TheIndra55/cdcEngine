#pragma once

#include "FileSystem.h"

namespace cdc
{
	class FileUserBufferReceiver : public cdc::FileReceiver
	{
	protected:
		char* m_pBuffer = nullptr;

	public:
		FileUserBufferReceiver(void* pBuffer);

		int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset);
		void ReceiveStarted(FileRequest* request, unsigned int requestSize);
		void ReceiveCancelled(FileRequest* request);
		void ReceiveDone(FileRequest* request);
	};

	class FileAllocatedReceiver : public cdc::FileReceiver
	{
	protected:
		char* m_pBuffer;

	public:
		FileAllocatedReceiver();

		int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset);
		void ReceiveStarted(FileRequest* request, unsigned int requestSize);
		void ReceiveCancelled(FileRequest* request);
		void ReceiveDone(FileRequest* request);
	};
}