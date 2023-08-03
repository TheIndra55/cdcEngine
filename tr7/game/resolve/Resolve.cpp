#include <string.h>

#include "Resolve.h"
#include "ResolveSection.h"

#include <game/Main.h>

static bool s_initResolve = true;
static ResolveObject* s_pLastObject;

ResolveObject::ResolveObject(const char* fileName)
{
	m_pNext = s_pLastObject;
	m_pRequest = nullptr;
	m_pRecord = nullptr;

	s_pLastObject = this;

	m_compositeSpecMask = 0;

	m_pFileName = new char[strlen(fileName) + 1];
	strcpy(m_pFileName, fileName);
}

void Resolve::ChangeSpecialisation(unsigned int oldSpecMask, unsigned int newSpecMask)
{
}

ResolveObject* Resolve::Load(const char* fileName, void(*callback)(void*, void*, void*, ResolveObject*), void* context1, void* context2, void** pointer)
{
	if (s_initResolve)
	{
		ResolveSection::Init();

		s_initResolve = false;
	}

	auto object = new ResolveObject(fileName);
	auto receiver = new ResolveReceiver();

	receiver->m_state = 0;
	receiver->m_numSections = 0;
	receiver->m_section = nullptr;
	receiver->m_rtrID = 0;
	receiver->m_sectionIndex = 0;
	receiver->m_bodyLen = 0;
	receiver->m_resolveList = nullptr;
	receiver->m_resolveLen = 0;
	receiver->m_pObject = object;
	receiver->m_reload = false;
	receiver->m_retData2 = context2;
	receiver->m_retPointer = pointer;

	auto fileSystem = GetFS();
	auto request = fileSystem->RequestRead(receiver, fileName, 0);

	object->m_pRequest = request;

	return object;
}

int ResolveReceiver::ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset)
{
	return 0;
}

void ResolveReceiver::ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize)
{
}

void ResolveReceiver::ReceiveCancelled(cdc::FileRequest* request)
{
}

void ResolveReceiver::ReceiveDone(cdc::FileRequest* request)
{
}