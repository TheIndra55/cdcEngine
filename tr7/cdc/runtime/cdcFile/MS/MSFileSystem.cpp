#include "MSFileSystem.h"

#include "cdc/runtime/cdcSys/Assert.h"

cdc::MSFileSystem::MSFileSystem(const char* basePath) : m_FileSource(), m_Requests(), m_Queue(nullptr), m_Free(nullptr), m_numUsedRequests(0)
{
	m_FileSource = new MSFileSourceDisk(basePath);

	SetupRequests();
}

void cdc::MSFileSystem::SetupRequests()
{
	m_Free = m_Requests;

	for (int i = 0; i < NUM_REQUESTS - 1; i++)
	{
		m_Requests[i].m_FileSystem = this;
		m_Requests[i].m_Next = &m_Requests[i + 1];
	}

	m_Requests[NUM_REQUESTS - 1].m_FileSystem = nullptr;
	m_Requests[NUM_REQUESTS - 1].m_Next = nullptr;
}

void cdc::MSFileSystem::ReleaseRequests()
{
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		m_Requests[i].Release();
	}
}

cdc::MSFileSystem::Request* cdc::MSFileSystem::PopFreeRequest()
{
	auto request = m_Free;
	m_Free = request->m_Next;
	request->m_Next = nullptr;

	m_numUsedRequests++;

	return request;
}

void cdc::MSFileSystem::PushFreeRequest(Request* request)
{
	if (m_Free)
	{
		request->m_Next = m_Free;
		m_Free = request;
	}
	else
	{
		m_Free = request;
		request->m_Next = nullptr;
	}

	m_numUsedRequests--;
}

void cdc::MSFileSystem::PutInQueue(Request* request, FileRequest::Priority priority)
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


void cdc::MSFileSystem::CancelRequest(Request* request)
{
	request->Cancel();
}

void cdc::MSFileSystem::RemoveFromQueue(Request* request)
{
	auto next = m_Queue;

	if (next == request)
	{
		m_Queue = request->m_Next;

		return;
	}

	while (next->m_Next)
	{
		if (next == request)
		{
			break;
		}

		next = next->m_Next;
	}

	if (next->m_Next != nullptr)
	{
		next->m_Next = next->m_Next->m_Next;
	}
}

cdc::MSFileSystem::Request* cdc::MSFileSystem::GetCurrentRequest()
{
	return m_Queue;
}

void cdc::MSFileSystem::PauseCurrentRequest()
{
	auto request = GetCurrentRequest();

	m_PausedBytesInBuffer = m_BytesInBuffer;
	m_PausedBufferOffset = m_BufferOffset;
	m_PausedFileOffset = m_FileOffset;
	m_PausedReadState = request->m_ReadState;

	request->m_ReadState = READ_STATE_PAUSED;

	auto v2 = request->m_Next;
	m_Queue = v2;

	//while (v2->m_Next)
	//{
	//	auto v3 = v2->m_Next;

	//	if (v3->m_Priority != FileRequest::HIGH)
	//	{
	//		break;
	//	}

	//	v2 = v2->m_Next;
	//}

	request->m_Next = v2->m_Next;
	v2->m_Next = request;
}

void cdc::MSFileSystem::UnpauseCurrentRequest()
{
	auto request = GetCurrentRequest();

	m_BytesInBuffer = m_PausedBytesInBuffer;
	m_BufferOffset = m_PausedBufferOffset;
	m_FileOffset = m_PausedFileOffset;

	request->m_ReadState = m_PausedReadState;
}

unsigned int cdc::MSFileSystem::RoundToSectors(unsigned int size)
{
	return (size + (SECTOR_SIZE - 1)) & ~(SECTOR_SIZE - 1);
}

unsigned int cdc::MSFileSystem::GetNumQueuedRequests()
{
	return m_numUsedRequests;
}

