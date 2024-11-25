#include "sndapp.h"

#include <string.h>

#include "cdc/runtime/cdcFile/FileSystem.h"
#include "cdc/runtime/cdcFile/FileReceivers.h"
#include "game/Main.h"

struct STREAMFILE
{
	cdc::File* file;
	cdc::FileRequest* request;
	int lastoffset;
	int lastsize;
	void* lastcontext;
	char name[128];
};

class SndReceiver : public cdc::FileAllocatedReceiver
{
protected:
	void (*m_pCallback)(void*, int, void*);
	void* m_pContext;
	unsigned int m_size;
	cdc::FileRequest** m_pRequestPtr;

public:
	SndReceiver(void (*pCallback)(void*, int, void*), void* pContext, cdc::FileRequest** pRequestPtr);

	void ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize);
	void ReceiveCancelled(cdc::FileRequest* request);
	void ReceiveDone(cdc::FileRequest* request);
};

static int s_openFiles = 0;

void* SNDAPP_StreamOpenFile(const char* fileName)
{
	s_openFiles++;

	auto streamfile = new STREAMFILE();

	auto fileSystem = GetFS();

	if (fileSystem->FileExists(fileName))
	{
		streamfile->file = fileSystem->OpenFile(fileName);
		streamfile->request = nullptr;

		strcpy_s(streamfile->name, fileName);
	}
	else
	{
		delete streamfile;
		streamfile = nullptr;
	}

	return streamfile;
}

void SNDAPP_StreamCloseFile(void* file)
{
	s_openFiles--;

	if (file)
	{
		auto streamfile = (STREAMFILE*)file;

		if (streamfile->file)
		{
			streamfile->file->~File();
		}

		delete file;
	}
}

int SNDAPP_StreamGetSize(void* file)
{
	auto streamfile = (STREAMFILE*)file;

	return streamfile->file->GetSize();
}

void SNDAPP_StreamRequestData(void* file, int offset, int size, void (*callback)(void*, int, void*), void* context)
{
	auto streamfile = (STREAMFILE*)file;

	streamfile->lastcontext = context;
	streamfile->lastoffset = offset;
	streamfile->lastsize = size;

	auto receiver = new SndReceiver(callback, context, &streamfile->request);

	auto request = streamfile->file->RequestRead(receiver, streamfile->name, offset);
	streamfile->request = request;

	request->SetSize(size);
	request->Submit(cdc::FileRequest::HIGH);
}

void SNDAPP_StreamCancel(void* file)
{
	if (file)
	{
		auto streamfile = (STREAMFILE*)file;
		auto request = streamfile->request;

		if (request)
		{
			request->Cancel();
			request->Release();

			streamfile->request = nullptr;
		}
	}
}

SndReceiver::SndReceiver(void (*pCallback)(void*, int, void*), void* pContext, cdc::FileRequest** pRequestPtr) : FileAllocatedReceiver()
{
	m_pCallback = pCallback;
	m_pContext = pContext;
	m_size = 0;
	m_pRequestPtr = pRequestPtr;
}

void SndReceiver::ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize)
{
	m_size = requestSize;

	FileAllocatedReceiver::ReceiveStarted(request, requestSize);
}

void SndReceiver::ReceiveCancelled(cdc::FileRequest* request)
{
	if (m_pRequestPtr)
	{
		(*m_pRequestPtr)->Release();
		m_pRequestPtr = nullptr;
	}

	m_pCallback(nullptr, m_size, m_pContext);

	FileAllocatedReceiver::ReceiveCancelled(request);
}

void SndReceiver::ReceiveDone(cdc::FileRequest* request)
{
	if (m_pRequestPtr)
	{
		(*m_pRequestPtr)->Release();
		m_pRequestPtr = nullptr;
	}

	m_pCallback(m_pBuffer, m_size, m_pContext);

	FileAllocatedReceiver::ReceiveDone(request);
}