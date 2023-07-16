#include "MSFileSystem.h"
#include "MSFileSources.h"

cdc::MSFileSystem::MSFileSystem(const char* basePath)
{
	auto fileSource = new MSFileSourceDisk();
	strcpy(fileSource->m_basePath, basePath);

	m_FileSource = fileSource;

	SetupRequests();
}

void cdc::MSFileSystem::SetupRequests()
{
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

cdc::FileRequest* cdc::MSFileSystem::RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	return nullptr;
}

bool cdc::MSFileSystem::FileExists(const char* fileName)
{
	return false;
}

unsigned int cdc::MSFileSystem::GetFileSize(const char* fileName)
{
	return 0;
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
}

void cdc::MSFileSystem::Request::AddRef()
{
}

void cdc::MSFileSystem::Request::SetCompressedSize(unsigned int compressedSize)
{
}

void cdc::MSFileSystem::Request::SetSize(unsigned int size)
{
}
cdc::FileRequest::Status cdc::MSFileSystem::Request::GetStatus()
{
	return CANCELLED;
}

void cdc::MSFileSystem::Request::Submit(Priority priority)
{
}

void cdc::MSFileSystem::Request::Cancel()
{
}

float cdc::MSFileSystem::Request::Completed()
{
	return 0.f;
}