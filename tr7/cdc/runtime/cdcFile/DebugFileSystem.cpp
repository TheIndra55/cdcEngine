#include "DebugFileSystem.h"

#include "MS/MSDebugFiles.h"

cdc::DebugFileSystem* cdc::DebugFileSystem::s_instance = nullptr;

cdc::DebugFile* cdc::DebugFileSystem::GetFile()
{
	return new MSDebugFile();
}

bool cdc::DebugFileSystem::FileExists(const char* fileName)
{
	auto file = GetFile();
	auto exists = file->Exists(fileName);

	delete file;

	return exists;
}

unsigned int cdc::DebugFileSystem::GetFileSize(const char* fileName)
{
	auto file = GetFile();
	auto size = 0;

	if (file->Open(fileName, READ))
	{
		size = file->GetSize();
		file->Close();
	}

	delete file;

	return size;
}

cdc::DebugFileSystem* cdc::DebugFileSystem::Create()
{
	s_instance = new cdc::DebugFileSystem();
	return s_instance;
}

void cdc::DebugFileSystem::Destroy()
{
	delete s_instance;
	s_instance = nullptr;
}

cdc::DebugFileSystem* cdc::DebugFileSystemPtr()
{
	return cdc::DebugFileSystem::s_instance;
}