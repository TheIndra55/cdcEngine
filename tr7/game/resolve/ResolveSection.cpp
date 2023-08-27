#include <string.h>

#include "ResolveSection.h"
#include "DTPDataSection.h"

ResolveSection* ResolveSection::s_pSection[10];

void ResolveSection::Init()
{
	// TODO
	for (int i = 0; i < 10; i++)
	{
		s_pSection[i] = new GenericSection();
	}

	s_pSection[7] = new DTPDataSection();
}

unsigned int GenericSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = false;

	return (unsigned int)new char[size];
}

void GenericSection::ReleaseResource(unsigned int rtrID)
{
}

int GenericSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	memcpy((void*)(offset + rtrID), pData, dataSize);

	return dataSize;
}

void GenericSection::HandleResourceEnd(unsigned int rtrID, SectionInfo* info)
{
}

void GenericSection::HandleResourceCancel(unsigned int rtrID)
{
	delete (void*)rtrID;
}

void GenericSection::HandleResourceReady(unsigned int rtrID, SectionInfo* info)
{
}

bool GenericSection::CanUpdate()
{
	return false;
}

void* GenericSection::GetBasePointer(unsigned int rtrID)
{
	return (void*)rtrID;
}

unsigned int GenericSection::GetSize(unsigned int rtrID)
{
	return 0; // not possible with standard allocator?
}

unsigned int GenericSection::GetRefCount(unsigned int rtrID)
{
	return 1;
}

unsigned int GenericSection::FindResourceA(unsigned int rtrID)
{
	return rtrID;
}