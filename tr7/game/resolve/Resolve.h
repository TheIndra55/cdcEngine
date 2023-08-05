#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

enum SectionType : char
{
	SECTION_GENERAL,
	SECTION_EMPTY,
	SECTION_ANIMATION,
	SECTION_PUSHBUFFER_WC,
	SECTION_PUSHBUFFER,
	SECTION_TEXTURE,
	SECTION_WAVE,
	SECTION_DTPDATA,
	SECTION_SCRIPT,
	SECTION_SHADERLIB,
	NUM_SECTION_TYPES
};

struct SectionInfo
{
	struct PackedData
	{
		unsigned int hasDebugInfo : 1;
		unsigned int resourceType : 7;
		unsigned int numRelocations : 24;
	};

	int size;
	SectionType type;
	bool skip;
	unsigned __int16 versionID;
	PackedData packed;
	int id;
	unsigned int specMask;
};

static_assert(sizeof(SectionInfo) == 20, "Wrong SectionInfo size");

struct Relocation
{
	enum Type
	{
		POINTER,
		RESOURCE_ID,
		RESOURCE_ID16,
		RESOURCE_POINTER
	};

	struct TypeSectionInfo
	{
		unsigned __int16 type : 3;
		unsigned __int16 sectionIndexOrType : 13;
	};

	TypeSectionInfo typeAndSectionInfo;
	unsigned __int16 typeSpecific;
	unsigned int offset;
};

static_assert(sizeof(Relocation) == 8, "Wrong Relocation size");

struct SectionList
{
	int versionNumber;
	int numSections;
	SectionInfo blockList[0];
};

static_assert(sizeof(Relocation) == 8, "Wrong SectionList size");

struct SectionResolveList
{
	int numRelocations;
	Relocation relocation[1];
};

class SectionRecord
{
public:
	struct Entry
	{
		unsigned int rtrID;
		unsigned __int16 sectionType;
	};

	Entry* m_pEntry;
	unsigned int m_numEntries;
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
	enum ResolveState
	{
		RESOLVE_LOAD_START,
		RESOLVE_LOAD_SECTION_CHOICE,
		RESOLVE_LOAD_SECTION_RELOCS_LENGTH,
		RESOLVE_LOAD_SECTION_RELOCS,
		RESOLVE_LOAD_SECTION_SKIP_RELOCS,
		RESOLVE_LOAD_SECTION_BODY,
		RESOLVE_LOAD_SECTION_SKIP_BODY,
		RESOLVE_LOAD_END
	};

	ResolveObject* m_pObject;
	SectionRecord* m_pReleaseRecord;

	bool m_reload;
	ResolveState m_state;

	int m_numSections;
	SectionInfo* m_section;
	unsigned int* m_rtrID;
	int m_sectionIndex;

	int m_bodyLen;
	int m_resolveLen;
	char* m_resolvePtr;

	SectionResolveList* m_resolveList;

	SectionRecord* m_pPreviousRecord;
	void(*m_retFunc)(void*, void*, void*, ResolveObject*);

	unsigned int m_previousSpecMask;
	void* m_retData;

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