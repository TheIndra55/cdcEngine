#pragma once

#include "ResolveSection.h"

class DTPDataSection : public ResolveSection
{
public:
	DTPDataSection();

	unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip);
	void ReleaseResource(unsigned int rtrID);
	int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset);
	void HandleResourceEnd(unsigned int rtrID, SectionInfo* info);
	void HandleResourceCancel(unsigned int rtrID);
	void HandleResourceReady(unsigned int rtrID, SectionInfo* info);
	bool CanUpdate();
	void* GetBasePointer(unsigned int rtrID);
	unsigned int GetSize(unsigned int rtrID);
	unsigned int GetRefCount(unsigned int rtrID);
	unsigned int FindResourceA(unsigned int rtrID);

	struct DTPDataInfoHeader
	{
		unsigned __int16 m_refCount;
		unsigned __int16 m_versionID;
		bool m_isStreaming;

		char pad[10];
	};

	static DTPDataInfoHeader* m_dtpDataInstances[0x8000];
	static void* GetPointer(unsigned int rtrID);
};