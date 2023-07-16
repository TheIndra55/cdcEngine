#include "MSFileSystem.h"
#include "MSFileSources.h"

cdc::MSFileSystem::MSFileSystem(const char* basePath)
	: m_Requests{}, m_Queue(nullptr), m_Free(nullptr), m_numUsedRequests(0)
{
	auto fileSource = new MSFileSourceDisk();
	strcpy(fileSource->m_basePath, basePath);

	m_FileSource = fileSource;

	SetupRequests();
}

void cdc::MSFileSystem::SetupRequests()
{
	m_Free = m_Requests;

	cdc::MSFileSystem::Request* request;

	for (int i = 0; i < 256 - 1; i++)
	{
		request = &m_Requests[i];

		request->m_FileSystem = this;
		request->m_Next = &m_Requests[i + 1];
	}

	request = &m_Requests[255];

	request->m_FileSystem = this;
	request->m_Next = nullptr;
}

void cdc::MSFileSystem::PutInQueue(Request* request, cdc::FileRequest::Priority priority)
{
	auto next = m_Queue;

	if (next)
	{
		if (priority >= cdc::FileRequest::Priority::NORMAL)
		{
			while (next->m_Next)
			{
				next = next->m_Next;
			}

			next->m_Next = request;
			request->m_Next = nullptr;
		}
		else
		{
			request->m_Next = next->m_Next;
			m_Queue->m_Next = request;
		}
	}
	else
	{
		m_Queue = request;
		request->m_Next = nullptr;
	}
}

cdc::FileRequest* cdc::MSFileSystem::RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto request = m_Free;
	m_Free = request->m_Next;
	request->m_Next = nullptr;

	m_numUsedRequests++;

	request->Init(receiver, fileName, 0, startOffset);

	return request;
}

bool cdc::MSFileSystem::FileExists(const char* fileName)
{
	return m_FileSource->Exists(fileName);
}

unsigned int cdc::MSFileSystem::GetFileSize(const char* fileName)
{
	return m_FileSource->GetSize(fileName);
}

void cdc::MSFileSystem::SetSpecialisationMask(unsigned int specMask)
{
}

unsigned int cdc::MSFileSystem::GetSpecialisationMask()
{
	return 0;
}

cdc::FileSystem::Status cdc::MSFileSystem::GetStatus()
{
	return IDLE;
}

void cdc::MSFileSystem::Update()
{
}

void cdc::MSFileSystem::Synchronize()
{
	while (GetStatus() == BUSY)
	{
		Update();
	}
}

void cdc::MSFileSystem::Request::Init(cdc::FileReceiver* receiver, const char* fileName, unsigned int fileHandle, unsigned int startOffset)
{
	m_IsInitialised = true;
	m_Priority = NORMAL;
	m_FileHandle = fileHandle;
	m_pReceiver = receiver;
	m_Status = SETUP;
	m_CloseFile = false;
	m_Offset = startOffset;
	m_BytesRead = 0;
	m_BytesProcessed = 0;
	m_ReadState = READ_STATE_INIT;
	m_IsCancelled = false;
	m_SkipSize = 0;
	m_Ref = 2;

	strcpy(m_pFileName, fileName);
	SetSize(0);
}

void cdc::MSFileSystem::Request::AddRef()
{
	m_Ref++;
}

void cdc::MSFileSystem::Request::SetCompressedSize(unsigned int compressedSize)
{
}

void cdc::MSFileSystem::Request::SetSize(unsigned int numBytesToRead)
{
	auto size = numBytesToRead;

	if (size == 0)
	{
		auto fileSystem = m_FileSystem;

		if (m_FileHandle)
		{
			size = fileSystem->m_FileSource->GetSize(m_FileHandle);
		}
		else
		{
			size = fileSystem->GetFileSize(m_pFileName);
		}
	}

	m_Size = size;
}

cdc::FileRequest::Status cdc::MSFileSystem::Request::GetStatus()
{
	return m_Status;
}

void cdc::MSFileSystem::Request::Submit(Priority priority)
{
	m_Status = QUEUED;
	m_Priority = priority;

	m_FileSystem->PutInQueue(this, priority);
}

void cdc::MSFileSystem::Request::Cancel()
{
	if (m_Status == QUEUED || m_Status == PROCESSING)
	{
		m_IsCancelled = true;
	}
	else
	{
		m_Status = CANCELLED;
	}
}

float cdc::MSFileSystem::Request::Completed()
{
	return 0.f;
}