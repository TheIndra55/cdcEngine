#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

class MultiFileSystem : public cdc::FileSystem
{
private:
	class System
	{
	public:
		cdc::FileSystem* m_pSystem;
		bool m_bReprioritize;
	};

	System m_Systems[8];
	unsigned int m_NumSystems;

	cdc::FileSystem* GetBestFileSystem(const char* fileName);

public:
	MultiFileSystem();
	void Add(cdc::FileSystem* pFileSystem, bool bReprioritize, bool bAddToFront);

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
