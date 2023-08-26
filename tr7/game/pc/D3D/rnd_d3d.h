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

extern bool D3D_InNextGenMode;

bool D3D_PreInit();
bool D3D_Init();
void D3D_BeginScene(bool enableReflection);
void D3D_EndScene();