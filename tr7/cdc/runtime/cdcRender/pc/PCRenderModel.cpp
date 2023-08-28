#include "PCRenderModel.h"
#include "PCRenderDevice.h"

#include "cdc/runtime/cdcSys/Assert.h"

void cdc::RenderModelInstance::Release()
{
}

int cdc::RenderModelInstance::GetMemorySize()
{
	return 0;
}

void cdc::RenderMesh::Release()
{
}

int cdc::RenderMesh::GetMemorySize()
{
	return 0;
}

cdc::PCRenderModel::PCRenderModel(PCModelData* data, TextureMap** pTextures, IShaderLib** pShaders, RenderMesh* pRefMesh, PCRenderDevice* pRenderDevice)
{
	m_pHeader = data;
	m_numPrimGroups = 0;
	m_pPrimGroups = nullptr;
	m_numModelBatches = 0;
	m_pModelBatches = nullptr;
	m_pBones = 0;
	m_pTextures = nullptr;
	m_pIndexData = nullptr;
	m_pIndexBuffer = nullptr;

	if (data->magicNumber != 0x39444350)
	{
		cdc::FatalError("Corrupt mesh data passed into PCRenderMesh constructor!");
	}

	if ((data->flags & PCModelData::kSkySphere) != 0)
	{
		data->boundingSphereRadius = data->boundingSphereRadius * 131072.f;
	}

	if (data->modelType == PCModelData::kTypeDummy)
	{
		return;
	}

	m_pPrimGroups = (PCPrimGroup*)((char*)data + data->primGroupOffset);
	m_numPrimGroups = data->numPrimGroups;

	for (int i = 0; i < data->numMaterials; i++)
	{
		// TODO
	}

	for (int i = 0; i < m_numPrimGroups; i++)
	{
		// TODO
	}

	if (data->numModelBatches)
	{
		m_pModelBatches = (PCModelBatch*)((char*)data + data->modelBatchOffset);
		m_numModelBatches = data->numModelBatches;

		for (int i = 0; i < m_numModelBatches; i++)
		{
			auto batch = &m_pModelBatches[i];
			batch->pVertexData = (char*)data + batch->vertexDataOffset;

			auto buffer = new PCStaticVertexBuffer(pRenderDevice->m_pStaticPool);
			buffer->Create(batch->pVertexData, batch->vertexElements, batch->vertexStride, batch->numVertices);

			batch->pVertexBuffer = buffer;

			if (data->modelType == PCModelData::kTypeSkinned)
			{
				batch->pSkinMap = (unsigned int*)((char*)data + batch->skinMapOffset);
			}
		}
	}

	if (data->numIndices)
	{
		m_pIndexData = (unsigned short*)((char*)data + data->indexOffset);

		auto buffer = new PCStaticIndexBuffer(pRenderDevice->m_pStaticPool);
		buffer->Create(m_pIndexData, data->numIndices);

		m_pIndexBuffer = buffer;
	}

	if (data->modelType == PCModelData::kTypeSkinned)
	{
		m_pBones = (unsigned int*)((char*)data + data->boneOffset);
	}

	for (int i = 0; i < data->numPixmaps; i++)
	{
		// TODO
	}

	BuildStencilData();
}

void cdc::PCRenderModel::BuildStencilData()
{
}

cdc::PCRenderModelInstance::PCRenderModelInstance(PCRenderModel* pRenderModel, PCRenderDevice* pRenderDevice)
{
	m_pRenderModel = pRenderModel;
	m_center = pRenderModel->m_pHeader->boundingSphereCenter;
	m_radius = pRenderModel->m_pHeader->boundingSphereRadius;
}

void cdc::PCRenderModelInstance::Draw(cdc::Matrix* pRootMatrix)
{
}