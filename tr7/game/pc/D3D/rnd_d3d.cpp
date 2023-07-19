#include "rnd_d3d.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

bool D3D_PreInit()
{
	return cdc::PCDeviceManager::Create() != nullptr;
}