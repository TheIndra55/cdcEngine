#pragma once

#include "cdc/runtime/cdcFile/FileReceivers.h"

#define ARCHIVE_ALIGNMENT 0x9600000

struct ArchiveRecord
{
	unsigned int size;
	unsigned int offset;
	unsigned int specMask;
	int compressedLength;
};

class ArchiveFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_pDiskFS;
	unsigned int m_specMask;

	cdc::File** m_pArchiveFiles;

	unsigned int m_numArchives;
	ArchiveRecord* m_pRecord;
	unsigned int* m_pHash;
	unsigned int m_numRecords;

public:
	ArchiveFileSystem(cdc::FileSystem* pDiskFS);

	bool Open(const char* archiveFileName);
	ArchiveRecord* Find(const char* fileName);
	unsigned int CalculateHash(const char* fileName);

	cdc::FileRequest* RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset);
	cdc::File* OpenFile(const char* fileName);
	bool FileExists(const char* fileName);
	unsigned int GetFileSize(const char* fileName);
	void SetSpecialisationMask(unsigned int specMask);
	unsigned int GetSpecialisationMask();
	Status GetStatus();
	void Update();
	void Synchronize();
};