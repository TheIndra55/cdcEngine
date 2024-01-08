#include "rnd_d3d.h"
#include "d3dterrain.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"
#include "cdc/runtime/cdcRender/pc/shared/PCStaticVertexBuffer.h"
#include "cdc/runtime/cdcRender/pc/shared/PCDrawable.h"
#include "cdc/runtime/cdcRender/pc/shared/PCShaderTable.h"
#include "cdc/runtime/cdcRender/pc/shared/PCDynamicIndexBuffer.h"

static D3DVERTEXELEMENT9 TerrainShaderDeclSpec[] =
{
	{ 0, 0,  D3DDECLTYPE_SHORT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 8,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
	{ 0, 12, D3DDECLTYPE_SHORT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 16, D3DDECLTYPE_SHORT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	D3DDECL_END()
};

static D3DVERTEXELEMENT9 EnvTerrainShaderDeclSpec[] =
{
	{ 0, 0,  D3DDECLTYPE_SHORT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 8,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
	{ 0, 12, D3DDECLTYPE_SHORT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 16, D3DDECLTYPE_SHORT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	D3DDECL_END()
};

class TerrainDrawable : cdc::PCDrawable
{
private:
	XboxPcMaterialStripList* m_pMaterial;
	TerrainTextureStripInfo* m_pTextureStrip;
	Terrain* m_pTerrain;
	TerrainGroup* m_pTerrainGroup;

	unsigned int m_tpageid;
	cdc::PCDynamicIndexBuffer* m_pIndexBuffer;
	cdc::PCVertexShader* m_pVertexShader;
	cdc::PCDrawableList* m_pRenderList;
	unsigned int m_nIndices;

public:
	TerrainDrawable(XboxPcMaterialStripList* pMaterial, TerrainTextureStripInfo* pTextureStrip, Terrain* pTerrain, TerrainGroup* pTerrainGroup);

	void InitAddToRenderList(unsigned int nIndices);
	unsigned int GetNumIndices();
	void SetIndices(__int16* indices, int vertexCount);

	void Draw(cdc::PCPass pass, cdc::PCDrawable* pPrevious);
	bool Compare(cdc::PCPass pass, cdc::PCDrawable* pOther);
	unsigned int GetPasses();
	unsigned int GetPolyFlags();
};

static cdc::PCStaticPool* s_pStaticPool;

static Terrain* s_terrain;
static TerrainGroup* s_bspTree;

static cdc::PCVertexFormat* s_envTerrainFormat;

static cdc::PCVertexShaderTable* s_pTerrainShaders;

char* NxShader_Terrain;

void D3DTERRAIN_Init()
{
	if (!D3D_InNextGenMode)
	{
		s_pStaticPool = new cdc::PCStaticPool(6600000, 0, false);
		s_pTerrainShaders = new cdc::PCVertexShaderTable(NxShader_Terrain, false, false);

		s_envTerrainFormat = cdc::PCVertexFormat::Create(EnvTerrainShaderDeclSpec);
	}

	for (int i = 0; i < MAX_STREAM_UNITS; i++)
	{
		if (StreamTracker.StreamList[i].used == 2)
		{
			DRAW_PrepareAreaForDraw(StreamTracker.StreamList[i].level);
		}
	}
}

void DRAW_PrepareAreaForDraw(Level* level)
{
	if (cdc::PCDeviceManager::s_pInstance->IsStatusOk())
	{
		if (D3D_InNextGenMode)
		{
			// TODO
		}
		else
		{
			auto terrain = level->terrain;

			if (!terrain->d3dvertexBuffer && terrain->numTerrainVertices > 0)
			{
				auto buffer = new cdc::PCStaticVertexBuffer(s_pStaticPool);
				buffer->Create(terrain->XboxPcVertexBuffer, TerrainShaderDeclSpec, sizeof(TerrainRenderVertex), terrain->numTerrainVertices);

				terrain->d3dvertexBuffer = buffer;
			}

			if (!terrain->d3dvmoVertexBuffer && terrain->d3dvmoVertexBuffer > 0)
			{
				// TODO
			}
		}
	}
}

static void DrawTerrainStrip(TerrainGroup* bspTree, TerrainTextureStripInfo* tsi, unsigned int lightFlags)
{
	auto materialList = bspTree->XboxPcMaterialList;

	for (auto strip = tsi; strip->vertexCount > 0; strip = strip->nextTexture)
	{
		auto material = &materialList->materials[strip->matIdx];

		materialList->materials[strip->matIdx].strips[material->numStrips] = strip;

		strip->lightFlags = lightFlags;
		material->numStrips++;
	}
}

static void DrawOctreeSphere(OctreeSphere* sphere, unsigned int lightFlags, unsigned int shadowFlags)
{
	if (sphere->numSpheres > 0)
	{
		for (int i = 0; i < sphere->numSpheres; i++)
		{
			DrawOctreeSphere(sphere->spheres[i], lightFlags, 0);
		}
	}
	else
	{
		DrawTerrainStrip(s_bspTree, sphere->strip, lightFlags);
	}
}

static void FinishUpTerrainGroup(TerrainGroup* bspTree, Terrain* terrain)
{
	auto materialList = bspTree->XboxPcMaterialList;

	for (int m = 0; m < materialList->numMaterials; m++)
	{
		auto material = &materialList->materials[bspTree->sortedMaterials[m]];

		for (int s = 0; s < material->numStrips; s++)
		{
			auto strip = material->strips[s];

			auto drawable = new TerrainDrawable(material, strip, terrain, bspTree);

			drawable->InitAddToRenderList(strip->vertexCount);
			drawable->SetIndices(strip->stripVertex, strip->vertexCount);
		}
	}
}

static void ClearTerrainGroupLists(TerrainGroup* bspTree)
{
	auto materialList = bspTree->XboxPcMaterialList;

	for (int i = 0; i < materialList->numMaterials; i++)
	{
		materialList->materials[i].numStrips = 0;
	}
}

void DRAW_DrawTerrainGroup(Terrain* terrain, TerrainGroup* bspTree)
{
	if (cdc::PCDeviceManager::s_pInstance->IsStatusOk() && !D3D_InNextGenMode)
	{
		ClearTerrainGroupLists(bspTree);

		s_terrain = terrain;
		s_bspTree = bspTree;

		DrawOctreeSphere(bspTree->octreeSphere, 0, 0);
		FinishUpTerrainGroup(bspTree, terrain);
	}
}

TerrainDrawable::TerrainDrawable(XboxPcMaterialStripList* pMaterial, TerrainTextureStripInfo* pTextureStrip, Terrain* pTerrain, TerrainGroup* pTerrainGroup)
{
	m_pTerrain = pTerrain;
	m_tpageid = 0;

	SetScene(&g_dummyScene);
	SetSortZ(0.f);

	m_pMaterial = pMaterial;
	m_pTextureStrip = pTextureStrip;
	m_pTerrainGroup = pTerrainGroup;
	m_pIndexBuffer = nullptr;
	m_nIndices = 0;

	m_pRenderList = GetDrawListByTpageId(m_tpageid, pTerrainGroup->flags < 0);
	m_pVertexShader = s_pTerrainShaders->GetVertexShader(0);
}

void TerrainDrawable::InitAddToRenderList(unsigned int nIndices)
{
	m_pIndexBuffer = GetIndexPool()->Create(nIndices);

	// Add to the draw queue
	m_pRenderList->Add(this);
}

unsigned int TerrainDrawable::GetNumIndices()
{
	return m_nIndices;
}

void TerrainDrawable::SetIndices(__int16* indices, int vertexCount)
{
	auto data = m_pIndexBuffer->Lock();

	memcpy(&data[m_nIndices], indices, sizeof(__int16) * vertexCount);
	m_nIndices += vertexCount;
}

void TerrainDrawable::Draw(cdc::PCPass pass, cdc::PCDrawable* pPrevious)
{
	if (pass != cdc::PCPass::PC_PASS_DEPTHTOCOLOR)
	{
		auto stateManager = cdc::PCDeviceManager::s_pInstance->GetStateManager();
		stateManager->SetIndexBuffer(m_pIndexBuffer);

		auto buffer = (cdc::PCVertexBuffer*)m_pTerrain->d3dvertexBuffer;

		stateManager->SetVertexBuffer(buffer, s_envTerrainFormat->m_pD3DVertexDeclaration);
		stateManager->SetVertexShader(m_pVertexShader);

		if (cdc::PCDeviceManager::s_pInstance->GetD3DDevice())
		{
			auto device = cdc::PCDeviceManager::s_pInstance->GetD3DDevice();

			device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				m_pMaterial->vbBaseOffset + buffer->GetBaseVertexIndex(),
				0,
				buffer->GetNumVertices(),
				m_pIndexBuffer->GetBaseIndex(),
				m_nIndices / 3);
		}
	}
}

bool TerrainDrawable::Compare(cdc::PCPass pass, cdc::PCDrawable* pOther)
{
	return false;
}

unsigned int TerrainDrawable::GetPasses()
{
	return 16;
}

unsigned int TerrainDrawable::GetPolyFlags()
{
	return 0;
}