bool cdc::MSFileSystem::ProcessBuffer(char* buffer, Request* request, bool isReading)
{
	auto bytesInBuffer = m_BytesInBuffer;

	if (bytesInBuffer == 0)
	{
		return true;
	}

	auto skipSize = request->m_SkipSize;

	if (skipSize >= bytesInBuffer)
	{
		request->m_BytesProcessed += bytesInBuffer;
		request->m_SkipSize = skipSize - m_BytesInBuffer;

		if (isReading)
		{
			m_BufferOffset += m_BytesInBuffer;
		}
		else
		{
			m_BufferOffset = 0;
		}

		m_BytesInBuffer = 0;

		return true;
	}

	auto dataSize = bytesInBuffer - skipSize;
	auto bytesRead = request->m_pReceiver->ReceiveData(&buffer[skipSize + m_BufferOffset], dataSize, skipSize + request->m_BytesProcessed);
	auto v12 = 0;

	if ((int)(bytesRead - dataSize) <= 0)
	{
		v12 = request->m_SkipSize + bytesRead;
		request->m_SkipSize = 0;
	}
	else
	{
		v12 = m_BytesInBuffer;
		request->m_SkipSize = bytesRead - dataSize;
	}

	auto v13 = m_BytesInBuffer - v12;

	if (v13 > 0)
	{
		if (isReading)
		{
			m_BufferOffset += v12;
		}
		else
		{
			auto v14 = (v12 - m_BytesInBuffer) & 3;
			auto v15 = &buffer[m_BufferOffset + v12];
			m_BufferOffset = v14;

			memmove(&buffer[v14], v15, v13);
		}

		request->m_BytesProcessed += v12;
		m_BytesInBuffer = v13;

		return v13 == 0;
	}
	else
	{
		request->m_BytesProcessed += v12;
		m_BytesInBuffer = 0;

		auto v11 = 0;
		if (isReading)
		{
			v11 = m_BytesInBuffer;
			m_BufferOffset += v12;
		}
		else
		{
			m_BufferOffset = 0;
		}

		return v11 == 0;
	}
}

cdc::FileRequest* cdc::MSFileSystem::RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto request = PopFreeRequest();

	request->Init(receiver, fileName, 0, startOffset);

	return request;
}

cdc::File* cdc::MSFileSystem::OpenFile(const char* fileName)
{
	return new File(fileName, this);
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
	return m_Queue == nullptr ? IDLE : BUSY;
}

