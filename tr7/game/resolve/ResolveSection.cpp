#include <string.h>

#include "ResolveSection.h"
#include "AnimationSection.h"
#include "DTPDataSection.h"
#include "ShaderLibSection.h"

ResolveSection* ResolveSection::s_pSection[10];

ResolveSection* CreateTextureSection();
ResolveSection* CreateWaveSection();

void ResolveSection::Init()
{
	s_pSection[SECTION_GENERAL] = new GenericSection();
	s_pSection[SECTION_EMPTY] = new EmptySection();
	s_pSection[SECTION_ANIMATION] = new AnimationSection();
	s_pSection[SECTION_PUSHBUFFER_WC] = new GenericSection();
	s_pSection[SECTION_PUSHBUFFER] = new GenericSection();
	s_pSection[SECTION_TEXTURE] = CreateTextureSection();
	s_pSection[SECTION_WAVE] = CreateWaveSection();
	s_pSection[SECTION_DTPDATA] = new DTPDataSection();
	s_pSection[SECTION_SHADERLIB] = new ShaderLibSection();
}

void ResolveSection::ReleaseResource(unsigned int rtrID)
{
}

void ResolveSection::HandleResourceEnd(unsigned int rtrID, SectionInfo * info)
{
}

void ResolveSection::HandleResourceCancel(unsigned int rtrID)
{
}

void ResolveSection::HandleResourceReady(unsigned int rtrID, SectionInfo * info)
{
}

bool ResolveSection::CanUpdate()
{
	return false;
}

void* ResolveSection::GetBasePointer(unsigned int rtrID)
{
	return nullptr;
}

unsigned int ResolveSection::GetSize(unsigned int rtrID)
{
	return 0;
}

unsigned int ResolveSection::GetRefCount(unsigned int rtrID)
{
	return 1;
}

unsigned int ResolveSection::FindResourceA(unsigned int rtrID)
{
	return 0;
}

unsigned int GenericSection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = false;

	return (unsigned int)new char[size];
}

int GenericSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	memcpy((void*)(offset + rtrID), pData, dataSize);

	return dataSize;
}

void GenericSection::HandleResourceCancel(unsigned int rtrID)
{
	delete (void*)rtrID;
}

void* GenericSection::GetBasePointer(unsigned int rtrID)
{
	return (void*)rtrID;
}

unsigned int GenericSection::GetSize(unsigned int rtrID)
{
	return 0; // not possible with standard allocator?
}

unsigned int GenericSection::FindResourceA(unsigned int rtrID)
{
	return rtrID;
}

unsigned int EmptySection::StartResource(unsigned int rtrID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	*skip = true;

	return 0;
}

int EmptySection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	return dataSize;
}