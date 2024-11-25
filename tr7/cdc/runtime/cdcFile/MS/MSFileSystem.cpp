#include "MSFileSystem.h"
#include "MSFileSources.h"

#include "cdc/runtime/cdcSys/Assert.h"

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

unsigned int cdc::MSFileSystem::RoundToSectors(unsigned int size)
{
	if ((size & 0x7FF) != 0)
	{
		return 0x800 - (size & 0x7FF) + size;
	}

	return size;
}

bool cdc::MSFileSystem::ProcessBuffer(char* buffer, Request* request, bool isReading)
{
	auto size = m_BytesInBuffer;

	if (size == 0)
	{
		return true;
	}

	auto skipSize = request->m_SkipSize;

	if (skipSize >= size)
	{
		request->m_BytesProcessed += size;
		request->m_SkipSize -= m_BytesInBuffer;

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

	auto readSize = size - skipSize;
	auto bytesRead = request->m_pReceiver->ReceiveData(&buffer[skipSize + m_BufferOffset], readSize, skipSize + request->m_BytesProcessed);

	auto bufferBytes = 0;
	if ((bytesRead - readSize) <= 0)
	{
		bufferBytes = request->m_SkipSize + bytesRead;
		request->m_SkipSize = 0;
	}
	else
	{
		bufferBytes = m_BytesInBuffer;
		request->m_SkipSize = bytesRead - readSize;
	}

	auto bytesLeft = m_BytesInBuffer - bufferBytes;

	if (bytesLeft == 0)
	{
		if (isReading)
		{
			m_BufferOffset += bufferBytes;
		}
		else
		{
			m_BufferOffset = (bufferBytes - m_BytesInBuffer) & 3;

			memmove(&buffer[m_BufferOffset], &buffer[m_BufferOffset + bufferBytes], bytesLeft);
		}

		request->m_BytesProcessed += bufferBytes;
		m_BytesInBuffer = bytesLeft;

		return bytesLeft == 0;
	}

	request->m_BytesProcessed += bufferBytes;
	m_BytesInBuffer = 0;

	auto bytes = 0;
	if (isReading)
	{
		bytes = m_BytesInBuffer;
		m_BufferOffset += bufferBytes;
	}
	else
	{
		m_BufferOffset = 0;
	}

	return bytes == 0;
}

cdc::MSFileSystem::Request* cdc::MSFileSystem::PopFreeRequest()
{
	auto request = m_Free;
	m_Free = request->m_Next;
	request->m_Next = nullptr;

	m_numUsedRequests++;

	return request;
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
	return m_Queue == 0 ? IDLE : BUSY;
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

		if (priority)
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

			request->m_Status = cdc::FileRequest::PROCESSING;
			request->m_pReceiver->ReceiveStarted(request, request->m_Size);

			if (!request->m_FileHandle)
			{
				auto handle = m_FileSource->Open(request->m_pFileName);

				request->m_FileHandle = handle;
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
				cdc::FatalError("Failed to open %s", request->m_pFileName);
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

			auto skipSize = request->m_SkipSize;

			if (skipSize + request->m_BytesProcessed == request->m_Size)
			{
				request->m_ReadState = READ_STATE_EXIT;
				break;
			}

			if (skipSize > 0x40000)
			{
				auto offset = m_FileOffset + skipSize;
				m_FileOffset = offset;

				skipSize = offset & 0x7FF;
				request->m_SkipSize = skipSize;

				m_FileOffset = m_FileOffset - skipSize;

				request->m_BytesRead += m_FileOffset - offset;
				request->m_BytesProcessed += m_FileOffset - offset;
			}

			auto target = &buffer[m_BytesInBuffer + m_BufferOffset];
			auto numReadBytes = 0x100000 - m_BytesInBuffer - m_BufferOffset;

			auto readSize = request->m_Size - request->m_BytesRead;
			if (numReadBytes > readSize)
			{
				numReadBytes = readSize;
			}

			if (numReadBytes > 0x80000)
			{
				numReadBytes = 0x80000;
			}

			numReadBytes = RoundToSectors(numReadBytes);

			if (numReadBytes + m_BytesInBuffer > 0x100800)
			{
				numReadBytes -= 0x800;
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
			unsigned int numBytesRead = 0;

			if (m_FileSource->GetReadResult(request->m_FileHandle, &numBytesRead))
			{
				m_BytesInBuffer += numBytesRead;
				m_FileOffset = numBytesRead + m_FileOffset;

				request->m_BytesRead += numBytesRead;

				if (request->m_BytesRead > request->m_Size)
				{
					m_BytesInBuffer += request->m_Size - request->m_BytesRead;
				}

				request->m_ReadState = READ_STATE_PROCESSING;

				break;
			}

			ProcessBuffer(buffer, request, true);

			return;
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
			m_BytesInBuffer = m_PausedBytesInBuffer;
			m_BufferOffset = m_PausedBufferOffset;
			m_FileOffset = m_PausedFileOffset;
			
			request->m_ReadState = m_PausedReadState;

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
				request->m_Status = cdc::FileRequest::CANCELLED;
			}
			else
			{
				request->m_pReceiver->ReceiveDone(request);
				request->m_Status = cdc::FileRequest::DONE;
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

	strcpy(m_pFileName, fileName);
	SetSize(0);
}

void cdc::MSFileSystem::Request::AddRef()
{
	m_Ref++;
}

void cdc::MSFileSystem::Request::Release()
{
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

cdc::MSFileSystem::File::File(const char* fileName, MSFileSystem* fileSystem)
{
	m_FileSystem = fileSystem;
	m_pRequest = nullptr;

	strcpy(m_pFileName, fileName);

	m_FileHandle = fileSystem->m_FileSource->Open(fileName);

	if (!m_FileHandle)
	{
		cdc::FatalError("Failed to open %s", fileName);
	}
}

cdc::FileRequest* cdc::MSFileSystem::File::RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto request = m_FileSystem->PopFreeRequest();
	m_pRequest = request;

	request->Init(receiver, fileName, m_FileHandle, startOffset);

	return request;
}

unsigned int cdc::MSFileSystem::File::GetSize()
{
	return m_FileSystem->m_FileSource->GetSize(m_FileHandle);
}