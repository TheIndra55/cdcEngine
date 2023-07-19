#include "PCStateManager.h"

cdc::PCStateManager::PCStateManager() : PCInternalResource()
{
	OnConstruct();
}

bool cdc::PCStateManager::OnCreateDevice()
{
	return true;
}

void cdc::PCStateManager::OnDestroyDevice()
{
}