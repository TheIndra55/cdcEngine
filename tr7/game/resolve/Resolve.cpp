#include <string.h>

#include "Resolve.h"
#include "ResolveSection.h"

#include "cdc/runtime/cdcSys/Assert.h"

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

	receiver->m_retFunc = callback;
	receiver->m_retData = context1;
	receiver->m_state = ResolveReceiver::RESOLVE_LOAD_START;
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
	request->Submit(cdc::FileRequest::NORMAL);

	return object;
}

int ResolveReceiver::ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset)
{
	if (dataSize == 0)
	{
		return dataSize;
	}

	auto ptr = data;
	auto remaining = dataSize;

	int numRead = 0;

	while (true)
	{
		switch (m_state)
		{
		case RESOLVE_LOAD_START:
		{
			auto header = (SectionList*)ptr;

			if (header->versionNumber != 14)
			{
				cdc::FatalError("Wrong mkloadob version data %x, code %x\nrebuild needed\n", header->versionNumber, 14);
			}

			m_numSections = header->numSections;

			m_section = new SectionInfo[m_numSections];
			m_rtrID = new unsigned int[m_numSections];

			memcpy(m_section, header->blockList, m_numSections * sizeof(SectionInfo));

			*m_rtrID = -1;

			auto fileSystem = GetFS();
			auto currentSpecMask = fileSystem->GetSpecialisationMask();

			DeltaMap* addList;
			DeltaMap* removeList;

			int numAdded = 0;
			int numRemoved = 0;
			DeltaMap* curSectionAddr = nullptr;

			if (m_reload)
			{
				addList = new DeltaMap[m_numSections];
				removeList = new DeltaMap[m_numSections];

				curSectionAddr = addList;
			}

			unsigned int compositeMask = 0xFFFFFFFF;

			int numEntries = 0;
			int oldRecordIndex = 0;

			for (int i = 0; i < m_numSections; i++)
			{
				auto section = &m_section[i];
				compositeMask &= section->specMask;

				if ((currentSpecMask & section->specMask) == currentSpecMask)
				{
					if (m_reload)
					{
						if ((m_previousSpecMask & section->specMask) == m_previousSpecMask)
						{
							oldRecordIndex++;
						}
						else
						{
							curSectionAddr[numAdded].absolute = i;
							curSectionAddr[numAdded++].relative = numEntries;
						}
					}

					m_rtrID[i] = -1;

					numEntries++;
				}
				else
				{
					m_rtrID[i] = 0;

					section->skip = true;

					if (m_reload)
					{
						if ((m_previousSpecMask & section->specMask) == m_previousSpecMask)
						{
							auto sectionType = ResolveSection::s_pSection[section->type];

							if (sectionType->CanUpdate())
							{
								removeList[numRemoved].absolute = i;
								removeList[numRemoved++].relative = oldRecordIndex;
							}
							else
							{
								auto entry = m_pPreviousRecord->m_pEntry[oldRecordIndex];

								if (entry.sectionType != SECTION_EMPTY && entry.rtrID != -1)
								{
									ResolveSection::s_pSection[entry.sectionType]->ReleaseResource(entry.rtrID);
								}
							}

							oldRecordIndex++;
						}
					}
				}
			}

			m_pObject->m_compositeSpecMask = compositeMask;

			auto record = new SectionRecord();

			record->m_numEntries = numEntries;
			record->m_pEntry = new SectionRecord::Entry[numEntries];

			m_pReleaseRecord = record;
			oldRecordIndex = 0;

			auto v72 = 0;

			for (int i = 0; i < m_numSections; i++)
			{
				auto section = &m_section[i];
				auto rtr = &m_rtrID[i];

				auto v73 = *rtr;

				if (*rtr == -1)
				{
					if (m_reload && (m_previousSpecMask & section->specMask) == m_previousSpecMask)
					{
						auto prevRtr = m_pPreviousRecord->m_pEntry[oldRecordIndex++].rtrID;

						auto record = m_pReleaseRecord;
						record->m_pEntry[v72].sectionType = section->type;
						record->m_pEntry[v72++].rtrID = prevRtr;

						section->skip = true;
					}
					else
					{
						auto sectionType = ResolveSection::s_pSection[section->type];

						bool skip = false;
						m_rtrID[i] = sectionType->StartResource(section->id, section->versionID, section->size, &skip);

						section->skip = skip;

						auto record = m_pReleaseRecord;
						record->m_pEntry[v72].sectionType = section->type;
						record->m_pEntry[v72++].rtrID = m_rtrID[i];
					}
				}
				else
				{
					if ((currentSpecMask & section->specMask) == currentSpecMask)
					{
						auto record = m_pReleaseRecord;
						record->m_pEntry[v72].sectionType = section->type;
						record->m_pEntry[v72++].rtrID = v73;
					}
					else
					{
						*rtr = -1;
					}

					if (m_reload && (m_previousSpecMask & section->specMask) == m_previousSpecMask)
					{
						oldRecordIndex++;
					}
				}
			}

			numRead = sizeof(SectionList) + (m_numSections * sizeof(SectionInfo));

			m_state = RESOLVE_LOAD_SECTION_CHOICE;
			m_sectionIndex = 0;
			m_resolvePtr = nullptr;

			if (m_reload)
			{
				if (m_pPreviousRecord)
				{
					delete m_pPreviousRecord;
				}
			}
			else if (m_retPointer)
			{
				*m_retPointer = ResolveSection::s_pSection[m_section->type]->GetBasePointer(*m_rtrID);
			}

			if (remaining == numRead)
				return dataSize;

			remaining -= numRead;
			ptr = (char*)(ptr + numRead);

			break;
		}
		case RESOLVE_LOAD_SECTION_CHOICE:
		{
			m_state = RESOLVE_LOAD_SECTION_RELOCS_LENGTH;

			numRead = 0;

			int i;
			for (i = 0; i < m_numSections; i++)
			{
				auto section = &m_section[i];

				if (!section->skip)
				{
					break;
				}

				numRead += section->size + (sizeof(Relocation) * section->packed.numRelocations);
			}

			if (i == m_numSections)
			{
				m_state = RESOLVE_LOAD_END;
			}
			else
			{
				m_sectionIndex = i;
			}

			if (numRead <= remaining)
			{
				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}

			return dataSize + numRead - remaining;
		}
		case RESOLVE_LOAD_SECTION_RELOCS_LENGTH:
			m_resolveLen = m_section[m_sectionIndex].packed.numRelocations;
			m_state = RESOLVE_LOAD_SECTION_RELOCS;

			if (remaining == numRead)
				return dataSize;

			remaining -= numRead;
			ptr = (char*)(ptr + numRead);

		case RESOLVE_LOAD_SECTION_RELOCS:
			if (!m_resolvePtr)
			{
				if (m_resolveLen)
				{
					m_resolveList = (SectionResolveList*)new char[sizeof(SectionResolveList) + (sizeof(Relocation) * m_resolveLen)];
					m_resolveList->numRelocations = m_resolveLen;

					m_resolveLen = m_resolveLen * sizeof(Relocation);
					m_resolvePtr = (char*)m_resolveList->relocation;
				}
				else
				{
					m_resolveList = nullptr;
				}
			}

			if (m_resolveLen > remaining)
			{
				m_resolveLen -= remaining;

				memcpy(m_resolvePtr, ptr, remaining);
				m_resolvePtr += remaining;

				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}

			memcpy(m_resolvePtr, ptr, m_resolveLen);

			m_state = RESOLVE_LOAD_SECTION_BODY;
			m_bodyLen = m_section[m_sectionIndex].size;

			if (m_bodyLen)
			{
				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}

			m_sectionIndex++;
			m_resolveList = nullptr;

			if (m_sectionIndex >= m_numSections)
			{
				m_state = RESOLVE_LOAD_END;
			}
			else
			{
				m_resolvePtr = nullptr;
				m_state = RESOLVE_LOAD_SECTION_CHOICE;
			}

			if (remaining == numRead)
				return dataSize;

			remaining -= numRead;
			ptr = (char*)(ptr + numRead);

			break;

		case RESOLVE_LOAD_SECTION_SKIP_RELOCS:
			if (!m_resolvePtr)
			{
				m_resolveLen = m_resolveLen * sizeof(Relocation);
				m_resolvePtr = nullptr;
			}

			numRead = remaining;
			if (m_resolveLen > remaining)
			{
				m_resolveLen -= remaining;

				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}

			m_state = RESOLVE_LOAD_SECTION_SKIP_BODY;
			m_bodyLen = m_section[m_sectionIndex].size;

			if (m_bodyLen)
			{
				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}

			m_sectionIndex++;

			if (m_sectionIndex > m_numSections)
			{
				m_state = RESOLVE_LOAD_END;
			}
			else
			{
				m_resolvePtr = nullptr;
				m_state = RESOLVE_LOAD_SECTION_CHOICE;
			}

			if (remaining == numRead)
				return dataSize;

			remaining -= numRead;
			ptr = (char*)(ptr + numRead);

			break;

		case RESOLVE_LOAD_SECTION_BODY:
		{
			auto remain = remaining;

			if (remaining > m_bodyLen)
			{
				remain = m_bodyLen;
			}

			auto section = &m_section[m_sectionIndex];
			auto sectionType = ResolveSection::s_pSection[section->type];

			numRead = sectionType->HandleResourceData(m_rtrID[m_sectionIndex], ptr, remain, section->size - m_bodyLen);

			auto left = m_bodyLen - numRead;
			m_bodyLen = left;
			if (numRead < remain)
				return dataSize + numRead - remaining;

			if (!left)
			{
				auto curSectionAddr = sectionType->GetBasePointer(m_rtrID[m_sectionIndex]);

				if (m_resolveList)
				{
					for (int i = 0; i < m_resolveList->numRelocations; i++)
					{
						auto relocation = &m_resolveList->relocation[i];

						auto info = relocation->typeAndSectionInfo;
						auto type = ResolveSection::s_pSection[info.type];
						auto addr = (int*)((int)curSectionAddr + relocation->offset);

						switch ((Relocation::Type)relocation->typeAndSectionInfo.type)
						{
						case Relocation::POINTER:
						{
							auto offset = *addr;
							auto base = (int)type->GetBasePointer(m_rtrID[info.sectionIndexOrType]);

							*addr = base + offset;
						}

						break;
						case Relocation::RESOURCE_ID:
							*addr = type->FindResourceA(info.sectionIndexOrType);

							break;
						case Relocation::RESOURCE_ID16:
							cdc::FatalError("Not implemented");

							break;
						case Relocation::RESOURCE_POINTER:
							cdc::FatalError("Not implemented");

							break;
						}
					}
				}

				sectionType->HandleResourceEnd(m_rtrID[m_sectionIndex], &m_section[m_sectionIndex]);

				m_sectionIndex++;
				m_resolveList = nullptr;

				if (m_sectionIndex >= m_numSections)
				{
					m_state = RESOLVE_LOAD_END;
				}
				else
				{
					m_resolvePtr = nullptr;
					m_state = RESOLVE_LOAD_SECTION_CHOICE;
				}

				if (remaining == numRead)
					return dataSize;

				remaining -= numRead;
				ptr = (char*)(ptr + numRead);

				break;
			}
		}
		case RESOLVE_LOAD_SECTION_SKIP_BODY:
			if (m_bodyLen <= remaining)
			{
				m_sectionIndex++;

				if (m_sectionIndex > m_numSections)
				{
					m_state = RESOLVE_LOAD_END;
				}
				else
				{
					m_resolvePtr = nullptr;
					m_state = RESOLVE_LOAD_SECTION_CHOICE;
				}
			}
			else
			{
				m_bodyLen -= remaining;
			}

			if (remaining == numRead)
				return dataSize;

			remaining -= numRead;
			ptr = (char*)(ptr + numRead);

			break;
		}
	}
}

void ResolveReceiver::ReceiveStarted(cdc::FileRequest* request, unsigned int requestSize)
{
}

void ResolveReceiver::ReceiveCancelled(cdc::FileRequest* request)
{
}

void ResolveReceiver::ReceiveDone(cdc::FileRequest* request)
{
	for (int i = 0; i < m_numSections; i++)
	{
		auto section = &m_section[i];

		if (!section->skip)
		{
			ResolveSection::s_pSection[section->type]->HandleResourceReady(m_rtrID[i], section);
		}
	}

	if (!m_reload && m_retFunc)
	{
		auto base = ResolveSection::s_pSection[m_section->type]->GetBasePointer(*m_rtrID);

		// call the resolve callback
		m_retFunc(base, m_retData, m_retData2, m_pObject);
	}

	m_pObject->m_pRequest = nullptr;
	delete this;
}