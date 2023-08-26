#pragma once

#include "cdc/runtime/cdcRender/pc/shared/PCInternalResource.h"

class DeviceCallback : public cdc::PCInternalResource
{
private:
	bool m_bInitialized = false;

public:
	DeviceCallback();

	bool OnCreateDevice();
	void OnDestroyDevice();
};

extern bool D3D_HasISSE;
extern bool D3D_HasMMX;

bool D3D_PreInit();
bool D3D_Init();