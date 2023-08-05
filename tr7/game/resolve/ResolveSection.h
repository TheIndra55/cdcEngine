#pragma once

#include "Resolve.h"

class ResolveSection
{
public:
	static ResolveSection* s_pSection[10];
	static void Init();

	virtual unsigned int StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip) = 0;
	virtual void ReleaseResource(unsigned int rtrID) = 0;
	virtual int HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset) = 0;
	virtual void HandleResourceEnd(unsigned int rtrID, SectionInfo* info) = 0;
	virtual void HandleResourceCancel(unsigned int rtrID) = 0;
	virtual void HandleResourceReady(unsigned int rtrID, SectionInfo* info) = 0;
	virtual bool CanUpdate() = 0;
	virtual void* GetBasePointer(unsigned int rtrID) = 0;
	virtual unsigned int GetSize(unsigned int rtrID) = 0;
	virtual unsigned int GetRefCount(unsigned int rtrID) = 0;
	virtual unsigned int FindResourceA(unsigned int rtrID) = 0;
};

class GenericSection : public ResolveSection
{
public:
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
};