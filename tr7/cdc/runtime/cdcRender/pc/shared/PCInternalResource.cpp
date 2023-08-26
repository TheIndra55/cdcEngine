#include "PCInternalResource.h"
#include "PCDeviceManager.h"

cdc::PCInternalResource::PCInternalResource()
	: m_pPrev(nullptr), m_pNext(nullptr)
{
	PCDeviceManager::s_pInstance->AddDeviceResource(this);
}

void cdc::PCInternalResource::OnConstruct()
{
	auto deviceManager = PCDeviceManager::s_pInstance;

	if (deviceManager->m_status == PCDeviceManager::kStatusOk && !deviceManager->m_bIsRecreatingResources)
	{
		OnCreateDevice();
	}
}

void cdc::PCInternalResource::OnDestruct()
{
	OnDestroyDevice();
}

void cdc::PCInternalResource::OnCreateResourceFailed()
{
	cdc::PCDeviceManager::s_pInstance->OnCreateResourceFailed();
}