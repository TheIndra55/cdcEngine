#pragma once

#include "Resolve.h"

class ResolveSection
{
public:
	static ResolveSection* s_pSection[NUM_SECTION_TYPES];
	static void Init();

	virtual unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip) = 0;
	virtual void ReleaseResource(unsigned int rtrID);
	virtual int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset) = 0;
	virtual void HandleResourceEnd(unsigned int rtrID, SectionInfo* info);
	virtual void HandleResourceCancel(unsigned int rtrID);
	virtual void HandleResourceReady(unsigned int rtrID, SectionInfo* info);
	virtual bool CanUpdate();
	virtual void* GetBasePointer(unsigned int rtrID);
	virtual unsigned int GetSize(unsigned int rtrID);
	virtual unsigned int GetRefCount(unsigned int rtrID);
	virtual unsigned int FindResourceA(unsigned int rtrID);
};

class GenericSection : public ResolveSection
{
public:
	unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip);
	int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset);
	void HandleResourceCancel(unsigned int rtrID);
	void* GetBasePointer(unsigned int rtrID);
	unsigned int GetSize(unsigned int rtrID);
	unsigned int FindResourceA(unsigned int rtrID);
};

class EmptySection : public ResolveSection
{
	unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip);
	int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset);
};