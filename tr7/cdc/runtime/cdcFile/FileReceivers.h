#pragma once

#include "FileSystem.h"

namespace cdc
{
	class FileUserBufferReceiver : public cdc::FileReceiver
	{
	public:
		char* m_pBuffer = nullptr;

		int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset);
		void ReceiveStarted(FileRequest* request, unsigned int requestSize);
		void ReceiveCancelled(FileRequest* request);
		void ReceiveDone(FileRequest* request);
	};
}