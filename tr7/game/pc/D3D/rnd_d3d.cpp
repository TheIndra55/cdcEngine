#include <intrin.h>

#include "rnd_d3d.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

bool D3D_HasISSE;
bool D3D_HasMMX;

bool D3D_PreInit()
{
	int regs[4];
	__cpuid(regs, 1);

	D3D_HasISSE = (regs[3] & 1 << 25) != 0;
	D3D_HasMMX = (regs[3] & 1 << 23) != 0;

	return cdc::PCDeviceManager::Create() != nullptr;
}