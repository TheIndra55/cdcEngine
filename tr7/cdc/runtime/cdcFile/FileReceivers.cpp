#include "FileReceivers.h"

#include <string.h>

int cdc::FileUserBufferReceiver::ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset)
{
	memcpy(&m_pBuffer[requestOffset], data, dataSize);

	return dataSize;
}

void cdc::FileUserBufferReceiver::ReceiveStarted(FileRequest* request, unsigned int requestSize)
{
}

void cdc::FileUserBufferReceiver::ReceiveCancelled(FileRequest* request)
{
}

void cdc::FileUserBufferReceiver::ReceiveDone(FileRequest* request)
{
}