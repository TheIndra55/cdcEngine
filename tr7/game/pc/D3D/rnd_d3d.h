#pragma once

#include "cdc/runtime/cdcRender/pc/shared/PCInternalResource.h"
#include "cdc/runtime/cdcRender/pc/shared/PCScene.h"
#include "cdc/runtime/cdcRender/pc/shared/PCIndexPool.h"
#include "cdc/runtime/cdcRender/pc/shared/PCDrawable.h"

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

extern cdc::PCScene g_dummyScene;

cdc::PCIndexPool* GetIndexPool();
cdc::PCDrawableList* GetDrawListByTpageId(unsigned int tpageid, bool bReflect);

bool D3D_PreInit();
bool D3D_Init();
void D3D_BeginScene(bool enableReflection);
void D3D_EndScene();
void D3D_PrevGen_Init();