#include "RenderLayer.h"

#include "game/pc/gamewindow.h"
#include "game/pc/D3D/d3dtextr.h"
#include "game/resolve/ResolveSection.h"
#include "game/resolve/DTPDataSection.h"

#include "cdc/runtime/cdcRender/pc/PCRenderDevice.h"
#include "cdc/runtime/cdcRender/pc/PCRenderModel.h"
#include "cdc/runtime/cdcRender/pc/PCTexture.h"
#include "cdc/runtime/cdcRender/pc/PCShaderLib.h"

static cdc::PCRenderDevice* s_cdcDev;
static cdc::RenderViewport s_curViewport;

struct TextureList
{
	int numTextures;
	int textureIDs[1];
};

struct MeshData
{
	int size;
	char data[1];
};

struct ModelData
{
	MeshData* meshData;
	TextureList* textureList;
	unsigned int* shaderLibResourceIDs;
};

cdc::TextureMap** BuildTextureListFromData(void* texBlock)
{
	auto list = (TextureList*)texBlock;
	auto textures = new cdc::TextureMap*[list->numTextures];

	for (int i = 0; i < list->numTextures; i++)
	{
		textures[i] = D3DTEX_GetCDCTexture(list->textureIDs[i]);
	}

	return textures;
}

cdc::IShaderLib** BuildShaderLibListFromResourceIDs(unsigned int* pResourceIDs)
{
	return nullptr;
}

cdc::RenderModelInstance* BuildRMIFromData(void* srcData)
{
	auto data = (ModelData*)srcData;
	auto meshData = data->meshData;

	if (meshData->size == 0)
	{
		return nullptr;
	}

	auto rmdCopy = new char[meshData->size];
	memcpy(rmdCopy, meshData->data, meshData->size);

	auto textures = BuildTextureListFromData(data->textureList);
	auto shaderLibs = BuildShaderLibListFromResourceIDs(data->shaderLibResourceIDs);

	auto renderMesh = s_cdcDev->CreateRenderModel(rmdCopy, textures, shaderLibs, nullptr);
	auto instance = s_cdcDev->CreateRenderModelInstance(renderMesh);

	delete[] textures;
	delete[] shaderLibs;

	return instance;
}

cdc::PCRenderDevice* cdcRenderLayer::GetPCRD()
{
	return s_cdcDev;
}

void cdcRenderLayer::DRAW_PrepareObjectForDraw(Object* object)
{
	for (int i = 0; i < object->numModels; i++)
	{
		auto data = DTPDataSection::GetPointer(object->modelList[i]->cdcRenderDataID);

		if (data)
		{
			auto instance = BuildRMIFromData(data);

			if (instance)
			{
				object->modelList[i]->cdcRenderModelData = instance;
			}
		}
	}
} 

void cdcRenderLayer::SetupViewport()
{
}

void cdcRenderLayer::RenderReflectionBuffer()
{
}

void cdcRenderLayer::BeginFrame()
{
	s_cdcDev->BeginFrame();
}

void cdcRenderLayer::EndFrame()
{
	s_cdcDev->SetFullScreenAlpha(0.f);
	s_cdcDev->EndFrame();
}

void cdcRenderLayer::D3D_BeginScene()
{
	s_cdcDev->BeginScene(&s_curViewport);
}

void cdcRenderLayer::D3D_EndScene()
{
	s_cdcDev->EndScene();
}

bool cdcRenderLayer::Create(HWND hWnd)
{
	cdc::PCRenderDeviceCreate(hWnd, gGameWindowStatus.screenWidth, gGameWindowStatus.screenHeight, true);
	s_cdcDev = (cdc::PCRenderDevice*)cdc::RenderDevice::s_pcInstance;

	return true;
}