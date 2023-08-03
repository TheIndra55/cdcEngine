#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

struct SectionInfo
{
};

struct SectionResolveList
{
};

class SectionRecord
{
};

class ResolveObject
{
public:
	cdc::FileRequest* m_pRequest;
	SectionRecord* m_pRecord;
	ResolveObject* m_pNext;

	unsigned int m_compositeSpecMask;
	char* m_pFileName;

	ResolveObject(const char* fileName);
};

class ResolveReceiver : public cdc::FileReceiver
{
public:
	ResolveObject* m_pObject;
	SectionRecord* m_pReleaseRecord;

	bool m_reload;
	int m_state;

	int m_numSections;
	SectionInfo* m_section;
	unsigned int* m_rtrID;
	int m_sectionIndex;

	int m_bodyLen;
	int m_resolveLen;

	SectionResolveList* m_resolveList;

	void* m_retData2;
	void** m_retPointer;

	int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset);
	void ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize);
	void ReceiveCancelled(cdc::FileRequest* request);
	void ReceiveDone(cdc::FileRequest* request);
};

class Resolve
{
public:
	static void ChangeSpecialisation(unsigned int oldSpecMask, unsigned int newSpecMask);
	static ResolveObject* Load(const char* fileName, void(*callback)(void*, void*, void*, ResolveObject*), void* context1, void* context2, void** pointer);
};