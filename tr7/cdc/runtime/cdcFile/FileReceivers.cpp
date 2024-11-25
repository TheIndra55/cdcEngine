#include "FileReceivers.h"

#include <string.h>

cdc::FileUserBufferReceiver::FileUserBufferReceiver(void* pBuffer)
{
	m_pBuffer = (char*)pBuffer;
}

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

cdc::FileAllocatedReceiver::FileAllocatedReceiver()
{
	m_pBuffer = nullptr;
}

int cdc::FileAllocatedReceiver::ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset)
{
	memcpy(&m_pBuffer[requestOffset], data, dataSize);
	return dataSize;
}

void cdc::FileAllocatedReceiver::ReceiveStarted(FileRequest* request, unsigned int requestSize)
{
	m_pBuffer = new char[requestSize];
}

void cdc::FileAllocatedReceiver::ReceiveCancelled(FileRequest* request)
{
	delete m_pBuffer;
}

void cdc::FileAllocatedReceiver::ReceiveDone(FileRequest* request)
{
	delete m_pBuffer;
}