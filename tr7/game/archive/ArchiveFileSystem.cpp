#include "ArchiveFileSystem.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cdc/runtime/cdcFile/FileReceivers.h"
#include "cdc/runtime/cdcSys/Assert.h"

unsigned int ComputeCRC(int size, const void* rawData);

ArchiveFileSystem::ArchiveFileSystem(FileSystem* pDiskFS)
	: m_numArchives(0), m_numRecords(0), m_pHash(nullptr), m_pRecord(nullptr)
{
	m_pDiskFS = pDiskFS;
	m_pArchiveFiles = nullptr;
	m_specMask = 1;
}

bool ArchiveFileSystem::Open(const char* archiveFileName)
{
	char extension[8];

	char baseFileName[160];
	strcpy(baseFileName, archiveFileName);

	auto len = strlen(baseFileName);
	for (unsigned int i = 0; i < len; i++)
	{
		if (baseFileName[i] == '.')
		{
			baseFileName[i] = 0;
		}
	}

	char tmpFileName[160];
	strcpy(tmpFileName, baseFileName);
	strcat(tmpFileName, ".000");

	auto receiver = new cdc::FileUserBufferReceiver();

	// Read number of records
	receiver->m_pBuffer = (char*)&m_numRecords;
	auto request = m_pDiskFS->RequestRead(receiver, tmpFileName, 0);

	request->SetSize(sizeof(m_numRecords));
	request->Submit(cdc::FileRequest::NORMAL);

	m_pDiskFS->Synchronize();

	// Read filename hashes and file records
	auto size = (sizeof(ArchiveRecord) + sizeof(unsigned int)) * m_numRecords;

	m_pHash = (unsigned int*)new char[size + 4];

	receiver->m_pBuffer = (char*)m_pHash;
	request = m_pDiskFS->RequestRead(receiver, tmpFileName, 0);

	request->SetSize(size + 4);
	request->Submit(cdc::FileRequest::NORMAL);

	m_pDiskFS->Synchronize();

	//
	m_pRecord = (ArchiveRecord*)&m_pHash[m_numRecords + 1];
	m_pHash++;

	auto offset = 0ULL;
	for (unsigned int i = 0; i < m_numRecords; i++)
	{
		auto record = m_pRecord[i];

		if ((unsigned __int64)record.offset << 11 > offset)
		{
			offset = (unsigned __int64)record.offset << 11;
		}
	}

	m_numArchives = (offset + 0x95FFFFF) / ARCHIVE_ALIGNMENT;
	m_pArchiveFiles = new cdc::File*[m_numArchives];

	for (unsigned int i = 0; i < m_numArchives; i++)
	{
		strcpy(tmpFileName, baseFileName);
		sprintf(extension, ".%03d", i);
		strcat(tmpFileName, extension);

		auto archiveFile = m_pDiskFS->OpenFile(tmpFileName);

		if (!archiveFile)
		{
			cdc::FatalError("Unable to open bigfile: %s", tmpFileName);
		}

		this->m_pArchiveFiles[i] = archiveFile;
	}

	return true;
}

ArchiveRecord* ArchiveFileSystem::Find(const char* fileName)
{
	auto hash = CalculateHash(fileName);

	// TODO replace with original game binary search algorithm
	for (unsigned int i = 0; i < m_numRecords; i++)
	{
		auto record = &m_pRecord[i];

		if (m_pHash[i] == hash && (record->specMask & m_specMask) == m_specMask)
		{
			return record;
		}
	}

	return nullptr;
}

unsigned int ArchiveFileSystem::CalculateHash(const char* fileName)
{
	char lowerCaseString[256];

	strcpy(lowerCaseString, fileName);
	_strlwr(lowerCaseString);

	return ComputeCRC(strlen(lowerCaseString), lowerCaseString);
}

cdc::FileRequest* ArchiveFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto record = Find(fileName);

	if (!record)
	{
		cdc::FatalError("Can't open file %s\n", fileName);
	}

	auto offset = (unsigned __int64)record->offset << 11;

	auto file = m_pArchiveFiles[offset / ARCHIVE_ALIGNMENT];
	auto request = file->RequestRead(receiver, fileName, startOffset + offset % ARCHIVE_ALIGNMENT);

	request->SetSize(record->size);
	request->SetCompressedSize(record->compressedLength);

	return request;
}

cdc::File* ArchiveFileSystem::OpenFile(const char* fileName)
{
	auto record = Find(fileName);

	if (!record)
	{
		return nullptr;
	}

	auto offset = (unsigned __int64)record->offset << 11;
	auto file = m_pArchiveFiles[offset / ARCHIVE_ALIGNMENT];

	return new ArchiveFile(record, file, offset % ARCHIVE_ALIGNMENT);
}

bool ArchiveFileSystem::FileExists(const char* fileName)
{
	return Find(fileName) != nullptr;
}

unsigned int ArchiveFileSystem::GetFileSize(const char* fileName)
{
	auto record = Find(fileName);

	if (record)
	{
		return record->size;
	}

	return 0;
}

void ArchiveFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	m_specMask = specMask;
}

unsigned int ArchiveFileSystem::GetSpecialisationMask()
{
	return m_specMask;
}

cdc::FileSystem::Status ArchiveFileSystem::GetStatus()
{
	return m_pDiskFS->GetStatus();
}

void ArchiveFileSystem::Update()
{
	m_pDiskFS->Update();
}

void ArchiveFileSystem::Synchronize()
{
	m_pDiskFS->Synchronize();
}

unsigned int ComputeCRC(int size, const void* rawData)
{
	auto data = (char*)rawData;
	auto hash = 0xFFFFFFFF;

	for (int i = 0; i < size; i++)
	{
		hash ^= data[i] << 24;

		for (int j = 0; j < 8; j++)
		{
			if (hash & 0x80000000)
			{
				hash = (hash << 1) ^ 0x4C11DB7;
			}
			else
			{
				hash <<= 1;
			}
		}
	}

	return ~hash;
}

ArchiveFile::ArchiveFile(ArchiveRecord* record, cdc::File* archiveFile, unsigned int offsetInArchive)
{
	m_pRecord = record;
	m_pArchiveFile = archiveFile;
	m_offsetInArchive = offsetInArchive;
}

cdc::FileRequest* ArchiveFile::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	return m_pArchiveFile->RequestRead(receiver, fileName, m_offsetInArchive + startOffset);
}

unsigned int ArchiveFile::GetSize()
{
	return m_pRecord->size;
}