void cdc::MSFileSystem::Update()
{
	while (true)
	{
		auto request = m_Queue;

		if (!request)
		{
			return;
		}

		auto priority = request->m_Priority;
		auto buffer = &m_Buffer[0x80400];

		if (priority != FileRequest::HIGH)
		{
			buffer = m_Buffer;
		}

		switch (request->m_ReadState)
		{
		case READ_STATE_INIT:
			if (request->m_IsCancelled)
			{
				request->m_ReadState = READ_STATE_EXIT;
				break;
			}

			request->m_Status = FileRequest::PROCESSING;
			request->m_pReceiver->ReceiveStarted(request, request->m_Size);

			if (!request->m_FileHandle)
			{
				request->m_FileHandle = m_FileSource->Open(request->m_pFileName);
				request->m_CloseFile = true;
			}

			if (request->m_FileHandle)
			{
				request->m_ReadState = READ_STATE_READ;
				request->m_BytesRead = 0;
				request->m_BytesProcessed = 0;

				m_BufferOffset = 0;
				m_BytesInBuffer = 0;
				m_FileOffset = request->m_Offset;
			}
			else
			{
				cdc::FatalError("Failed to open %s\n", request->m_pFileName);
				request->m_ReadState = READ_STATE_EXIT;
			}

			break;
		case READ_STATE_READ:
		{
			if (request->m_IsCancelled)
			{
				request->m_ReadState = READ_STATE_EXIT;
				break;
			}

			if (request->m_Next && request->m_Next->m_Priority < priority)
			{
				PauseCurrentRequest();
				break;
			}

			if (request->m_SkipSize + request->m_BytesProcessed == request->m_Size)
			{
				request->m_ReadState = READ_STATE_EXIT;
				break;
			}

			if (request->m_SkipSize > SEEK_THRESHOLD_SIZE)
			{
				auto offset = m_FileOffset;

				m_FileOffset = offset + request->m_SkipSize;
				request->m_SkipSize = m_FileOffset & (SECTOR_SIZE - 1);

				m_FileOffset = m_FileOffset - request->m_SkipSize;

				request->m_BytesRead += m_FileOffset - offset;
				request->m_BytesProcessed += m_FileOffset - offset;
			}

			auto target = &buffer[m_BytesInBuffer + m_BufferOffset];
			auto numReadBytes = BLOCK_SIZE - m_BytesInBuffer - m_BufferOffset;

			auto v17 = request->m_Size - request->m_BytesRead;

			if (numReadBytes >= v17)
			{
				numReadBytes = v17;
			}

			if (numReadBytes > 0x80000)
			{
				numReadBytes = 0x80000;
			}

			numReadBytes = RoundToSectors(numReadBytes);

			if (numReadBytes + m_BytesInBuffer > BUFFER_SIZE)
			{
				numReadBytes -= SECTOR_SIZE;
			}

			if (numReadBytes > 0x80000)
			{
				numReadBytes = 0x80000;
			}

			if (m_FileSource->Read(request->m_FileHandle, m_FileOffset, numReadBytes, target))
			{
				request->m_ReadState = READ_STATE_READING;
				return;
			}

			break;
		}
		case READ_STATE_READING:
		{
			auto numBytesRead = 0u;

			if (m_FileSource->GetReadResult(request->m_FileHandle, &numBytesRead))
			{
				m_BytesInBuffer += numBytesRead;
				m_FileOffset += numBytesRead;

				request->m_BytesRead += numBytesRead;

				if (request->m_BytesRead > request->m_Size)
				{
					m_BytesInBuffer += request->m_Size - request->m_BytesRead;
				}

				request->m_ReadState = READ_STATE_PROCESSING;

				break;
			}

			ProcessBuffer(buffer, request, true);

			break;
		}
		case READ_STATE_PROCESSING:
			ProcessBuffer(buffer, request, false);

			if (request->m_BytesRead >= request->m_Size)
			{
				if (request->m_BytesProcessed >= request->m_Size)
				{
					request->m_ReadState = READ_STATE_EXIT;
				}
			}
			else
			{
				request->m_ReadState = READ_STATE_READ;
			}

			break;
		case READ_STATE_PAUSED:
			UnpauseCurrentRequest();

			break;
		case READ_STATE_EXIT:
			if (request->m_CloseFile)
			{
				m_FileSource->Close(request->m_FileHandle);
				request->m_FileHandle = 0;
			}

			if (request->m_IsCancelled)
			{
				request->m_pReceiver->ReceiveCancelled(request);
				request->m_Status = FileRequest::CANCELLED;
			}
			else
			{
				request->m_pReceiver->ReceiveDone(request);
				request->m_Status = FileRequest::DONE;
			}

			RemoveFromQueue(request);
			request->m_ReadState = READ_STATE_DONE;

			break;
		}
	}
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

	strcpy_s(m_pFileName, fileName);
	SetSize(0);
}

bool cdc::MSFileSystem::Request::IsInitialised()
{
	return m_IsInitialised;
}

void cdc::MSFileSystem::Request::AddRef()
{
	m_Ref++;
}

void cdc::MSFileSystem::Request::Release()
{
	m_Ref--;

	if (m_Ref <= 1)
	{
		m_FileSystem->PushFreeRequest(this);
		m_IsInitialised = false;
	}
}

void cdc::MSFileSystem::Request::SetCompressedSize(unsigned int compressedSize)
{
}

void cdc::MSFileSystem::Request::SetSize(unsigned int numBytesToRead)
{
	auto size = numBytesToRead;

	if (size == 0)
	{
		if (m_FileHandle)
		{
			size = m_FileSystem->m_FileSource->GetSize(m_FileHandle);
		}
		else
		{
			size = m_FileSystem->GetFileSize(m_pFileName);
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

cdc::MSFileSystem::File::File(const char* fileName, MSFileSystem* fileSystem)
{
	m_FileSystem = fileSystem;
	m_pRequest = nullptr;

	strcpy_s(m_pFileName, fileName);

	m_FileHandle = fileSystem->m_FileSource->Open(fileName);

	if (!m_FileHandle)
	{
		cdc::FatalError("Failed to open %s\n", fileName);
	}
}

cdc::FileRequest* cdc::MSFileSystem::File::RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	m_pRequest = m_FileSystem->PopFreeRequest();

	m_pRequest->Init(receiver, fileName, m_FileHandle, startOffset);

	return m_pRequest;
}

unsigned int cdc::MSFileSystem::File::GetSize()
{
	return m_FileSystem->m_FileSource->GetSize(m_FileHandle);
}

char* cdc::MSFileSystem::File::GetName()
{
	return m_pFileName;
}