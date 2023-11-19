#include <string.h>

#include "DTPDataSection.h"

#include "cdc/runtime/cdcSys/Assert.h"

DTPDataSection::DTPDataInfoHeader* DTPDataSection::m_dtpDataInstances[0x8000];

DTPDataSection::DTPDataSection()
{
	memset(m_dtpDataInstances, 0, sizeof(m_dtpDataInstances));
}

unsigned int DTPDataSection::StartResource(unsigned int resourceID, unsigned __int16 versionID, unsigned int size, bool* skip)
{
	if (resourceID >= 0x8000 || resourceID < 0)
	{
		cdc::FatalError("Out of range DTPData ID!");
		return resourceID;
	}

	auto instance = m_dtpDataInstances[resourceID];

	if (instance)
	{
		instance->m_refCount++;
		*skip = true;
	}
	else
	{
		instance = (DTPDataInfoHeader*)new char[size + 16];
		instance->m_versionID = versionID;
		instance->m_refCount = 1;
		instance->m_isStreaming = true;

		m_dtpDataInstances[resourceID] = instance;

		*skip = false;
	}

	return resourceID;
}

void DTPDataSection::ReleaseResource(unsigned int resourceID)
{
	auto instance = m_dtpDataInstances[resourceID];

	instance->m_refCount--;
	if (instance->m_refCount == 0)
	{
		delete instance;
		m_dtpDataInstances[resourceID] = nullptr;
	}
}

int DTPDataSection::HandleResourceData(unsigned int rtrID, const char* pData, unsigned int dataSize, unsigned int offset)
{
	memcpy((char*)&m_dtpDataInstances[rtrID][1] + offset, pData, dataSize);

	return dataSize;
}

void DTPDataSection::HandleResourceReady(unsigned int rtrID, SectionInfo* info)
{
	m_dtpDataInstances[rtrID]->m_isStreaming = false;
}

void* DTPDataSection::GetBasePointer(unsigned int resourceID)
{
	return &m_dtpDataInstances[resourceID][1];
}

unsigned int DTPDataSection::GetSize(unsigned int rtrID)
{
	cdc::FatalError("Not implemented");

	return 0;
}

unsigned int DTPDataSection::GetRefCount(unsigned int resourceID)
{
	auto instance = m_dtpDataInstances[resourceID];

	if (instance)
	{
		return instance->m_refCount;
	}

	return 0;
}

unsigned int DTPDataSection::FindResourceA(unsigned int resourceID)
{
	return resourceID;
}

void* DTPDataSection::GetPointer(unsigned int rtrID)
{
	auto instance = m_dtpDataInstances[rtrID];

	if (instance && !instance->m_isStreaming)
	{
		return &instance[1];
	}

	return nullptr;
}