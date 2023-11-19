#include "MultiFileSystem.h"

MultiFileSystem::MultiFileSystem() : m_NumSystems(0), m_Systems()
{
}

cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName)
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		if (m_Systems[i].m_pSystem->FileExists(fileName))
		{
			// TODO reprioritize

			return m_Systems[i].m_pSystem;
		}
	}

	return nullptr;
}

void MultiFileSystem::Add(cdc::FileSystem* pFileSystem, bool bReprioritize, bool bAddToFront)
{
	if (bAddToFront)
	{
		for (int i = m_NumSystems - 1; i >= 0; i--)
		{
			m_Systems[i + 1].m_pSystem = m_Systems[i].m_pSystem;
			m_Systems[i + 1].m_bReprioritize = m_Systems[i].m_bReprioritize;
		}

		m_Systems[0].m_pSystem = pFileSystem;
		m_Systems[0].m_bReprioritize = bReprioritize;
	}
	else
	{
		m_Systems[m_NumSystems].m_pSystem = pFileSystem;
		m_Systems[m_NumSystems].m_bReprioritize = bReprioritize;
	}

	m_NumSystems++;
}

cdc::FileRequest* MultiFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->RequestRead(receiver, fileName, startOffset);
}

cdc::File* MultiFileSystem::OpenFile(const char* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->OpenFile(fileName);
}

bool MultiFileSystem::FileExists(const char* fileName)
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		if (m_Systems[i].m_pSystem->FileExists(fileName))
		{
			return true;
		}
	}

	return false;
}

unsigned int MultiFileSystem::GetFileSize(const char* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->GetFileSize(fileName);
}

void MultiFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		m_Systems[i].m_pSystem->SetSpecialisationMask(specMask);
	}
}

unsigned int MultiFileSystem::GetSpecialisationMask()
{
	if (m_NumSystems == 0)
	{
		return 0xFFFFFFFF;
	}

	return m_Systems[0].m_pSystem->GetSpecialisationMask();
}

cdc::FileSystem::Status MultiFileSystem::GetStatus()
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		if (m_Systems[i].m_pSystem->GetStatus() == BUSY)
		{
			return BUSY;
		}
	}

	return IDLE;
}

void MultiFileSystem::Update()
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		m_Systems[i].m_pSystem->Update();
	}
}

void MultiFileSystem::Synchronize()
{
	for (int i = 0; i < m_NumSystems; i++)
	{
		m_Systems[i].m_pSystem->Synchronize();
	}
}