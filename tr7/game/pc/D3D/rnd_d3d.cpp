#include <intrin.h>

#include "rnd_d3d.h"
#include "d3dtextr.h"
#include "d3ddraw.h"

#include "game/pc/gamewindow.h"
#include "game/local/localstr.h"
#include "game/Scene/Scene.h"
#include "game/pc/cdcRenderLayer/RenderLayer.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

bool D3D_HasISSE;
bool D3D_HasMMX;

bool D3D_InNextGenMode;
bool D3D_InScene;

cdc::PCScene g_dummyScene;

static DeviceCallback* s_pDeviceCallback;

static cdc::PCIndexPool* s_pDynamicIndexPool;
static cdc::PCDrawableList* s_pDrawLists[14];

static cdc::Matrix s_projectionMatrix;
float gProjectionDistance = 512.f;

bool D3D_PreInit()
{
	int regs[4];
	__cpuid(regs, 1);

	D3D_HasISSE = (regs[3] & 1 << 25) != 0;
	D3D_HasMMX = (regs[3] & 1 << 23) != 0;

	return cdc::PCDeviceManager::Create() != nullptr;
}

bool D3D_Init()
{
	if (!cdc::PCDeviceManager::s_pInstance->Init(gGameHwnd, &settings))
	{
		return false;
	}

	cdcRenderLayer::Create(gGameHwnd);
	D3DTEX_Init();

	s_pDeviceCallback = new DeviceCallback();

	return true;
}

void D3D_BeginScene(bool enableReflection)
{
	cdcRenderLayer::BeginFrame();

	if (D3D_InNextGenMode)
	{
		cdcRenderLayer::SetupViewport();

		if (enableReflection)
		{
			cdcRenderLayer::RenderReflectionBuffer();
		}

		cdcRenderLayer::D3D_BeginScene();
		D3D_InScene = true;
	}
	else
	{
		if (cdc::PCDeviceManager::s_pInstance->IsStatusOk())
		{
		}

		D3D_InScene = true;
	}
}

void D3D_ActivateMatrix(float screenZ)
{
	cdc::Matrix m;
	memcpy(&m, &s_projectionMatrix, sizeof(m));

	if (screenZ != 0.f)
	{
		// TODO
	}

	cdc::PCDeviceManager::s_pInstance->GetStateManager()->SetVertexConstantMatrix4x4(0, &m);
}

void D3D_PrevGen_EndScene()
{
	if (cdc::PCDeviceManager::s_pInstance->IsStatusOk())
	{
		memset(&s_projectionMatrix, 0, sizeof(s_projectionMatrix));

		// TODO figure out these values
		s_projectionMatrix.col2.z = 1.0001221f;
		s_projectionMatrix.col2.w = 1.0f;
		s_projectionMatrix.col3.z = -16.001953f;
		s_projectionMatrix.col0.x = gProjectionDistance * 0.001953125f + gProjectionDistance * 0.001953125f;
		s_projectionMatrix.col1.y = gProjectionDistance * 0.002232143f * -2.f;

		D3D_ActivateMatrix(0.f);

		s_pDynamicIndexPool->EndScene();

		s_pDrawLists[0]->Draw(cdc::PC_PASS_OPAQUE, &g_dummyScene);
		s_pDrawLists[0]->Clear();
	}
}

void D3D_EndScene()
{
	if (D3D_InScene)
	{
		D3D_InScene = false;

		if (D3D_InNextGenMode)
		{
			cdcRenderLayer::D3D_EndScene();
		}
		else
		{
			D3D_PrevGen_EndScene();
		}

		cdcRenderLayer::EndFrame();

		if (!D3D_InNextGenMode && cdc::PCDeviceManager::s_pInstance->IsStatusOk())
		{
			// TODO
		}
	}
}

void D3D_PrevGen_Init()
{
	s_pDynamicIndexPool = new cdc::PCIndexPool(16);

	for (int i = 0; i < 14; i++)
	{
		s_pDrawLists[i] = new cdc::PCDrawableList();
	}
}

void D3D_SetFog(int r, int g, int b, float fognear, float fogfar)
{
	if (cdc::PCDeviceManager::s_pInstance->IsStatusOk())
	{
		auto fog = 1.f / (fognear - fogfar);

		float const4[4] = { fog, -(fog * fogfar), 0.f, 1.f };
		cdc::PCDeviceManager::s_pInstance->GetStateManager()->SetVertexConstants(4, const4, 1);
	}
}

cdc::PCDrawableList* GetDrawListByTpageId(unsigned int tpageid, bool bReflect)
{
	// TODO
	return s_pDrawLists[0];
}

cdc::PCIndexPool* GetIndexPool()
{
	return s_pDynamicIndexPool;
}

DeviceCallback::DeviceCallback() : PCInternalResource()
{
	OnConstruct();
}

bool DeviceCallback::OnCreateDevice()
{
	if (!m_bInitialized)
	{
		m_bInitialized = true;

		auto inNextGen = cdc::PCDeviceManager::s_pInstance->m_isPixelShader20;
		D3D_InNextGenMode = inNextGen;

		if (!inNextGen)
		{
			D3D_PrevGen_Init();

			auto device = cdc::PCDeviceManager::s_pInstance->GetD3DDevice();

			device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
			device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			auto stateManager = cdc::PCDeviceManager::s_pInstance->GetStateManager();

			stateManager->SetRenderState(D3DRS_FOGVERTEXMODE, 0);
			stateManager->SetRenderState(D3DRS_FOGTABLEMODE, 0);
			stateManager->SetRenderState(D3DRS_FOGENABLE, TRUE);

			stateManager->SetPixelShader(nullptr);

			D3D_SetFog(0, 0, 0, 131070.f, 131071.f);
		}

		localstr_set_gfx_gen(inNextGen);
		SceneLayer::SetEnabled(inNextGen);

		auto renderDevice = cdcRenderLayer::GetPCRD();
		renderDevice->ResolveDeferredReleases();

		D3DDRAW_Init();
	}

	return true;
}

void DeviceCallback::OnDestroyDevice()
{
	if (m_bInitialized)
	{
		m_bInitialized = false;

		InvalidateRect(gGameHwnd, NULL, TRUE);
	}
}

unsigned int cdc::PCScene::GetId()
{
	return 0;
}

void cdc::PCScene::Begin()
{
}

void cdc::PCScene::End()
{
}